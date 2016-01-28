//
//  cameraUnitManager.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#include "cameraUnitManager.h"

void cameraUnitManager::setup(string host)
{
	addSender(host, 12400);
}

void cameraUnitManager::setCameraSlot(string name, bool enable, bool viewA, bool viewB)
{
	ofxOscMessage m;
	m.setAddress(OSC_SET_SCENE_ADDR);
	m.addStringArg(name);
	m.addIntArg(enable);
	m.addIntArg(viewA);
	m.addIntArg(viewB);
	sendMessage(m);
}