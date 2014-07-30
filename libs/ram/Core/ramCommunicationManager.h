//
//  ramCommunicationManager.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/06/18.
//  Copyright (c) 2014 YCAMInterlab. All rights reserved.
//

#ifndef __RAMDanceToolkit__ramCommunicationManager__
#define __RAMDanceToolkit__ramCommunicationManager__

#include "ofMain.h"
#include "ramOscManager.h"
#include "ramConstants.h"
#include "ramUtils.h"
#include "ofxUI.h"


struct ramCommunicateAssign;

class ramCommunicationManager
{
public:

	void	setup(ramOscManager* oscMan);
	void	update();
	void	draw();

	//Sender Methods
	void	addSender(string address,int port);
	void	sendOscMessage(ofxOscMessage &m);
	void	sendNoteOn(string name,float velocity);
	void	sendNoteOff(string name);
	void	sendCC(string name,vector<float>cc);
	void	sendCC(string name,float* cc,int num);

	//Receiver Methods
	void	assignVelocity(string name, float* value);
	void	assignCC(string name, int ccNum, float* value);

	bool	getVelocityExist(string name);
	bool	getVelocityExist(int index);
	bool	getCCExist(string name,int ccNum);
	bool	getCCExist(int index,int ccNum);
	int		getNumCCArg(string name);
	float	getVelocity(string name);
	float	getVelocity(int index);
	float	getCC(string name,int ccNum);
	float	getCC(int index,int ccNum);

	int				getInstNum(string name);
	ofxUICanvas*	getCanvas(){return &UIcanvas;};

	inline static ramCommunicationManager& instance()
	{
		if (__instance == NULL)
			__instance = new ramCommunicationManager;
		return* __instance;
	};

private:
	int addInstrument(string name);
	static ramCommunicationManager *__instance;

	ramCommunicationManager() {};
	ramCommunicationManager(const ramCommunicationManager&){}
	ramCommunicationManager& operator=(const ramCommunicationManager&) {return *this; }
	~ramCommunicationManager() {};

	bool bVisible;
	bool bEnable;

	//Gui
	void							refleshInstruments();
	vector<ofParameterGroup*>		Instruments;
	vector<ofxUISlider*>			velocities;
	vector<vector<ofxUISlider*> >	ccs;
	ofxUICanvas						UIcanvas;
	ofxUICanvas						mainPanel;

	//OSC
	ramOscManager*		oscManager;
	ramOscReceiveTag	oscReceiver;

	void				updateWithOscMessage(const ofxOscMessage &m);

	ofEvent<ofxUIEventArgs> newGUIEvent;
	void guiEvent(ofxUIEventArgs &e);
	void keyPressed(ofKeyEventArgs &key);
	void windowResized(ofResizeEventArgs &win);

	//Assign
	vector<ramCommunicateAssign> assigns;
};

struct ramCommunicateAssign{

	ramCommunicateAssign(string name,int index, float* val){
		target = name;
		idx = index;
		value = val;
		Found = false;
		Changed = false;
		Pindex = -1;
		Pposition = -1;
	}

	string	target;
	int		idx;
	float*	value;

	bool Found;
	bool Changed;
	int Pindex;
	int Pposition;
};

#endif /* defined(__RAMDanceToolkit__ramCommunicationManager__) */