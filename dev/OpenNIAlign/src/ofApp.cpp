#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

inline vector<ofVec3f> loadPoints(string filename) {
	Mat pointsMat;
	loadMat(pointsMat, filename);
	vector<ofVec3f> points(pointsMat.rows);
	for(int i = 0; i < pointsMat.rows; i++) {
		points[i] = toOf(pointsMat.at<Point3f>(i));
	}
	return points;
}
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	vector<ofVec3f> fromBuf = loadPoints("0.txt");
	vector<ofVec3f> toBuf = loadPoints("1.txt");
	int n = MIN(fromBuf.size(), toBuf.size());
	for(int i = 0; i < n; i++) {
		if(i == 0 || (fromBuf[i] != from.back() && toBuf[i] != to.back())) {
			from.push_back(fromBuf[i]);
			to.push_back(toBuf[i]);
		}
	}
	
	ofMatrix4x4 affine = estimateAffine3D(from, to);
	for(int i = 0; i < from.size(); i++) {
		matched.push_back(affine.preMult(from[i]));
	}
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofScale(.1, .1, .1);
	ofDrawGrid(2000, 10, false, false, true, false);
	ofMesh mesh;
	
	glPointSize(3);
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.addVertices(from);
	ofSetColor(255);
	mesh.draw();
	ofSetColor(ofColor::red);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.draw();
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.addVertices(to);
	ofSetColor(255);
	mesh.draw();
	ofSetColor(ofColor::red);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.draw();
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.addVertices(matched);
	ofSetColor(ofColor::green);
	mesh.draw();
	ofSetColor(ofColor::red);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.draw();
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINES);
	for(int i = 0; i < from.size(); i++) {
		mesh.addVertex(from[i]);
		mesh.addVertex(matched[i]);
	}
	ofSetColor(ofColor::blue);
	mesh.draw();
	
	int i = ofMap(mouseX, 0, ofGetWidth(), 0, MIN(from.size(), to.size()) - 1);
	ofVec3f a = from[i], b = from[i + 1];
	ofVec3f c = to[i], d = to[i + 1];
	ofVec3f ba = b - a;
	ofVec3f dc = d - c;
	ofLine(a, b + ba * 10);
	ofLine(c, d + dc * 10);
	
	cam.end();
}
