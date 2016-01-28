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
	
	scene_rdtk1_a = "RDTK_1_A";
	scene_rdtk1_b = "RDTK_1_B";
	scene_rdtk2_a = "RDTK_2_A";
	scene_rdtk2_b = "RDTK_2_B";
}

void sceneController::disableScene(string name, bool RDTK_isA)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(0);
	m.addIntArg(0);
	m.addIntArg(0);
	sendMessageSelect(RDTK_isA ? addr_rdtk1 : addr_rdtk2, m);
}

void sceneController::setScene(string name, bool RDTK_is1, bool viewA, bool viewB)
{
	if (RDTK_is1)
	{
		if (viewA) scene_rdtk1_a = name;
		if (viewB) scene_rdtk1_b = name;
	}else
	{
		if (viewA) scene_rdtk2_a = name;
		if (viewB) scene_rdtk2_b = name;
	}
	
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(1);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessageSelect(RDTK_is1 ? addr_rdtk1 : addr_rdtk2, m);
}

void sceneController::setScene_both(string name, bool viewA, bool viewB)
{
	if (viewA) scene_rdtk1_a = name;
	if (viewB) scene_rdtk1_b = name;
	if (viewA) scene_rdtk2_a = name;
	if (viewB) scene_rdtk2_b = name;
	
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
	m.setAddress(OSC_EXTRACTOR_ADDR+scene+"/clear");
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