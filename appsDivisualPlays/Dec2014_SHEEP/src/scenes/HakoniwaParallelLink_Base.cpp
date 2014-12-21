//
//  HakoniwaParallelLink_Base.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/20.
//
//

#include "HakoniwaParallelLink_Base.h"

HakoniwaParallelLink_Base::HakoniwaParallelLink_Base(){

	mLinkManager.setup("cu.usbmodem1141");
	mLinkManager.setupOsc("192.168.20.37", 12345);

	mLinkManager.height = 250.0;

	mLinkManager.area_clamp.set(80.0, 43.67, 80.0);
	mLinkManager.area_offset.set(0.0, 150.0, 0.0);
	mLinkManager.delta.rotation = 30;
	mLinkManager.id_offset = 0;
	mLinkManager.id_swap = false;

	mOscSender = &mLinkManager.stepManager.sender;
}

void HakoniwaParallelLink_Base::update(){

	ofVec3f v = mActor.getNode(ramActor::JOINT_RIGHT_HAND);


	if (ofGetFrameNum() % 3 == 0){
		ofxOscMessage m;
		m.setAddress("/dp/hakoniwa/digitalWrite/");
		m.addIntArg(3);
		m.addIntArg(mDigitalOut && ofGetFrameNum() % 6 == 0);
		mOscSender->sendMessage(m);
	}

	mLinkManager.update();

	if (CalibratePose)		mLinkManager.setPlot_inClamp(ofVec3f(0.0,100.0,0.0));
	else if (ManualPose)	mLinkManager.setPlot_inClamp(mManualPosition);
	else					mLinkManager.setPlot_inClamp(v - machinePosition);

}

void HakoniwaParallelLink_Base::draw(){
	ofVec3f v = mActor.getNode(ramActor::JOINT_LEFT_ELBOW);

	ramBeginCamera();
	glEnable(GL_DEPTH_TEST);
	ofPushMatrix();
	ofTranslate(machinePosition);
	mLinkManager.draw();
	ofPopMatrix();
	glDisable(GL_DEPTH_TEST);
	ramEndCamera();

}

void HakoniwaParallelLink_Base::setupControlPanel(){

	ramGetGUI().addLabel("System");
	ramGetGUI().getCurrentUIContext()->addSpacer();
	ramGetGUI().addSlider("arm1", 0.0, 200.0, &mLinkManager.armLength1);
	ramGetGUI().addSlider("arm2", 0.0, 200.0, &mLinkManager.armLength2);
	ramGetGUI().addSlider("radius", 0.0, 100.0, &mLinkManager.radius);
	ramGetGUI().addSlider("height", 0.0, 300.0, &mLinkManager.height);
	ramGetGUI().addSlider("plotRadius", 0.0, 100.0, &mLinkManager.plot_radius);

	ramGetGUI().addLabel("Calibration");
	ramGetGUI().getCurrentUIContext()->addSpacer();
	ramGetGUI().addSlider("Machine_X", -200.0, 200.0, &machinePosition.x);
	ramGetGUI().addSlider("Machine_Y", -200.0, 200.0, &machinePosition.y);
	ramGetGUI().addSlider("Machine_Z", -200.0, 200.0, &machinePosition.z);
	ramGetGUI().addSlider("Clamp_X", 0.0, 200.0, &mLinkManager.area_clamp.x);
	ramGetGUI().addSlider("Clamp_Y", 0.0, 200.0, &mLinkManager.area_clamp.y);
	ramGetGUI().addSlider("Clamp_Z", 0.0, 200.0, &mLinkManager.area_clamp.z);
	ramGetGUI().addSlider("OffsetX", -200.0, 200.0, &mLinkManager.area_offset.x);
	ramGetGUI().addSlider("OffsetY", -200.0, 200.0, &mLinkManager.area_offset.y);
	ramGetGUI().addSlider("OffsetZ", -200.0, 200.0, &mLinkManager.area_offset.z);

	ramGetGUI().addSlider("ManualX", -200.0, 200.0, &mManualPosition.x);
	ramGetGUI().addSlider("ManualY", -200.0, 200.0, &mManualPosition.y);
	ramGetGUI().addSlider("ManualZ", -200.0, 200.0, &mManualPosition.z);

	CalibratePose	= false;
	ManualPose		= false;

	ramGetGUI().addToggle("ManualPosition", &ManualPose);
	ramGetGUI().addToggle("CalibratePosition", &CalibratePose);
	ramGetGUI().addButton("Calibration");
	ramGetGUI().addToggle("Enable", &mLinkManager.enableSync);

	ramGetGUI().addToggle("digitalIO", &mDigitalOut);
	ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);

}

void HakoniwaParallelLink_Base::drawActor(const ramActor &actor)
{
	mActor = actor;
}

void HakoniwaParallelLink_Base::onPanelChanged(ofxUIEventArgs& e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "Calibration"){
		mLinkManager.calibrate();
	}

}