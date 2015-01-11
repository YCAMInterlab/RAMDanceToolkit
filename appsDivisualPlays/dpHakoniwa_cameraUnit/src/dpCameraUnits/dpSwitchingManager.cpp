//
//  dpSwitchingManager.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2015/01/09.
//
//

#include "dpSwitchingManager.h"

void dpSwitchingManager::setup(){

}

void dpSwitchingManager::update(){

}

void dpSwitchingManager::draw(){

}

void dpSwitchingManager::receiveOscMessage(ofxOscMessage &m){

	if (m.getAddress() == "/dp/switch/enable"){

	}
	if (m.getAddress() == "/dp/switch/disable"){

	}

}

void dpSwitchingManager::enableHakoniwa(hakoniwaType type, dpDisplaySelector disp){



}