#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
								 (y / Yres - .5f) * z * YtoZ,
								 z);
}

void testApp::setup() {
	ofSetVerticalSync(true);
	kinect.init(true, true); // use infrared
	kinect.open();
}

void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		circleFinder.update(kinect);
		
		cloud.clear();
		int width = kinect.getWidth();
		int height = kinect.getHeight();
		float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
		cloud.setMode(OF_PRIMITIVE_POINTS);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int i = y * width + x;
				float z = distancePixels[i];
				if(z != 0) { // ignore empty depth pixels
					ofVec3f cur = ConvertProjectiveToRealWorld(x, y, z);
					cloud.addVertex(cur);
				}
			}
		}
	}
}

void testApp::draw() {
	ofBackground(0);
		
	easyCam.begin();	
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	ofSetColor(255);
	cloud.draw();
	easyCam.end();
	
	ofSetColor(255);
	kinect.drawDepth(0, 0);
	kinect.draw(640, 0);
	ofNoFill();
	ofSetColor(magentaPrint);
	for(int i = 0; i < circleFinder.size(); i++) {
		float radius = circleFinder.getRadius(i);
		if(radius > 2) {
			ofCircle(circleFinder.getCenter(i), radius);
		}
	}
}

void testApp::exit() {
	kinect.close();
}