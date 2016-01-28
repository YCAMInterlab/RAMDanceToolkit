//
//  environmentManager.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#include "environmentManager.h"

void environmentManager::setCameraPreset(ofVec3f position, ofVec3f target, float speed, float smooth)
{

	setFloatTune(camScn, "targ_x", position.x);
	setFloatTune(camScn, "targ_y", position.y);
	setFloatTune(camScn, "targ_z", position.z);
	setFloatTune(camScn, "look_x", target.x);
	setFloatTune(camScn, "look_x", target.y);
	setFloatTune(camScn, "look_x", target.z);
	setFloatTune(camScn, "speed", speed);
	setFloatTune(camScn, "smooth", smooth);
	setToggleTune(camScn, "nodeCam", false);
}

void environmentManager::setNodeCamera(string actor, Joint from, Joint look)
{
	setToggleTune(camScn, "nodeCam", true);
	clearExtractor(camScn);
	setExtractor(camScn, actor, from);
	setExtractor(camScn, actor, look);
}

void environmentManager::setViewFloor(bool b)
{
	setToggleTune("dp16_ramActorTranslator", "drawFloor", b);
}

void environmentManager::setActorMargine(float value)
{
	setFloatTune("dp16_ramActorTranslator", "margine", value);
}

void environmentManager::setActorDraw(string name, bool view)
{
	setToggleTune("dp16_ramActorTranslator", "V_"+name, view);
}