//
//  ParallelLinkManager.cpp
//  motorTester
//
//  Created by Ovis aries on 2014/11/19.
//
//

#include "ParallelLinkManager.h"

void ParallelLinkManager::setup(string serial){

	stepManager.addStepper("unit1", 200, 0);
	stepManager.addStepper("unit2", 200, 1);
	stepManager.addStepper("unit3", 200, 2);

//	stepManager.setup(serial, 57600);

	stepManager.resetAllDevices();
	stepManager.setupEasyFromPreset(KSMR_STEP_P_PMSA_B56D5);

	area_clamp.set(30.0, 50.0, 30.0);
	area_offset.set(0, 0.0, 0);

	radius		=  39.0;
	armLength1	= 100.0;
	armLength2	= 100.0;
	height		= 100.0;
	plot_radius =  15.0;

	enableSync = false;
	id_offset = 0;
	id_swap = true;

	signal_step = 2;

}

void ParallelLinkManager::update(){

	//idling
	if (ofGetFrameNum() < 60){
		stepManager.setStepperAll(true);
		stepManager.absPos(0);
		stepManager.setStepperAll(false);
	}

	if (enableSync && (ofGetFrameNum() % (int)signal_step == 0)){
		sendSignal();
	}

	delta.setup(radius, armLength1*2, armLength2*2, height, plot_radius);
	
}

void ParallelLinkManager::sendSignal(){
	int ps[3];
	for (int i = 0;i < 3;i++){
		ps[i] = -delta.actuator[(i+id_offset)%3].getGlobalOrientation().getEuler().x/2;
	}

	if (id_swap) swap(ps[1], ps[2]);

	stepManager.setStepperAll(true);
	stepManager.multi_go_to(ps);
	stepManager.setStepperAll(false);
}

void ParallelLinkManager::draw(){

	delta.draw();

	ofSetColor(255, 255, 0);
	ofNoFill();
	ofDrawBox(area_offset, area_clamp.x,area_clamp.y,area_clamp.z);
	ofFill();
	ofSetColor(255);

	for (int i = 0;i < 3;i++){
		float ang = delta.actuator[i].getGlobalOrientation().getEuler().x;
		ofDrawBitmapString(ofToString(ang), delta.actuator[i].getGlobalPosition() + ofVec2f(0.0,20.0));
	}
};

void ParallelLinkManager::setPlot_inClamp(ofVec3f p){

	p.x = ofClamp(p.x, -area_clamp.x/2 + area_offset.x, area_clamp.x/2 + area_offset.x);
	p.y = ofClamp(p.y, -area_clamp.y/2 + area_offset.y, area_clamp.y/2 + area_offset.y);
	p.z = ofClamp(p.z, -area_clamp.z/2 + area_offset.z, area_clamp.z/2 + area_offset.z);
	delta.setPlot(p);

}

void ParallelLinkManager::calibrate(){

	stepManager.setupEasyFromPreset(KSMR_STEP_P_PMSA_B56D5);
	stepManager.setStepperAll(false);

	ofSleepMillis(300);
	int ps[3];
	for (int i = 0;i < 3;i++){
		ps[i] = -delta.actuator[(i+id_offset)%3].getGlobalOrientation().getEuler().x/2;
	}
	if (id_swap) swap(ps[1], ps[2]);

	ofSleepMillis(300);
	for (int i = 0;i < 3;i++){
		stepManager.setStepperSingle(i, true);
		stepManager.absPos(ps[i]);
		stepManager.setStepperSingle(i, false);
	}

	stepManager.setStepperAll(true);
	stepManager.multi_go_to(ps);
	stepManager.setStepperAll(false);

}