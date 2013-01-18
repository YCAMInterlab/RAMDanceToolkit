#include "testApp.h"

bool showDebug = false;
bool backgroundClear = false;
bool backgroundCalibrate = false;
float backgroundThreshold = 5;

float blurRadius = 2;
float circleThreshold = 160;
float minRadius = 2;
float maxRadius = 10;
float sampleRadius = 12;

bool registrationClear = false;
bool registrationCalibrate = false;
float registrationCalibrationRate = 8;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	int n = ofxKinect::numAvailableDevices();
	cout << "total devices: " << n << endl;
	while(sensors.size() < n) {
		sensors.push_back(ofPtr<CircleSensor>(new CircleSensor()));
		sensors.back()->setup();
	}
	
	float dim = 20;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 320 - xInit;
	gui = new ofxUICanvas(0, 0, length + xInit * 2, ofGetHeight());
	gui->setFont("/System/Library/Fonts/HelveticaLight.ttf");
	ofColor cb(64, 192),
	co(192, 192),
	coh(128, 192),
	cf(240, 255),
	cfh(128, 255),
	cp(96, 192),
	cpo(255, 192);
	gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
	
	gui->addLabel("Background", OFX_UI_FONT_LARGE);
	gui->addLabelToggle("Debug", &showDebug, length, dim);
	gui->addLabelButton("Clear", &backgroundClear, length, dim);
	gui->addLabelToggle("Calibrate", &backgroundCalibrate, length, dim);
	gui->addSlider("Threshold", 0, 255, &backgroundThreshold, length, dim);
	
	gui->addLabel("Tracking", OFX_UI_FONT_LARGE);
	gui->addSlider("Blur radius", 0, 11, &blurRadius, length, dim);
	gui->addSlider("Threshold", 0, 255, &circleThreshold, length, dim);
	gui->addSlider("Min radius", 0, 12, &minRadius, length, dim);
	gui->addSlider("Max radius", 0, 12, &maxRadius, length, dim);
	gui->addSlider("Sample radius", 0, 24, &sampleRadius, length, dim);
	
	gui->addLabel("Registration", OFX_UI_FONT_LARGE);
	gui->addLabelButton("Clear", &registrationClear, length, dim);
	gui->addLabelToggle("Calibrate", &registrationCalibrate, length, dim);
	gui->addSlider("Calibration rate", 1, 15, &registrationCalibrationRate, length, dim);
}

void testApp::update() {
	registrationCalibrationTimer.setFramerate(registrationCalibrationRate);
	
	for(int i = 0; i < sensors.size(); i++) {
		CircleSensor& sensor = *sensors[i];
		if(backgroundClear) {
			sensor.backgroundClear = true;
		}
		if(registrationClear) {
			sensor.registrationClear = true;
		}
		sensor.backgroundCalibrate = backgroundCalibrate;
		sensor.backgroundThreshold = backgroundThreshold;
		sensor.sampleRadius = sampleRadius;
		sensor.circleFinder.blurRadius = blurRadius;
		sensor.circleFinder.threshold = circleThreshold;
		sensor.circleFinder.minRadius = minRadius;
		sensor.circleFinder.maxRadius = maxRadius;
		sensor.update();
	}
	
	if(registrationCalibrate && registrationCalibrationTimer.tick()) {
		if(!sensors.empty()) {
			if(sensors[0]->oneTrackedPosition()) {
				for(int i = 1; i < sensors.size(); i++) {
					if(sensors[i]->oneTrackedPosition()) {
						sensors[i]->updateRegistration(sensors[0]->trackedPositions[0]);
					}
				}
			}
		}
	}
}

void testApp::draw() {
	ofBackgroundGradient(ofColor(64), ofColor(0));
	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	for(int i = 0; i < sensors.size(); i++) {
		CircleSensor& sensor = *sensors[i];
		sensor.drawCloud();
	}
	easyCam.end();
	
	if(showDebug) {
		ofPushMatrix();
		ofTranslate(320, 0);
		glDisable(GL_DEPTH_TEST);
		for(int i = 0; i < sensors.size(); i++) {
			CircleSensor& sensor = *sensors[i];
			sensor.drawDebug();
			ofTranslate(0, 480);
		}
		ofPopMatrix();
	}
}

