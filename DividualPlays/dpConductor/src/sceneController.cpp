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

void sceneController::setScene(string name, bool RDTK_isA, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessageSelect(RDTK_isA ? addr_rdtk1 : addr_rdtk2, m);
}

void sceneController::setScene_both(string name, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessage(m);
}

void sceneController::setExtractor(string scene, string name, Joint node);
{
	ofxOscMessage m;
	m.setAddress(OSC_EXTRACTOR_ADDR+scene+"/push");
}