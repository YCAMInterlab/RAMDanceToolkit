#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	//ofSetVerticalSync(true);
	
	img.loadImage("emissive.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	circleFinder.threshold = ofMap(mouseX, 0, ofGetWidth(), 64, 255, true);
	circleFinder.update(img);
}

void testApp::draw() {
	ofSetColor(255);
	img.draw(0, 0);
	ofSetColor(magentaPrint);
	circleFinder.contourFinder.draw();
	ofNoFill();
	ofTranslate(.5, .5);
	for(int i = 0; i < circleFinder.size(); i++) {
		float radius = circleFinder.getRadius(i);
		const ofVec2f& center = circleFinder.getCenter(i);
		ofCircle(center, radius);
		ofVec2f pos = center + ofVec2f(10, 20);
		pos.x = 10 * (int) (pos.x / 10);
		pos.y = 10 * (int) (pos.y / 10);
		ofDrawBitmapString(ofToString(radius), pos.x, pos.y);
	}
	ofSetWindowTitle(ofToString((int) ofGetFrameRate()));
	ofDrawBitmapString(ofToString((int) circleFinder.threshold), 10, 20);
}
