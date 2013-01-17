#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

inline void savePoints(vector<ofVec3f>& points, string filename) {
	Mat pointsMat(points.size(), 1, CV_32FC3, &points[0]);
	saveMat(pointsMat, filename);
}

inline vector<ofVec3f> loadPoints(string filename) {
	Mat pointsMat;
	loadMat(pointsMat, filename);
	vector<ofVec3f> points(pointsMat.rows);
	for(int i = 0; i < pointsMat.rows; i++) {
		points[i] = toOf(pointsMat.at<Point3f>(i));
	}
	return points;
}

string formatPosition(ofVec3f point) {
	return ofToString((int) point.x) + "/" + ofToString((int) point.y) + "/" + ofToString((int) point.z);
}

ofQuaternion findRotation(ofVec3f up1, ofVec3f right1, ofVec3f up2, ofVec3f right2) {
	ofQuaternion rot1, rot2;
	rot1.makeRotate(up1, up2);
	ofVec3f remainder = rot1 * right1;
	rot2.makeRotate(remainder, right2);
	return rot1;// + rot2;
}

void ofApp::addPanel(int deviceID) {
	string did = ofToString(deviceID);
	gui.addPanel("Kinect" + did);
	gui.addSlider("tx" + did, 0, -2000, 2000);
	gui.addSlider("ty" + did, 0, -2000, 2000);
	gui.addSlider("tz" + did, 0, -2000, 2000);
	gui.addSlider("ux" + did, 0, -1, 1);
	gui.addSlider("uy" + did, 0, -1, 1);
	gui.addSlider("uz" + did, 1, -1, 1);
	gui.addSlider("rot" + did, 0, -PI, PI);
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("port", 10001));
	settings.popTag();
	
	gui.setup();
	gui.addPanel("Settings");
	gui.addToggle("translationEstimate");
	gui.addToggle("completeEstimate"); 
}

void ofApp::update() {
	while(osc.hasWaitingMessages()) {
		ofxOscMessage msg;
		osc.getNextMessage(&msg);
		if(checkAddress(msg, "openni", 0)) {
			int deviceID = ofToInt(getAddress(msg, 1));
			if(opennis.find(deviceID) == opennis.end()) {
				addPanel(deviceID);
			}
			opennis[deviceID].update(msg);
		}
	}	
	
	bool translationEstimate = gui.getValueB("translationEstimate");
	bool completeEstimate = gui.getValueB("completeEstimate");
	gui.setValueB("translationEstimate", false);
	gui.setValueB("completeEstimate", false);
	if(translationEstimate || completeEstimate) {
		if(!opennis.empty()) {
			map<int, OscOpenNI>::iterator itr = opennis.begin();
			OscOpenNI& curOpenNI = itr->second;
			OscUser* referenceUser = curOpenNI.recentUser;
			for(itr++; itr != opennis.end(); itr++) {
				OscUser* curUser = itr->second.recentUser;
				if(curUser != NULL) {
					curOpenNI.registration = ofMatrix4x4();
					ofVec3f referenceJoint = referenceUser->joints[JOINT_NECK].position;
					ofVec3f curJoint = curUser->joints[JOINT_NECK].position;
					// this is broken right now
					if(completeEstimate) {
						ofVec3f up1 = curUser->joints[JOINT_HEAD].position;
						ofVec3f right1 = curUser->joints[JOINT_RIGHT_SHOULDER].position;
						ofVec3f up2 = referenceUser->joints[JOINT_HEAD].position;
						ofVec3f right2 = referenceUser->joints[JOINT_RIGHT_SHOULDER].position;
						ofQuaternion rotation = findRotation(up1 - curJoint, right1 - curJoint,
																								 up2 - referenceJoint, right2 - referenceJoint);
						curOpenNI.registration.glRotate(rotation);
					}
					curOpenNI.registration.glTranslate(curJoint - referenceJoint);
					curOpenNI.hasRegistration = true;
				}
			}
		}
	}
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofScale(.15, .15, .15);
	ofDrawGrid(2000, 10, false, false, true, false);
	for(map<int, OscOpenNI>::iterator openniItr = opennis.begin(); openniItr != opennis.end(); openniItr++) {
		OscOpenNI& openni = openniItr->second;
		for(map<int, OscUser>::iterator userItr = openni.users.begin(); userItr != openni.users.end(); userItr++) {
			OscUser& user = userItr->second;
			ofSetColor(255);
			user.drawSkeleton();
			if(openni.hasRegistration) {
				ofSetColor(ofColor::green);
				user.drawSkeleton(openni.registration);
			}
		}
	}
	cam.end();
}

void ofApp::keyPressed(int key) {
	if(key == '\t') {
		for(map<int, OscOpenNI>::iterator itr = opennis.begin(); itr != opennis.end(); itr++) {
			//savePoints(itr->second.recentData, ofToString(itr->first) + ".txt");
		}
	}
}