//
//  dpSceneChanger.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/12.
//
//

#include "dpSceneChanger.h"

void dpSceneChanger::setup(){
	
	receiver.addAddress("/dp/sceneManage");
	ramOscManager::instance().addReceiverTag(&receiver);
	
}

void dpSceneChanger::update(){
	
	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if (m.getAddress() == "/dp/sceneManage/enable"){
			
		}
	}
	
}