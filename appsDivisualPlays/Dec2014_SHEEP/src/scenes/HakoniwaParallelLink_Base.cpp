//
//  HakoniwaParallelLink_Base.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/20.
//
//

#include "HakoniwaParallelLink_Base.h"
ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

HakoniwaParallelLink_Base::HakoniwaParallelLink_Base(){

	mLinkManager.setup("cu.usbmodem1141");
	mLinkManager.setupOsc("192.168.20.56", 8528);
//	mLinkManager.setupUDP("192.168.20.56", 8528);

	mLinkManager.height = 250.0;

	mLinkManager.area_clamp.set(80.0, 43.67, 80.0);
	mLinkManager.area_offset.set(0.0, 150.0, 0.0);
	mLinkManager.delta.rotation = 30;
	mLinkManager.id_offset = 0;
	mLinkManager.id_swap = false;

	mOscSender = &mLinkManager.stepManager.sender;

	mSetting_Accel = 16;
	mSetting_Deccel = 16;
	mSetting_MaxSpeed = 37;
}

void HakoniwaParallelLink_Base::update(){

	ofVec3f v = mActor.getNode(ramActor::JOINT_RIGHT_HAND);
	ofVec3f chest = mActor.getNode(ramActor::JOINT_CHEST);

	if (mTrackMachine){
		machinePosition.x = chest.x;
		machinePosition.z = chest.z;
	}

	if (ofGetFrameNum() % 1 == 0){
		if (mLinkManager.stepManager.useOsc)
		{
			ofxOscMessage m;
			m.setAddress("/dp/hakoniwa/analogWrite/");
			m.addIntArg(3);
			m.addIntArg(mDigitalOut * mPwm_Param);
			mOscSender->sendMessage(m);
		}
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

	if (ofGetFrameNum() % (int)mLinkManager.signal_step == 0){
//		ofCircle(ofGetWidth()/2, ofGetHeight()/2, 300);
	}

}

void HakoniwaParallelLink_Base::setupControlPanel(){

	ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();

	//==============================   System GUI   ===========================//
	systemGui = new ofxUICanvas();
	systemGui->setAutoDraw(false);
	systemGui->disableMouseEventCallbacks();
	systemGui->addLabel("System");
	systemGui->addSpacer();
	systemGui->addSlider("arm1", 0.0, 200.0, &mLinkManager.armLength1);
	systemGui->addSlider("arm2", 0.0, 200.0, &mLinkManager.armLength2);
	systemGui->addSlider("radius", 0.0, 100.0, &mLinkManager.radius);
	systemGui->addSlider("height", 0.0, 300.0, &mLinkManager.height);
	systemGui->addSlider("plotRadius", 0.0, 100.0, &mLinkManager.plot_radius);
	systemGui->addSlider("signalStep", 1, 30, &mLinkManager.signal_step);

	systemGui->addLabel("Calibration");
	systemGui->addSpacer();

	systemGui->addToggle("ManualPosition", &ManualPose);
	systemGui->addToggle("TrackMachine", &mTrackMachine);
	systemGui->addToggle("CalibratePosition", &CalibratePose);
	systemGui->addButton("Calibration",false);
	systemGui->addToggle("Enable", &mLinkManager.enableSync);

	systemGui->addToggle("digitalIO", &mDigitalOut);
	systemGui->addSlider("pwmParam", 0.0, 255.0, &mPwm_Param);
	systemGui->setPosition(0, 30);
	systemGui->autoSizeToFitWidgets();


	//==============================   XYZ GUI   ===========================//
	xyzGui = new ofxUICanvas();
	xyzGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
						uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	xyzGui->setAutoDraw(false);
	xyzGui->disableMouseEventCallbacks();
	xyzGui->addLabel("Machine");
	xyzGui->addSlider("X", -200.0, 200.0, &machinePosition.x,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	xyzGui->addSlider("Y", -200.0, 200.0, &machinePosition.y,15,100);
	xyzGui->addSlider("Z", -200.0, 200.0, &machinePosition.z,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

	xyzGui->addLabel("Clamp");
	xyzGui->addSlider("X", 0.0, 200.0, &mLinkManager.area_clamp.x,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	xyzGui->addSlider("Y", 0.0, 200.0, &mLinkManager.area_clamp.y,15,100);
	xyzGui->addSlider("Z", 0.0, 200.0, &mLinkManager.area_clamp.z,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

	xyzGui->addLabel("Offset");
	xyzGui->addSlider("X", -200.0, 200.0, &mLinkManager.area_offset.x,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	xyzGui->addSlider("Y", -200.0, 200.0, &mLinkManager.area_offset.y,15,100);
	xyzGui->addSlider("Z", -200.0, 200.0, &mLinkManager.area_offset.z,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

	xyzGui->addLabel("Manual");
	xyzGui->addSlider("X", -200.0, 200.0, &mManualPosition.x,15,100);
	xyzGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	xyzGui->addSlider("Y", -200.0, 200.0, &mManualPosition.y,15,100);
	xyzGui->addSlider("Z", -200.0, 200.0, &mManualPosition.z,15,100);
	xyzGui->setPosition(240, 30);
	xyzGui->autoSizeToFitWidgets();


	//==============================   Settings GUI   ===========================//
	settingGui = new ofxUICanvas();
	settingGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
							uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	settingGui->setAutoDraw(false);
	settingGui->disableMouseEventCallbacks();
	settingGui->addLabel("Settings");
	settingGui->addSlider("Accel", 0.0, 500.0, &mSetting_Accel);
	settingGui->addSlider("Deccel", 0.0, 500.0, &mSetting_Deccel);
	settingGui->addSlider("MaxSPD", 0.0, 500.0, &mSetting_MaxSpeed);
	settingGui->addButton("SendSettings", false);
	settingGui->setPosition(0, 450);
	settingGui->autoSizeToFitWidgets();

	panel->addWidget(systemGui);
	panel->addWidget(xyzGui);
	panel->addWidget(settingGui);

	CalibratePose	= false;
	ManualPose		= false;

	ofAddListener(panel->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);
	ofAddListener(systemGui->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);
	ofAddListener(xyzGui->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);
	ofAddListener(settingGui->newGUIEvent,
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

	if (w->getName() == "SendSettings"){
		mLinkManager.stepManager.setStepperAll(true);
		mLinkManager.stepManager.setParam_Accel(mSetting_Accel);
		mLinkManager.stepManager.setParam_Decel(mSetting_Deccel);
		mLinkManager.stepManager.setParam_maxSpeed(mSetting_MaxSpeed);
		mLinkManager.stepManager.setStepperAll(false);
	}

}

void HakoniwaParallelLink_Base::onEnabled(){
//	xyzGui->setAutoDraw(true);
}

void HakoniwaParallelLink_Base::onDisabled(){
	xyzGui->setAutoDraw(false);

}