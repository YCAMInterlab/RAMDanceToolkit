#include "testApp.h"

bool showDebug = false;
bool backgroundClear = true;
bool backgroundCalibrate = false;
bool setDeadZones = false;
float backgroundThreshold = 5;

float blurRadius = 7;
float circleThreshold = 80;
float minRadius = 1;
float maxRadius = 12;
float sampleRadius = 1.2;

bool registrationClear = false;
bool registrationCalibrate = false;
float registrationCalibrationRate = 10;
float registrationCalibrationAccuracy = 1000;

bool bySerial(const ofPtr<CircleSensor>& a, const ofPtr<CircleSensor>& b) {
    return a->kinect.getSerial() < b->kinect.getSerial();
}

void testApp::setup() {
	ofSetVerticalSync(true);
    ofEnableSmoothing();
	
	int n = ofxKinect::numAvailableDevices();    
	while(sensors.size() < n) {
		sensors.push_back(ofPtr<CircleSensor>(new CircleSensor()));
		sensors.back()->setup();
	}
    ofSort(sensors, bySerial);
	
	float dim = 20;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 320 - xInit;
	gui = new ofxUICanvas(0, 0, length + xInit * 2, 2560);
	gui->setFont("/System/Library/Fonts/Geneva.dfont");
	ofColor cb(64, 192),
	co(192, 192),
	coh(128, 192),
	cf(240, 255),
	cfh(128, 255),
	cp(96, 192),
	cpo(255, 192);
	gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
	
    gui->addLabel("Kinects: " +  ofToString(ofxKinect::numConnectedDevices()) + " / " + ofToString(ofxKinect::numTotalDevices()), OFX_UI_FONT_SMALL);
    
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
	gui->addLabelButton("Set dead zones", &setDeadZones, length, dim);
	
	gui->addLabel("Registration", OFX_UI_FONT_LARGE);
	gui->addLabelButton("Clear", &registrationClear, length, dim);
	gui->addLabelToggle("Calibrate", &registrationCalibrate, length, dim);
	gui->addSlider("Calibration accuracy", 100, 10000, &registrationCalibrationAccuracy, length, dim);
}

void testApp::update() {
    string kinectDropdownStatus = "";
	for(int i = 0; i < sensors.size(); i++) {
		CircleSensor& sensor = *sensors[i];
        kinectDropdownStatus += "[" + (sensor.kinect.isConnected() ? ofToString(sensor.kinect.getSerial()) : "not connected") + "]";
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
        if(setDeadZones) {
            sensor.setDeadZones();
        }
	}
	
	if(registrationCalibrate && !sensors.empty() && sensors[0]->oneTrackedPosition()) {
        for(int i = 1; i < sensors.size(); i++) {
            if(sensors[i]->oneTrackedPosition()) {
                sensors[i]->updateRegistration(sensors[0]->trackedPositions[0], 1. - (1./registrationCalibrationAccuracy));
            }
		}
	}
}

void testApp::draw() {
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofBackgroundGradient(ofColor(64), ofColor(0));
	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	for(int i = 0; i < sensors.size(); i++) {
        ofSetColor(ofColor::fromHsb(((int) ofMap(i, 0, sensors.size(), 0, 255) + 40) % 256, 255, 255));
		sensors[i]->drawCloud();
	}
    ofFill();
    ofSphere(filtered, 10);
	easyCam.end();
	
	if(showDebug) {
		ofPushMatrix();
		ofTranslate(320, 0);
        float scale = ((float) ofGetHeight() / sensors.size()) / 480;
        ofScale(scale, scale);
		glDisable(GL_DEPTH_TEST);
		for(int i = 0; i < sensors.size(); i++) {
			sensors[i]->drawDebug();
			ofTranslate(0, 480);
		}
		ofPopMatrix();
	}
}

