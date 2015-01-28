//
//  simpleSwitcher.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/22.
//
//

#include "simpleSwitcher.h"
ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

void simpleSwitcher::setup(){
	hakoniwas.push_back("dpVisPLink_Laser");
	hakoniwas.push_back("dpVisPLink_Oil");
	hakoniwas.push_back("dpVisSandStorm");
	hakoniwas.push_back("dpVisMagPendulum");
	hakoniwas.push_back("dpHGearMove");
	hakoniwas.push_back("dpVisServoPendulum");
	hakoniwas.push_back("dpVisStruggle");
	hakoniwas.push_back("dpVisTornado");
	hakoniwas.push_back("dpHWorm");
	hakoniwas.push_back("dpHfrozenIce");
	hakoniwas.push_back("dpVisTheta");
	hakoniwas.push_back("dpVisStage");
	hakoniwas.push_back("FutureRE");
	hakoniwas.push_back("distanceMetaball");
	hakoniwas.push_back("dpVisPLink_Prism");
	hakoniwas.push_back("distanceMetaball");
	hakoniwas.push_back("Line");
	hakoniwas.push_back("Four Points");
	hakoniwas.push_back("FutureRE");
	hakoniwas.push_back("Future");
	hakoniwas.push_back("OnNote");
	hakoniwas.push_back("dpHEyeBallDancer");
	hakoniwas.push_back("Kioku");
	
	hakoniwas.push_back("TestSceneA");
	hakoniwas.push_back("TestSceneB");
	hakoniwas.push_back("TestSceneC");
	hakoniwas.push_back("TestSceneD");
	hakoniwas.push_back("Blank");
	
	
	
	selector_hakoniwa = new ofxUICanvas();
	selector_hakoniwa->setup();
	selector_hakoniwa->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
								   uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	
	selector_hakoniwa->addLabel("TargetHakoniwa",OFX_UI_FONT_LARGE);
	selector_hakoniwa->addTextInput("Target", "")->setAutoClear(false);
	selector_hakoniwa->addSpacer();
	selector_hakoniwa->addRadio("Hakoniwa", hakoniwas);
	selector_hakoniwa->autoSizeToFitWidgets();
	
	selector_display = new ofxUICanvas();
	selector_display->setup();
	selector_display->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
								  uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	selector_display->addToggle("Enable", &mEnable);
	selector_display->addToggle("DISP_A", &displays[0]);
	//	selector_display->addToggle("DISP_B", &displays[1]);
	selector_display->addToggle("DISP_B", &displays[2]);
	//	selector_display->addToggle("DISP_D", &displays[3]);
	selector_display->addButton("Send", false);
	selector_display->addSpacer();
	selector_display->addButton("AllClear", false);
	selector_display->addLabel("IPAddress",OFX_UI_FONT_LARGE);
	//	selector_display->addToggle("Auto Shuffle", &mAutoShuffle);
	addressInput = selector_display->addTextInput("IPAddress", "192.168.20.5");
	addressInput->setAutoClear(false);
	
	selector_display->autoSizeToFitWidgets();
	
	ofAddListener(selector_display->newGUIEvent, this,
				  &simpleSwitcher::guiEvent);
	ofAddListener(selector_hakoniwa->newGUIEvent, this,
				  &simpleSwitcher::guiEvent);

	selector_hakoniwa->setPosition(ofGetWidth()-400, 0);
	selector_display->setPosition(ofGetWidth()-180, 0);
}

void simpleSwitcher::update(){

	
	if (mAutoShuffle && (ofGetFrameNum() % 30 == 0)){
		ofxUIRadio* slt = (ofxUIRadio*)(selector_display->getWidget("Slot"));
		ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa->getWidget("Hakoniwa"));
		
		mEnable = (ofRandomuf() < 0.5);
		for (int i = 0;i < 4;i++)
			displays[i] = ofRandomuf() < 0.5;
		
		hkn->activateToggle(hakoniwas[int(ofRandom(4))+5]);
		((ofxUITextInput*)(selector_hakoniwa->getWidget("Target")))->setTextString(hkn->getActiveName());
		
		sendMessage();
	}
}

void simpleSwitcher::draw(){
	
}

void simpleSwitcher::guiEvent(ofxUIEventArgs &ev){
	ofxUIWidget* w = ev.widget;
	
	if (w->getName() == "AllClear" && w->getState() == OFX_UI_STATE_DOWN){
		for (int i = 0;i < hakoniwas.size();i++){
			((ofxUITextInput*)(selector_hakoniwa->getWidget("Target")))->setTextString(hakoniwas[i]);
			mEnable = false;
			displays[0] = false;
			displays[1] = false;
			displays[2] = false;
			displays[3] = false;
			sendMessage();
		}
	}
	
	if (w->getName() == "Send" && w->getState() == OFX_UI_STATE_DOWN){
		sendMessage();
	}
	
	for (int i = 0;i < hakoniwas.size();i++){
		if (w->getName() == hakoniwas[i]){
			((ofxUITextInput*)(selector_hakoniwa->getWidget("Target")))->setTextString(hakoniwas[i]);
		}
	}
	
	if (w->getName() == "Target"){
		ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa->getWidget("Hakoniwa"));
		hkn->activateToggle("");
	}

}

void simpleSwitcher::sendMessage(){
	ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa->getWidget("Hakoniwa"));
	
	string h = ((ofxUITextInput*)(selector_hakoniwa->getWidget("Target")))->getTextString();
	string ad = addressInput->getTextString();
	
	int hh;
	for (int i = 0;i < hakoniwas.size();i++){
		if (h == hakoniwas[i]) hh = i;
	}
	
	ofxOscMessage m;
	m.setAddress("/ram/set_scene");
	m.addStringArg(h);
	m.addIntArg(mEnable);
	m.addIntArg(displays[0]);
	m.addIntArg(displays[2]);
	
	sender.setup(ad, 12400);
	sender.sendMessage(m);
}