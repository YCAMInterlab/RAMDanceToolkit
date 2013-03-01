#include "testApp.h"

bool bySerial(const ofPtr<ofxKinect>& a, const ofPtr<ofxKinect>& b) {
    return a->getSerial() < b->getSerial();
}

bool byDeviceId(const ofPtr<ofxKinect>& a, const ofPtr<ofxKinect>& b) {
    return a->getDeviceId() < b->getDeviceId();
}

void testApp::setup() {
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
	int n = ofxKinect::numAvailableDevices();
	while(kinects.size() < n) {
		kinects.push_back(ofPtr<ofxKinect>(new ofxKinect()));
		kinects.back()->init(true);
        kinects.back()->open();
	}
}

void testApp::update() {
    for(int i = 0; i < kinects.size(); i++) {
        kinects[i]->update();
    }
}

void testApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    ofPushMatrix();
    float scale = ((float) ofGetHeight() / kinects.size()) / 480;
    ofScale(scale, scale);
    for(int i = 0; i < kinects.size(); i++) {
        kinects[i]->drawDepth(0, 0);
        kinects[i]->draw(640, 0);
        ofTranslate(0, 480);
    }
    ofPopMatrix();
    for(int i = 0; i < kinects.size(); i++) {
        ofDrawBitmapString(ofToString(kinects[i]->getDeviceId()) + ":" + kinects[i]->getSerial(),
                           10, 20 + i * 20);
    }
}

void testApp::keyPressed(int key) {
    if(key == 's') {
        ofSort(kinects, bySerial);        
    }
    if(key == 'i') {
        ofSort(kinects, byDeviceId);
    }
}

void testApp::exit() {
    for(int i = 0; i < kinects.size(); i++) {
        kinects[i]->close();
    }
}