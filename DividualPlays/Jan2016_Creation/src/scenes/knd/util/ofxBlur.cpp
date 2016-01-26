#include "ofxBlur.h"

float Gaussian(float x, float mean, float variance) {
	x -= mean;
  return (1. / sqrt(TWO_PI * variance)) * exp(-(x * x) / (2 * variance));
}

void GaussianRow(int elements, vector<float>& row, float variance = .2) {
	row.resize(elements);
	for(int i = 0; i < elements; i++) {
		float x = ofMap(i, 0, elements - 1, -1, 1);
		row[i] = Gaussian(x, 0, variance);
	}
}

string generateBlurSource(int radius, float shape) {
	int rowSize = 2 * radius + 1;

	// generate row
	vector<float> row;
	GaussianRow(rowSize, row, shape);

	// normalize row and coefficients
	vector<float> coefficients;
	float sum = 0;
	for(int i = 0; i < row.size(); i++) {
		sum += row[i];
	}
	for(int i = 0; i < row.size(); i++) {
		row[i] /= sum;
	}
	int center = row.size() / 2;
	coefficients.push_back(row[center]);
	for(int i = center + 1; i < row.size(); i += 2) {
		float weightSum = row[i] + row[i + 1];
		coefficients.push_back(weightSum);
	}

	// generate offsets
	vector<float> offsets;
	for(int i = center + 1; i < row.size(); i += 2) {
		int left = i - center;
		int right = left + 1;
		float leftVal = row[i];
		float rightVal = row[i + 1];
		float weightSum = leftVal + rightVal;
		float weightedAverage = (left * leftVal + right * rightVal) / weightSum;
		offsets.push_back(weightedAverage);
	}

	stringstream src;
    src << "#version 120\n";
	src << "#extension GL_ARB_texture_rectangle : enable\n";
	src << "uniform sampler2DRect source;\n";
	src << "uniform vec2 direction;\n";
	src << "void main(void) {\n";
	src << "\tvec2 tc = gl_TexCoord[0].st;\n";
	src << "\tgl_FragColor = " << coefficients[0] << " * texture2DRect(source, tc);\n";
	for(int i = 1; i < coefficients.size(); i++) {
		int curOffset = i - 1;
		src << "\tgl_FragColor += " << coefficients[i] << " * \n";
		src << "\t\t(texture2DRect(source, tc - (direction * " << offsets[i - 1] << ")) + \n";
		src << "\t\ttexture2DRect(source, tc + (direction * " << offsets[i - 1] << ")));\n";
	}
	src << "}\n";

	return src.str();
}

string generateCombineSource(int passes, float downsample) {
	vector<string> combineNames;
	for(int i = 0; i < passes; i++) {
		combineNames.push_back("s" + ofToString(i));
	}
	stringstream src;
    src << "#version 120\n";
	src << "#extension GL_ARB_texture_rectangle : enable\n";
	src << "uniform sampler2DRect " << ofJoinString(combineNames, ",") << ";\n";
	src << "uniform float brightness;\n";
	if(downsample == 1) {
		src << "const float scaleFactor = 1.;\n";
	} else {
		src << "const float scaleFactor = " << downsample << ";\n";
	}
	src << "void main(void) {\n";
	src << "\tvec2 tc = gl_TexCoord[0].st;\n";
	for(int i = 0; i < passes; i++) {
		src << "\tgl_FragColor " << (i == 0 ? "=" : "+=");
		src << " texture2DRect(" << combineNames[i] << ", tc);";
		src << (i + 1 != passes ? " tc *= scaleFactor;" : "");
		src << "\n";
	}
	src << "\tgl_FragColor *= brightness / " << passes << ".;\n";
	src << "}\n";
	return src.str();
}

ofxBlur::ofxBlur()
:scale(1)
,rotation(0)
,brightness(1) {
}

void ofxBlur::setup(int width, int height, int radius, float shape, int passes, float downsample) {
	string blurSource = generateBlurSource(radius, shape);
	if(ofGetLogLevel() == OF_LOG_VERBOSE) {
		cout << "ofxBlur is loading blur shader:" << endl << blurSource << endl;
	}
	blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, blurSource);
	blurShader.linkProgram();

	if(passes > 1) {
		string combineSource = generateCombineSource(passes, downsample);
		if(ofGetLogLevel() == OF_LOG_VERBOSE) {
			cout << "ofxBlur is loading combine shader:" << endl << combineSource << endl;
		}
		combineShader.setupShaderFromSource(GL_FRAGMENT_SHADER, combineSource);
		combineShader.linkProgram();
	}

    base.allocate(width, height);
    base.begin(); ofClear(0); base.end();

    ofFbo::Settings settings;
    settings.useDepth = false;
    settings.useStencil = false;
    settings.numSamples = 0;
    ping.resize(passes);
    pong.resize(passes);
    for(int i = 0; i < passes; i++) {
        ofLogVerbose() << "building ping/pong " << width << "x" << height;
        settings.width = width;
        settings.height = height;
        ping[i].allocate(settings);
        ping[i].begin(); ofClear(0); ping[i].end();
        pong[i].allocate(settings);
        pong[i].begin(); ofClear(0); pong[i].end();
//        ping[i].setDefaultTextureIndex(i);
//        pong[i].setDefaultTextureIndex(i);
        width *= downsample;
        height *= downsample;
    }
}

void ofxBlur::setScale(float scale) {
	this->scale = scale;
}

void ofxBlur::setRotation(float rotation) {
	this->rotation = rotation;
}

void ofxBlur::setBrightness(float brightness) {
	this->brightness = brightness;
}

void ofxBlur::begin() {
	base.begin();
}

void ofxBlur::end() {
	base.end();

	ofPushStyle();
	ofSetColor(255);

	ofVec2f xDirection = ofVec2f(scale, 0).getRotatedRad(rotation);
	ofVec2f yDirection = ofVec2f(0, scale).getRotatedRad(rotation);
	for(int i = 0; i < ping.size(); i++) {
		ofFbo& curPing = ping[i];
		ofFbo& curPong = pong[i];

		// resample previous result into ping
		curPing.begin();
		int w = curPing.getWidth();
		int h = curPing.getHeight();
		if(i > 0) {
			ping[i - 1].draw(0, 0, w, h);
		} else {
			base.draw(0, 0, w, h);
		}
		curPing.end();

		// horizontal blur ping into pong
		curPong.begin();
		blurShader.begin();
		blurShader.setUniformTexture("source", curPing.getTextureReference(), 0);
		blurShader.setUniform2f("direction", xDirection.x, xDirection.y);
		curPing.draw(0, 0);
		blurShader.end();
		curPong.end();

		// vertical blur pong into ping
		curPing.begin();
		blurShader.begin();
		blurShader.setUniformTexture("source", curPong.getTextureReference(), 0);
		blurShader.setUniform2f("direction", yDirection.x, yDirection.y);
		curPong.draw(0, 0);
		blurShader.end();
		curPing.end();
	}

	// render ping back into base
	if(ping.size() > 1) {
		int w = base.getWidth();
		int h = base.getHeight();

        ofPlanePrimitive plane;
        plane.set(w, h);
        plane.mapTexCoordsFromTexture(ping[0].getTextureReference());

		base.begin();
		combineShader.begin();
		for(int i = 0; i < ping.size(); i++) {
			string name = "s" + ofToString(i);
            combineShader.setUniformTexture(name.c_str(),
                                            ping[i].getTextureReference(),
                                            1 + i);
        }
		combineShader.setUniform1f("brightness", brightness);
        ofPushMatrix();
        ofTranslate(w / 2, h / 2);
        plane.draw();
        ofPopMatrix();
		combineShader.end();
		base.end();
	} else {
		base.begin();
		ping[0].draw(0, 0);
		base.end();
	}

	ofPopStyle();
}

ofTexture& ofxBlur::getTextureReference() {
	return base.getTextureReference();
}

void ofxBlur::draw() {
	base.draw(0, 0);
}

void ofxBlur::draw(ofRectangle rect) {
    base.draw(rect);
}
