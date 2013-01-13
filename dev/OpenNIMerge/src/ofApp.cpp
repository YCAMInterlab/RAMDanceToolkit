#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

bool OscOpenNI::calibrating = false;

string formatPosition(ofVec3f point) {
	return ofToString((int) point.x) + "/" + ofToString((int) point.y) + "/" + ofToString((int) point.z);
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("port", 10001));
	settings.popTag();
}

void ofApp::update() {
	while(osc.hasWaitingMessages()) {
		ofxOscMessage msg;
		osc.getNextMessage(&msg);
		if(checkAddress(msg, "openni", 0)) {
			int deviceID = ofToInt(getAddress(msg, 1));
			opennis[deviceID].update(msg);
		}
	}
	if(OscOpenNI::calibrating) {
		map<int, OscOpenNI>::iterator itr = opennis.begin();
		vector<ofVec3f>& reference = itr->second.recentData;
		if(reference.size() > 4) {
			for(itr++; itr != opennis.end(); itr++) {
				OscOpenNI& cur = itr->second;
				registration[itr->first] = estimateAffine3D(cur.recentData, reference);
				cout << "registration for kinect " << itr->first << ": " << endl;
				cout << registration[itr->first] << endl;
			}
		}
	}
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofScale(.05, .05, .05);
	ofDrawGrid(2000, 10, false, true, true, true);
	for(map<int, OscOpenNI>::iterator openniItr = opennis.begin(); openniItr != opennis.end(); openniItr++) {
		OscOpenNI& openni = openniItr->second;
		for(map<int, OscUser>::iterator userItr = openni.users.begin(); userItr != openni.users.end(); userItr++) {
			OscUser& user = userItr->second;
			ofSetColor(255);
			user.drawSkeleton();
			for(map<int, OscJoint>::iterator jointItr = user.joints.begin(); jointItr != user.joints.end(); jointItr++) {
				OscJoint& joint = jointItr->second;
				ofSetColor(ofColor::fromHsb(255 * joint.confidence, 255, 255));
				ofBox(joint.position, 10);
				ofSetColor(255);
				//ofDrawBitmapString(formatPosition(joint.position), joint.position);
			}
		}
	}
	cam.end();
}

void ofApp::keyPressed(int key) {
	if(key == ' ') {
		OscOpenNI::calibrating = !OscOpenNI::calibrating;
	}
}