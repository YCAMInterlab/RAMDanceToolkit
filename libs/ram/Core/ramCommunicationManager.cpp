//
//  ramCommunicationManager.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/06/18.
//  Copyright (c) 2014 YCAMInterlab. All rights reserved.
//

#include "ramCommunicationManager.h"

using namespace rdtk;

CommunicationManager* CommunicationManager::__instance = NULL;
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
void CommunicationManager::setup(OscManager* oscMan){

	mainPanel.setName("mainPanel");
	mainPanel.addLabel("Communicator");
	mainPanel.addToggle("Enable", true);
	mainPanel.autoSizeToFitWidgets();
	mainPanel.disableAppDrawCallback();

	UIcanvas.setName("Communicator");
	UIcanvas.disableAppDrawCallback();
	//UIcanvas.setAutoDraw(false);
	//UIcanvas.setUIColors(uiThemecb, uiThemeco, uiThemecoh, uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);

	refleshInstruments();

	ofAddListener(newGUIEvent, this, &CommunicationManager::guiEvent);
	ofAddListener(ofEvents().keyPressed, this, &CommunicationManager::keyPressed);
	ofAddListener(ofEvents().windowResized, this, &CommunicationManager::windowResized);

	bVisible = false;

	oscManager = oscMan;
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEON);
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEOFF);
	oscReceiver.addAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	oscMan->addReceiverTag(&oscReceiver);
}

void CommunicationManager::update(){

	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

		CommunicationManager::instance().updateWithOscMessage(m);
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

void CommunicationManager::draw(){
	if (bVisible) UIcanvas.draw();
}

void CommunicationManager::updateWithOscMessage(const ofxOscMessage &m){
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
		bVisible = true;
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
		bVisible = true;
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
		bVisible = true;
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

int CommunicationManager::addInstrument(const string &name){

	ofParameterGroup* newInst = new ofParameterGroup();

	newInst->setName(name);
	newInst->add((*new ofParameter<float>).set("velocity",0.0,0.0,1.0));

	Instruments.push_back(newInst);

	refleshInstruments();

	return Instruments.size() - 1;
}

void CommunicationManager::refleshInstruments(){

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

float CommunicationManager::getVelocity(const string &name) const {
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getVelocity(index);
}

float CommunicationManager::getVelocity(int index) const {
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	return Instruments[index]->getFloat("velocity");
}

float CommunicationManager::getCC(const string &name, int ccNum) const {
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getCC(index,ccNum);
}

float CommunicationManager::getCC(int index, int ccNum) const {
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	if (Instruments[index]->contains("cc"+ofToString(ccNum)))
		return Instruments[index]->getFloat("cc"+ofToString(ccNum));

	return 0.0;
}

void CommunicationManager::guiEvent(ofxUIEventArgs &e){

}

void CommunicationManager::keyPressed(ofKeyEventArgs &key){
	if (key.key == OF_KEY_TAB) bVisible ^= true;
}

bool CommunicationManager::getVelocityExist(const string& name) const {
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getVelocityExist(index);
}

bool CommunicationManager::getVelocityExist(int index) const {
	if ((Instruments.size() == 0) || (index >= Instruments.size()) || (index < 0))
		return false;
	else
		return true;
}

bool CommunicationManager::getCCExist(const string& name, int ccNum) const {
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}
	return getCCExist(index, ccNum);
}

bool CommunicationManager::getCCExist(int index, int ccNum) const {
	if ((Instruments.size() == 0) || index >= Instruments.size() || (index < 0))
		return false;

	if (Instruments[index]->contains("cc"+ofToString(ccNum))){
		return true;
	}else{
		return false;
	}
}

void CommunicationManager::assignVelocity(const string& name, float* value){

	assigns.push_back(CommunicateAssign(name, -1, value));

}

void CommunicationManager::assignCC(const string& name, int ccNum, float* value){

	assigns.push_back(CommunicateAssign(name, ccNum, value));

}

int CommunicationManager::getInstNum(const string& name) const {
	int index = -1;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}
	return index;
}

void CommunicationManager::windowResized(ofResizeEventArgs &win){

	UIcanvas.setPosition(win.width - UIcanvas.getRect()->getWidth(), 0.0);

}

void CommunicationManager::addSender(const string& address, int port){
	if (oscManager != NULL){
        ofxOscSender *sender = new ofxOscSender;
        sender->setup(address, port);
        senders.push_back(shared_ptr<ofxOscSender>(sender));
	}
}

void CommunicationManager::sendNoteOn(const string& name, float velocity){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEON);
	m.addStringArg(name);
	m.addFloatArg(velocity);
    for(int i = 0; i < senders.size(); i++) {
        senders[i]->sendMessage(m);
    }
}

void CommunicationManager::sendNoteOff(const string& name){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_NOTEOFF);
	m.addStringArg(name);
	m.addFloatArg(0.0);
    for(int i = 0; i < senders.size(); i++) {
        senders[i]->sendMessage(m);
    }
}

void CommunicationManager::sendCC(const string& name, vector<float> cc){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	m.addStringArg(name);

	for (int i = 0;i < cc.size();i++){
		m.addFloatArg(cc[i]);
	}

    for(int i = 0; i < senders.size(); i++) {
        senders[i]->sendMessage(m);
    }
}

void CommunicationManager::sendCC(const string& name, float *cc, int num){
	ofxOscMessage m;
	m.setAddress(RAM_OSC_ADDR_COMMUNICATE_CC);
	m.addStringArg(name);

	for (int i = 0;i < num;i++){
		m.addFloatArg(cc[i]);
	}

    for(int i = 0; i < senders.size(); i++) {
        senders[i]->sendMessage(m);
    }
}

void CommunicationManager::sendOscMessage(ofxOscMessage &m){
    for(int i = 0; i < senders.size(); i++) {
        senders[i]->sendMessage(m);
    }
}

int CommunicationManager::getNumCCArg(const string& name) const {
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
