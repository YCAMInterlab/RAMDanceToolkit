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

void sceneController::setScene(string name, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(viewA);
	m.addIntArg(viewB);

	sendMessage(m);
}