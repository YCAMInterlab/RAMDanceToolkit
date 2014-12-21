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

	mLinkManager.area_clamp.set(80.0, 80.0, 80.0);
	mLinkManager.area_offset.set(0.0, 150.0, 0.0);
}

void HakoniwaParallelLink_Base::update(){

	ofVec3f v = mActor.getNode(ramActor::JOINT_LEFT_ELBOW);

	mLinkManager.update();

	if (CalibratePose)		mLinkManager.setPlot_inClamp(ofVec3f(0.0,100.0,0.0));
	else if (ManualPose)	mLinkManager.setPlot_inClamp(mManualPosition);
	else					mLinkManager.setPlot_inClamp(v);

}

void HakoniwaParallelLink_Base::draw(){
	ofVec3f v = mActor.getNode(ramActor::JOINT_LEFT_ELBOW);

	ramBeginCamera();
	glEnable(GL_DEPTH_TEST);
	mLinkManager.draw();
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

	cout << "Ev" << endl;

}