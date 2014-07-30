//
//  ramCommunicationManager.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/06/18.
//  Copyright (c) 2014 YCAMInterlab. All rights reserved.
//

#include "ramCommunicationManager.h"

ramCommunicationManager* ramCommunicationManager::__instance = NULL;
/*
ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);
*/
void ramCommunicationManager::setup(ramOscManager* oscMan){

	mainPanel.setName("mainPanel");
	mainPanel.addLabel("Communicator");
	mainPanel.addToggle("Enable", true);
	mainPanel.autoSizeToFitWidgets();
	mainPanel.setAutoDraw(false);

	UIcanvas.setName("Communicator");
	UIcanvas.setAutoDraw(false);
	//UIcanvas.setUIColors(uiThemecb, uiThemeco, uiThemecoh, uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);

	refleshInstruments();

	ofAddListener(newGUIEvent, this, &ramCommunicationManager::guiEvent);
	ofAddListener(ofEvents().keyPressed, this, &ramCommunicationManager::keyPressed);
	ofAddListener(ofEvents().windowResized, this, &ramCommunicationManager::windowResized);

	bVisible = true;

	oscManager = oscMan;
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEON);
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEOFF);
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	oscMan->addReceiverTag(&oscReceiver);
}

void ramCommunicationManager::update(){

	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

		ramCommunicationManager::instance().updateWithOscMessage(m);
	}

	for (int i = 0;i < assigns.size();i++){
		if (assigns[i].idx == -1){
			if (assigns[i].Found && assigns[i].Changed){

				*assigns[i].value = Instruments[assigns[i].Pindex]->getFloat(assigns[i].Pposition);
				assigns[i].Changed = false;

			}else if (getVelocityExist(assigns[i].target)){

				assigns[i].Found = true;
				assigns[i].Pindex = getInstNum(assigns[i].target);
				assigns[i].Pposition = 0;

			}
		}else{

			if (assigns[i].Found && assigns[i].Changed){

				*assigns[i].value = Instruments[assigns[i].Pindex]->getFloat(assigns[i].Pposition);
				assigns[i].Changed = false;

			}else if (getCCExist(assigns[i].target,assigns[i].idx)){

				assigns[i].Found = true;
				assigns[i].Pindex = getInstNum(assigns[i].target);
				assigns[i].Pposition = assigns[i].idx + 1;

			}
		}
	}
	
}

void ramCommunicationManager::draw(){
	if (bVisible) UIcanvas.draw();
}

void ramCommunicationManager::updateWithOscMessage(const ofxOscMessage &m){
	if (!dynamic_cast<ofxUIToggle*>(mainPanel.getWidget("Enable"))->getValue()) return;
	
	const std::string addr = m.getAddress();
	const std::string name = m.getArgAsString(0);

	int index = 0;
	int assIdx = -1;

	bool isExist = false;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
		{
			index   = i;
			isExist = true;
		}
	}

	if (!isExist) index = addInstrument(name);


	if (addr == RAM_OSC_ADDR_COMMUNICATE_NOTEON){
		Instruments[index]->getFloat("velocity") = m.getArgAsFloat(1);
		velocities[index]->setValue(m.getArgAsFloat(1));
		assIdx = 0;

		for (int j = 0;j < assigns.size();j++){
			if (assigns[j].Found &&
				assigns[j].Pindex == index &&
				assigns[j].Pposition == assIdx)
				assigns[j].Changed = true;
		}
	}

	else if (addr == RAM_OSC_ADDR_COMMUNICATE_NOTEOFF){
		Instruments[index]->getFloat("velocity") = 0.0;
		velocities[index]->setValue(0.0);
		assIdx = 0;

		for (int j = 0;j < assigns.size();j++){
			if (assigns[j].Found &&
				assigns[j].Pindex == index &&
				assigns[j].Pposition == assIdx)
				assigns[j].Changed = true;
		}
	}

	else if (addr == RAM_OSC_ADDR_COMMUNICATE_CC){
		int ccNum = m.getNumArgs();
		for (int i = 0;i < ccNum - 1;i++){
			string ccLabel = "cc" + ofToString(i);

			if (Instruments[index]->contains(ccLabel))
			{

				Instruments[index]->getFloat(ccLabel) = m.getArgAsFloat(i+1);
				ccs[index][i]->setValue(m.getArgAsFloat(i+1));
				assIdx = i;

				for (int j = 0;j < assigns.size();j++){
					if (assigns[j].Found &&
						assigns[j].Pindex == index &&
						assigns[j].Pposition == assIdx)
						assigns[j].Changed = true;
				}
			}
			else
			{
				Instruments[index]->add((*new ofParameter<float>).set(ccLabel,
																	  m.getArgAsFloat(i+1),0.0,1.0));
				refleshInstruments();
			}

		}

	}

}

int ramCommunicationManager::addInstrument(string name){

	ofParameterGroup* newInst = new ofParameterGroup();

	newInst->setName(name);
	newInst->add((*new ofParameter<float>).set("velocity",0.0,0.0,1.0));

	Instruments.push_back(newInst);

	refleshInstruments();

	return Instruments.size() - 1;
}

void ramCommunicationManager::refleshInstruments(){

	UIcanvas.removeWidgets();
	UIcanvas.resetPlacer();
	UIcanvas.addSpacer(0,0);
	UIcanvas.addWidgetDown(&mainPanel);

	velocities.clear();
	ccs.clear();

	for (int i = 0;i < Instruments.size();i++){
		UIcanvas.addSpacer();
		UIcanvas.addLabel("Instruments"+ofToString(i)+" : "+Instruments[i]->getName(),OFX_UI_FONT_SMALL);
		velocities.push_back(UIcanvas.addSlider("velocity",
												0.0,
												1.0,
												Instruments[i]->getFloat("velocity"),
												150,3));

		vector<ofxUISlider*> cc;
		ccs.push_back(cc);

		for (int j = 0;j < 64;j++){
			if (Instruments[i]->contains("cc"+ofToString(j))){
				ccs[i].push_back(UIcanvas.addSlider("cc"+ofToString(j),
													0.0,
													1.0,
													0.0,150,3));
			}
		}
	}

	UIcanvas.autoSizeToFitWidgets();
	UIcanvas.setPosition(ofGetWidth() - UIcanvas.getRect()->getWidth(), 0.0);

}

float ramCommunicationManager::getVelocity(string name){
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getVelocity(index);
}

float ramCommunicationManager::getVelocity(int index){
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	return Instruments[index]->getFloat("velocity");
}

float ramCommunicationManager::getCC(string name, int ccNum){
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getCC(index,ccNum);
}

float ramCommunicationManager::getCC(int index, int ccNum){
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	if (Instruments[index]->contains("cc"+ofToString(ccNum)))
		return Instruments[index]->getFloat("cc"+ofToString(ccNum));

	return 0.0;
}

void ramCommunicationManager::guiEvent(ofxUIEventArgs &e){

}

void ramCommunicationManager::keyPressed(ofKeyEventArgs &key){
	if (key.key == OF_KEY_TAB) bVisible ^= true;
}

bool ramCommunicationManager::getVelocityExist(string name){
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getVelocityExist(index);
}

bool ramCommunicationManager::getVelocityExist(int index){
	if ((Instruments.size() == 0) || (index >= Instruments.size()) || (index < 0))
		return false;
	else
		return true;
}

bool ramCommunicationManager::getCCExist(string name,int ccNum){
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}
	return getCCExist(index, ccNum);
}

bool ramCommunicationManager::getCCExist(int index,int ccNum){
	if ((Instruments.size() == 0) || index >= Instruments.size() || (index < 0))
		return false;

	if (Instruments[index]->contains("cc"+ofToString(ccNum))){
		return true;
	}else{
		return false;
	}
}

void ramCommunicationManager::assignVelocity(string name, float *value){

	assigns.push_back(ramCommunicateAssign(name, -1, value));

}

void ramCommunicationManager::assignCC(string name, int ccNum, float *value){

	assigns.push_back(ramCommunicateAssign(name, ccNum, value));

}

int ramCommunicationManager::getInstNum(string name){
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}
	return index;
}

void ramCommunicationManager::windowResized(ofResizeEventArgs &win){

	UIcanvas.setPosition(win.width - UIcanvas.getRect()->getWidth(), 0.0);

}

void ramCommunicationManager::addSender(string address, int port){
	if (oscManager != NULL){
		oscManager->addSenderTag(port, address);
	}
}

void ramCommunicationManager::sendNoteOn(string name, float velocity){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEON);
	m.addStringArg(name);
	m.addFloatArg(velocity);
	if (oscManager != NULL) oscManager->sendMessage(m);
}

void ramCommunicationManager::sendNoteOff(string name){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEOFF);
	m.addStringArg(name);
	m.addFloatArg(0.0);
	if (oscManager != NULL) oscManager->sendMessage(m);
}

void ramCommunicationManager::sendCC(string name, vector<float> cc){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	m.addStringArg(name);

	for (int i = 0;i < cc.size();i++){
		m.addFloatArg(cc[i]);
	}

	if (oscManager != NULL) oscManager->sendMessage(m);
}

void ramCommunicationManager::sendCC(string name, float *cc, int num){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	m.addStringArg(name);

	for (int i = 0;i < num;i++){
		m.addFloatArg(cc[i]);
	}

	if (oscManager != NULL) oscManager->sendMessage(m);
}

void ramCommunicationManager::sendOscMessage(ofxOscMessage &m){
	if (oscManager != NULL) oscManager->sendMessage(m);
}

int ramCommunicationManager::getNumCCArg(string name){
	int count = 0;
	for (int i = 0;i < 64;i++){
		if (getCCExist(name, i)){
			count++;
		}else{
			break;
		}
	}
	
	return count;
}