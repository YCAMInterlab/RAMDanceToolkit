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


	mLinkManager.height = 250.0;

	mLinkManager.area_clamp.set(50.0, 100.0, 50.0);
	mLinkManager.area_offset.set(0.0, 144.0, 0.0);
	mLinkManager.delta.rotation = 30;
	mLinkManager.id_offset = 0;
	mLinkManager.id_swap = false;

	mOscSender = &mLinkManager.stepManager.sender;
	mLinkManager.setupOsc("192.168.20.69", 8528);

	mSetting_Accel = 16;
	mSetting_Deccel = 16;
	mSetting_MaxSpeed = 37;
	mLinkManager.signal_step = 5;

	mTrackMachine = true;

}

void HakoniwaParallelLink_Base::update(){
	motionEx.update();

	ofVec3f v = motionEx.getNodeAt(1);
	ofVec3f chest = motionEx.getNodeAt(0);

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

	if (CalibratePose)		mLinkManager.setPlot_inClamp(ofVec3f(0.0,196.0,0.0));
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

	if (mDrawExtractor) motionEx.draw();

	glDisable(GL_DEPTH_TEST);
	ramEndCamera();

	if (ofGetFrameNum() % (int)mLinkManager.signal_step == 0){
//		ofCircle(ofGetWidth()/2, ofGetHeight()/2, 300);
	}

}

void HakoniwaParallelLink_Base::setupControlPanel(){

	ofxUICanvasPlus* panel = ramGetGUI().getCurrentUIContext();

	//==============================   System GUI   ===========================//
	systemGui = new ofxUICanvas();
	systemGui->disableAppDrawCallback();
	systemGui->disableMouseEventCallbacks();
	systemGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
						uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);

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
	systemGui->setPosition(5, 30);
	systemGui->autoSizeToFitWidgets();


	//==============================   XYZ GUI   ===========================//
	xyzGui = new ofxUICanvas();
	xyzGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
						uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);

	xyzGui->disableAppDrawCallback();
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
	xyzGui->setPosition(480, 30);
	xyzGui->autoSizeToFitWidgets();


	//==============================   Settings GUI   ===========================//
	settingGui = new ofxUICanvas();
	settingGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
							uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	settingGui->disableAppDrawCallback();
	settingGui->disableMouseEventCallbacks();
	settingGui->addLabel("Settings");
	settingGui->addButton("A_Up", false);
	settingGui->addButton("A_Dn", false);
	settingGui->addButton("B_Up", false);
	settingGui->addButton("B_Dn", false);
	settingGui->addButton("C_Up", false);
	settingGui->addButton("C_Dn", false);
	settingGui->addSlider("Accel", 0.0, 500.0, &mSetting_Accel);
	settingGui->addSlider("Deccel", 0.0, 500.0, &mSetting_Deccel);
	settingGui->addSlider("MaxSPD", 0.0, 500.0, &mSetting_MaxSpeed);
	settingGui->addButton("SendSettings", false);
	settingGui->setPosition(5, 460);
	settingGui->autoSizeToFitWidgets();

	utilityGui = new ofxUICanvas();
	utilityGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
							uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	utilityGui->disableAppDrawCallback();
	utilityGui->disableMouseEventCallbacks();
	utilityGui->addLabel("Utility");
	utilityGui->addTextInput("OSCAddress", "192.168.20.55");
	utilityGui->addToggle("drawExtractor", &mDrawExtractor);
	utilityGui->setPosition(240, 450);
	utilityGui->autoSizeToFitWidgets();

	panel->addWidget(systemGui);
	panel->addWidget(settingGui);
	panel->addWidget(xyzGui);
	panel->addWidget(utilityGui);

	panel->autoSizeToFitWidgets();

	CalibratePose	= false;
	ManualPose		= false;

	ofAddListener(panel->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);

	motionEx.setupControlPanel(this);
}

void HakoniwaParallelLink_Base::drawActor(const ramActor &actor)
{
	mActor = actor;
}

void HakoniwaParallelLink_Base::onPanelChanged(ofxUIEventArgs& e){
	ofxUIWidget* w = e.widget;

	if ((w->getName() == "A_Up") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[0].getGlobalOrientation().getEuler().x/2+4;
		mLinkManager.stepManager.setStepperSingle(0, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}
	if ((w->getName() == "A_Dn") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[0].getGlobalOrientation().getEuler().x/2-4;
		mLinkManager.stepManager.setStepperSingle(0, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}
	if ((w->getName() == "B_Up") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[1].getGlobalOrientation().getEuler().x/2+4;
		mLinkManager.stepManager.setStepperSingle(1, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}
	if ((w->getName() == "B_Dn") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[1].getGlobalOrientation().getEuler().x/2-4;
		mLinkManager.stepManager.setStepperSingle(1, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}
	if ((w->getName() == "C_Up") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[2].getGlobalOrientation().getEuler().x/2+4;
		mLinkManager.stepManager.setStepperSingle(2, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}
	if ((w->getName() == "C_Dn") && (w->getState() == OFX_UI_STATE_DOWN)){
		int tg = mLinkManager.delta.actuator[2].getGlobalOrientation().getEuler().x/2-4;
		mLinkManager.stepManager.setStepperSingle(2, true);
		mLinkManager.stepManager.absPos(tg);
		mLinkManager.stepManager.setStepperAll(false);
	}

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

}

void HakoniwaParallelLink_Base::onDisabled(){

}