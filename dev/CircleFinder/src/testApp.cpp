#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	//ofSetVerticalSync(true);
		
	img.loadImage("simple.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
	
	int blurRadius = 0;
	blur(img, blurred, blurRadius);
	blurred.update();
}

void testApp::update() {
	int searchRadius = 12;
	unsigned char threshold = 192;
	
	centers.clear();
	radii.clear();
	
	// could also just do contour detection here
	unsigned char* pixels = img.getPixels();
	unsigned char* blurredPixels = blurred.getPixels();
	vector<ofVec2f> candidates;
	for(int y = searchRadius; y < img.getHeight() - searchRadius; y++) {
		for(int x = searchRadius; x < img.getWidth() - searchRadius; x++) {
			int i = y * img.getWidth() + x;
			if(blurredPixels[i] > threshold) {
				ofVec2f avg;
				float radius;
				float weightSum = 0;
				for(int yd = -searchRadius; yd < searchRadius; yd++) {
					for(int xd = -searchRadius; xd < searchRadius; xd++) {
						int j = (y + yd) * img.getWidth() + (x + xd);
						unsigned char brightness = pixels[j];
						ofVec2f position(x + xd, y + yd);
						avg += brightness * position;
						weightSum += pixels[j];
					}
				}
				avg /= weightSum;
				candidates.push_back(avg);
			}
		}
	}
	
	// collect multiple matches
	for(int i = 0; i < candidates.size(); i++) {
		ofVec2f& cur = candidates[i];
		bool exists = false;
		for(int j = 0; j < centers.size(); j++) {
			if(cur.distance(centers[j]) < searchRadius) {
				exists = true;
				break;
			}
		}
		if(!exists) {
			centers.push_back(cur);
		}
	}
	
	// this second half estimates the size of the circle
	float areaThreshold = 64;
	for(int i = 0; i < centers.size(); i++) {
		float weightSum = 0;
		ofVec2f positionSum;
		ofVec2f& avg = centers[i]; 
		int xa = avg.x, ya = avg.y;
		float area = 0;
		for(int yd = -searchRadius; yd < searchRadius; yd++) {
			for(int xd = -searchRadius; xd < searchRadius; xd++) {
				int j = (ya + yd) * img.getWidth() + (xa + xd);
				float brightness = (float) pixels[j];
				ofVec2f position(xa + xd, ya + yd);
				float radius = avg.distance(position);
				float weight = brightness;
				positionSum += weight * position;
				weightSum += weight;
				if(brightness > areaThreshold) {
					area += brightness;
				}
			}
		}
		radii.push_back(sqrtf(area / (PI * 255)));
		centers[i] = positionSum / weightSum;
	}
}

void testApp::draw() {
	int minRadius = 2, maxRadius = 6;
	ofSetColor(255);
	img.draw(0, 0);
	blurred.draw(640, 0);
	ofSetColor(magentaPrint);
	ofNoFill();
	ofTranslate(.5, .5);
	for(int i = 0; i < centers.size(); i++) {
		if(radii[i] > minRadius && radii[i] < maxRadius) {
			ofCircle(centers[i], radii[i]);
			ofVec2f pos = centers[i] + ofVec2f(10, 20);
			pos.x = 10 * (int) (pos.x / 10);
			pos.y = 10 * (int) (pos.y / 10);
			ofDrawBitmapString(ofToString(radii[i]), pos.x, pos.y);
		}
	}
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}
