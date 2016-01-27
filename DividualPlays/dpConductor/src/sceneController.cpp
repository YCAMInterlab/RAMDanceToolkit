//
//  sceneController.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#include "sceneController.h"

void sceneController::clearScene()
{
	sendSimpleMessage(OSC_CLEAR_SCENE_ADDR);
}

void sceneController::setup(string RDTK1_addr, string RDTK2_addr)
{
	addSender(RDTK1_addr, 10000);
	addSender(RDTK2_addr, 10000);
	
	addr_rdtk1 = RDTK1_addr;
	addr_rdtk2 = RDTK2_addr;
}

void sceneController::setScene(string name, bool RDTK_isA, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(1);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessageSelect(RDTK_isA ? addr_rdtk1 : addr_rdtk2, m);
}

void sceneController::setScene_both(string name, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(1);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessage(m);
}

void sceneController::clearExtractor(string scene)
{
	ofxOscMessage m;
	m.setAddress(OSC_EXTRACTOR_ADDR+scene+"clear");
	m.addStringArg(scene);
	sendMessage(m);
}

void sceneController::setExtractor(string scene, string name, Joint node)
{
	ofxOscMessage m;
	m.setAddress(OSC_EXTRACTOR_ADDR+scene+"/pushAct");
	m.addStringArg(name);
	m.addIntArg(node);
	sendMessage(m);
}

void sceneController::setFloatTune(string scene, string type, float value)
{
	ofxOscMessage m;
	m.setAddress(OSC_TUNE_ADDR + scene + "/" + type);
	m.addIntArg(OFX_UI_WIDGET_SLIDER_H);
	m.addFloatArg(value);
	sendMessage(m);
}

void sceneController::setToggleTune(string scene, string type, bool value)
{
	ofxOscMessage m;
	m.setAddress(OSC_TUNE_ADDR + scene + "/" + type);
	m.addIntArg(OFX_UI_WIDGET_TOGGLE);
	m.addIntArg(value);
	sendMessage(m);
}

void sceneController::setButtonTune(string scene, string type)
{
	ofxOscMessage m;
	m.setAddress(OSC_TUNE_ADDR + scene + "/" + type);
	m.addIntArg(OFX_UI_WIDGET_BUTTON);
	sendMessage(m);
}