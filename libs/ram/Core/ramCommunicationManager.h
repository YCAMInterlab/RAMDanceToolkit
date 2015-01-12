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
	void	addSender(const string& address,int port);
	void	sendOscMessage(ofxOscMessage& m);
	void	sendNoteOn(const string& name,float velocity);
	void	sendNoteOff(const string& name);
	void	sendCC(const string& name, vector<float> cc);
	void	sendCC(const string& name, float* cc, int num);

	//Receiver Methods
	void	assignVelocity(const string& name, float* value);
	void	assignCC(const string& name, int ccNum, float* value);

	bool	getVelocityExist(const string& name) const;
	bool	getVelocityExist(int index) const;
	bool	getCCExist(const string& name, int ccNum) const;
	bool	getCCExist(int index, int ccNum) const;
	int		getNumCCArg(const string& name) const;
	float	getVelocity(const string& name) const;
	float	getVelocity(int index) const;
	float	getCC(const string& name, int ccNum) const;
	float	getCC(int index, int ccNum) const;

	int				getInstNum(const string& name) const;
	ofxUICanvas*	getCanvas(){return &UIcanvas;};

	inline static ramCommunicationManager& instance()
	{
		if (__instance == NULL)
			__instance = new ramCommunicationManager;
		return *__instance;
	};

private:
	int addInstrument(const string &name);
	static ramCommunicationManager* __instance;

	ramCommunicationManager() {};
	ramCommunicationManager(const ramCommunicationManager&){}
	ramCommunicationManager& operator=(const ramCommunicationManager&) {return *__instance; }
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

	ramCommunicateAssign(const string &name,int index, float* val){
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