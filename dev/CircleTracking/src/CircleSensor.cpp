#include "CircleSensor.h"

using namespace ofxCv;
using namespace cv;

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(const ofVec3f& point) {
	return ofVec3f((point.x / Xres - .5f) * point.z * XtoZ,
								 (point.y / Yres - .5f) * point.z * YtoZ,
								 point.z);
}

CircleSensor::CircleSensor()
:backgroundClear(false)
,backgroundCalibrate(false)
,backgroundThreshold(10)
,sampleRadius(12) {
}

void CircleSensor::setup() {
	kinect.init(true, true);
	string serial = ofxKinect::nextAvailableSerial();
	kinect.open(serial);
	cout << "Connected to Kinect " << serial << endl;
	
	imitate(background, kinect, CV_8UC1);
	imitate(valid, kinect, CV_8UC1);
}
void CircleSensor::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		circleFinder.update(kinect);
		
		cloud.clear();
		int width = kinect.getWidth(), height = kinect.getHeight();
		float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
		cloud.setMode(OF_PRIMITIVE_POINTS);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int i = y * width + x;
				float z = distancePixels[i];
				if(z != 0) { // ignore empty depth pixels
					ofVec3f cur = ConvertProjectiveToRealWorld(ofVec3f(x, y, z));
					cloud.addVertex(cur);
				}
			}
		}
		
		unsigned char* backgroundPixels = background.getPixels();
		int n = width * height;
		if(backgroundClear) {
			for(int i = 0; i < n; i++) {
				backgroundPixels[i] = 0;
			}
			background.update();
		}
		
		unsigned char* kinectPixels = kinect.getDepthPixels();
		if(backgroundCalibrate) {
			for(int i = 0; i < n; i++) {
				if(kinectPixels[i] > 0) {
					if(backgroundPixels[i] == 0) {
						backgroundPixels[i] = kinectPixels[i];
					}	else {
						backgroundPixels[i] = (backgroundPixels[i] + kinectPixels[i]) / 2;
					}
				}
			}
			background.update();
		}
		
		unsigned char* validPixels = valid.getPixels();
		for(int i = 0; i < n; i++) {
			int kinectPixel = kinectPixels[i];
			int backgroundPixel = backgroundPixels[i];
			bool far = abs(kinectPixel - backgroundPixel) > backgroundThreshold;
			if(kinectPixel < 255 && kinectPixel > 0 && (backgroundPixel == 0 || (backgroundPixel > 0 && far))) {
				validPixels[i] = 255;
			} else {
				validPixels[i] = 0;
			}
		}
		valid.update();
		
		trackedPositions.clear();
		for(int i = 0; i < circleFinder.size(); i++) {
			ofVec3f sum;
			int count = 0;
			const ofVec2f& center = circleFinder.getCenter(i);
			for(int y = -sampleRadius; y < +sampleRadius; y++) {
				for(int x = -sampleRadius; x < +sampleRadius; x++) {
					if(ofDist(0, 0, x, y) <= sampleRadius) {
						int curx = center.x + x;
						int cury = center.y + y;
						if(curx > 0 && curx < width &&
							 cury > 0 && cury < height) {
							int i = cury * width + curx;
							float curz = distancePixels[i];
							if(curz != 0) {
								sum.x += curx;
								sum.y += cury;
								sum.z += curz;
								count++;
							}
						}
					}
				}
			}
			trackedPositions.push_back(sum / count);
		}
	}
}
void CircleSensor::drawCloud() {
	ofPushStyle();
	ofSetColor(255);
	cloud.draw();
	ofFill();
	ofSetColor(cyanPrint);
	for(int i = 0; i < trackedPositions.size(); i++) {
		ofBox(ConvertProjectiveToRealWorld(trackedPositions[i]), 8);
	}
	ofPopStyle();
}
void CircleSensor::drawDebug() {
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(255);
	kinect.draw(0, 0);
	kinect.drawDepth(640, 0, 320, 240);
	background.draw(640, 240, 320, 240);
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(yellowPrint);
	valid.draw(0, 0);
	ofDisableBlendMode();
	
	ofNoFill();
	ofSetColor(magentaPrint);
	for(int i = 0; i < circleFinder.size(); i++) {
		float radius = circleFinder.getRadius(i);
		if(radius > 2) {
			ofCircle(circleFinder.getCenter(i), radius);
			ofDrawBitmapString(ofToString(ConvertProjectiveToRealWorld(trackedPositions[i])), circleFinder.getCenter(i));
		}
	}
	ofPopMatrix();
	ofPopStyle();
}
CircleSensor::~CircleSensor() {
	kinect.close();
}