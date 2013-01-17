#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	
	int n = ofxKinect::numAvailableDevices();
	cout << "total devices: " << n << endl;
	while(sensors.size() < n) {
		sensors.push_back(ofPtr<CircleSensor>(new CircleSensor()));
		sensors.back()->setup();
	}
	
	gui.setup();
	gui.addPanel("Control");
	gui.addToggle("showDebug", true);
	gui.addToggle("backgroundClear", false);
	gui.addToggle("backgroundCalibrate", false);
	gui.addSlider("backgroundThreshold", 10, 0, 255, true);
	gui.addSlider("sampleRadius", 12, 0, 32);
	gui.addToggle("registrationClear", false);
	gui.addToggle("registrationCalibrate", false);
	gui.addSlider("registrationCalibrationRate", 2, 1, 10, true);
	
	gui.addPanel("CircleFinder");
	gui.addSlider("blurRadius", 5, 0, 11);
	gui.addSlider("circleThreshold", 192, 0, 255);
	gui.addSlider("minRadius", 2, 0, 12);
	gui.addSlider("maxRadius", 6, 0, 12);
}

void testApp::update() {
	registrationCalibrationTimer.setFramerate(gui.getValueI("registrationCalibrationRate"));
	bool registrationCalibrate = gui.getValueB("registrationCalibrate");
	
	for(int i = 0; i < sensors.size(); i++) {
		CircleSensor& sensor = *sensors[i];
		sensor.backgroundClear = gui.getValueB("backgroundClear");
		sensor.backgroundCalibrate = gui.getValueB("backgroundCalibrate");
		sensor.backgroundThreshold = gui.getValueI("backgroundThreshold");
		sensor.sampleRadius = gui.getValueF("sampleRadius");
		sensor.circleFinder.blurRadius = gui.getValueF("blurRadius");
		sensor.circleFinder.threshold = gui.getValueF("circleThreshold");
		sensor.circleFinder.minRadius = gui.getValueF("minRadius");
		sensor.circleFinder.maxRadius = gui.getValueF("maxRadius");
		sensor.update();
	}
	
	if(registrationCalibrate && registrationCalibrationTimer.tick()) {
		bool singleMatch = true;
		for(int i = 0; i < sensors.size(); i++) {
			if(sensors[i]->trackedPositions.size() != 1) {
				singleMatch = false;
				break;
			}
		}
		if(singleMatch) {
			for(int i = 0; i < sensors.size(); i++) {
				sensors[i]->sampleRegistration();
			}
			for(int i = 1; i < sensors.size(); i++) {
				sensors[i]->updateRegistration(*sensors[0]);
			}
		}
	}
	
	gui.setValueB("backgroundClear", false);
	gui.setValueB("registrationClear", false);
}

void testApp::draw() {
	ofBackground(0);
	
	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	for(int i = 0; i < sensors.size(); i++) {
		CircleSensor& sensor = *sensors[i];
		sensor.drawCloud();
	}
	easyCam.end();
	
	if(gui.getValueB("showDebug")) {
		ofPushMatrix();
		glDisable(GL_DEPTH_TEST);
		for(int i = 0; i < sensors.size(); i++) {
			CircleSensor& sensor = *sensors[i];
			sensor.drawDebug();
			ofTranslate(0, 480);
		}
		ofPopMatrix();
	}
}