#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");

	ofSetLogLevel(OF_LOG_VERBOSE);
	settings.pushTag("openNI");
	openNI.setDeviceID(settings.getValue("deviceID", 0));
	openNI.setup();
	openNI.addDepthGenerator();
	openNI.setRegister(false);
	openNI.setMirror(false);
	openNI.addUserGenerator();
	openNI.setMaxNumUsers(settings.getValue("maxNumUsers", 2));
	openNI.start();
	settings.popTag();
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("hostname", "localhost"), settings.getValue("port", 10001));
	settings.popTag();
}

void ofApp::update(){
	openNI.update();
	if(openNI.isNewFrame()) {
		ofxOscBundle bundle;
		string baseAddress = "/openni/" + ofToString(openNI.getDeviceID());
		ofxOscMessage numTrackedUsersMsg;
		numTrackedUsersMsg.setAddress(baseAddress + "/numTrackedUsers");
		int numTrackedUsers = openNI.getNumTrackedUsers();
		numTrackedUsersMsg.addIntArg(numTrackedUsers);
		bundle.addMessage(numTrackedUsersMsg);
		for(int i = 0; i < numTrackedUsers; i++) {
			ofxOpenNIUser& user = openNI.getTrackedUser(i);
			ofxOscMessage userMsg;
			userMsg.setAddress(baseAddress + "/user/" + ofToString(user.getXnID()));
			userMsg.addIntArg(user.isFound());
			userMsg.addIntArg(user.isTracking());
			userMsg.addIntArg(user.isSkeleton());
			userMsg.addIntArg(user.isCalibrating());
			bundle.addMessage(userMsg);
			ofxOscMessage jointMsg;
			jointMsg.setAddress(baseAddress + "/user/" + ofToString(user.getXnID()) + "/joints");
			int numJoints = user.getNumJoints();
			for(int j = 0; j < numJoints; j++) {
				ofxOpenNIJoint& joint = user.getJoint((Joint) j);
				ofPoint& worldPosition = joint.getWorldPosition();
				jointMsg.addFloatArg(worldPosition.x);
				jointMsg.addFloatArg(worldPosition.y);
				jointMsg.addFloatArg(worldPosition.z);
				jointMsg.addFloatArg(joint.getPositionConfidence());
			}
			bundle.addMessage(jointMsg);
		}
		osc.sendBundle(bundle);
	}
}

void ofApp::draw() {
	ofSetColor(255);
	openNI.drawDepth();
	for(int i = 0; i < openNI.getNumTrackedUsers(); i++) {
		openNI.getTrackedUser(i).drawSkeleton();
	}
}

void ofApp::userEvent(ofxOpenNIUserEvent & event) {
	ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

void ofApp::exit() {
	openNI.stop();
}
