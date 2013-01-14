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

ofMatrix4x4 estimateRigid3D(vector<ofVec3f>& from, vector<ofVec3f>& to) {
	Scalar translation = mean(Mat(to.size(), 1, CV_32FC3, &to[0])) - mean(Mat(from.size(), 1, CV_32FC3, &from[0]));
	ofMatrix4x4 mat;
	mat.translate(translation[0], translation[1], translation[2]);
	return mat;
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("port", 10001));
	settings.popTag();
	
	calibrating = false;
}

void ofApp::update() {
	while(osc.hasWaitingMessages()) {
		ofxOscMessage msg;
		osc.getNextMessage(&msg);
		if(checkAddress(msg, "openni", 0)) {
			int deviceID = ofToInt(getAddress(msg, 1));
			opennis[deviceID].update(msg, calibrating);
		}
	}
	if(calibrating) {
		map<int, OscOpenNI>::iterator itr = opennis.begin();
		vector<ofVec3f>& reference = itr->second.recentData;
		if(reference.size() > 4) {
			for(itr++; itr != opennis.end(); itr++) {
				OscOpenNI& cur = itr->second;
				vector<ofVec3f> from = cur.recentData;
				vector<ofVec3f> to = reference;
				int matchSize = MIN(from.size(), to.size());
				from.resize(matchSize);
				to.resize(matchSize);
				cur.registration = estimateAffine3D(from, to);
				cur.hasRegistration = true;
				cout << "registration for kinect " << itr->first << ": " << endl;
				cout << cur.registration << endl;
			}
		}
	}
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofScale(.1, .1, .1);
	ofDrawGrid(2000, 10, false, true, true, true);
	for(map<int, OscOpenNI>::iterator openniItr = opennis.begin(); openniItr != opennis.end(); openniItr++) {
		OscOpenNI& openni = openniItr->second;
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
		mesh.addVertices(openni.recentData);
		ofSetColor(ofColor::red);
		mesh.draw();
		if(openni.hasRegistration) {
			cout << "drawing registration for kinect " << openniItr->first << endl;
			for(int i = 0; i < mesh.getNumVertices(); i++) {
				mesh.getVertices()[i] = openni.registration.preMult(mesh.getVertices()[i]);
			}
			ofSetColor(ofColor::green);
			mesh.draw();
		}
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
		calibrating = !calibrating;
	}
	if(key == '\t') {
		for(map<int, OscOpenNI>::iterator itr = opennis.begin(); itr != opennis.end(); itr++) {
			savePoints(itr->second.recentData, ofToString(itr->first) + ".txt");
		}
	}
}