#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	//ofSetVerticalSync(true);
		
	img.loadImage("ito.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	circleFinder.update(img);
}

void testApp::draw() {
	int minRadius = 2, maxRadius = 6;
	ofSetColor(255);
	img.draw(0, 0);
	ofSetColor(magentaPrint);
	ofNoFill();
	ofTranslate(.5, .5);
	for(int i = 0; i < circleFinder.size(); i++) {
		float radius = circleFinder.getRadius(i);
		const ofVec2f& center = circleFinder.getCenter(i);
		if(radius > minRadius && radius < maxRadius) {
			ofCircle(center, radius);
			ofVec2f pos = center + ofVec2f(10, 20);
			pos.x = 10 * (int) (pos.x / 10);
			pos.y = 10 * (int) (pos.y / 10);
			ofDrawBitmapString(ofToString(radius), pos.x, pos.y);
		}
	}
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}
