#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);
	openNIDevice.setDeviceID(0);
	openNIDevice.setup();
	openNIDevice.addDepthGenerator();
	openNIDevice.setRegister(false);
	openNIDevice.setMirror(false);
	openNIDevice.addUserGenerator();
	openNIDevice.setMaxNumUsers(2);
	openNIDevice.start();
}

void ofApp::update(){
	openNIDevice.update();
}

void ofApp::draw() {
	ofSetColor(255);
	openNIDevice.drawDepth();
	for(int i = 0; i < openNIDevice.getNumTrackedUsers(); i++) {
		openNIDevice.getTrackedUser(i).drawSkeleton();
	}
}

void ofApp::userEvent(ofxOpenNIUserEvent & event) {
	ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

void ofApp::exit() {
	openNIDevice.stop();
}
