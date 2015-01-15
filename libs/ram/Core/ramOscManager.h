//
//  ramOscManager.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/07/14.
//  Copyright (c) 2014年 YCAMInterlab. All rights reserved.
//

#ifndef __RAMDanceToolkit__ramOscManager__
#define __RAMDanceToolkit__ramOscManager__

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramConstants.h"
#include "ramUtils.h"
#include "ofxUI.h"

class ramOscReceiveTag{
public:

	ramOscReceiveTag(const string& addr_){
		addr.push_back(addr_);
	}
	ramOscReceiveTag(){
	}

	void addAddress(const string& addr_){
		addr.push_back(addr_);
	}

	void addMessage(const ofxOscMessage& m){
		msg.push(m);

<<<<<<< HEAD
		while (msg.size() > 10){
			msg.erase(msg.begin());
=======
		while (msg.size() > 128){
            msg.pop();
>>>>>>> 96cec5f6445fb879293b5d619e11bbf5d861df8e
		}
	}

	void getNextMessage(ofxOscMessage* m){
		m->copy(msg.front());
        msg.pop();
	}

	bool hasWaitingMessages() const {return (msg.size() > 0);};

	queue<ofxOscMessage> msg;
	vector<string> addr;

};

class ramOscManager{
public:

	void setup(int receivePort);
	void update();

	void addReceiverTag(ramOscReceiveTag* ptr);

	inline static ramOscManager& instance()
	{
		if (__instance == NULL)
			__instance = new ramOscManager;
		return *__instance;
	}

private:
	static ramOscManager *__instance;

	ramOscManager() {};
	ramOscManager(const ramOscManager&){}
	ramOscManager& operator=(const ramOscManager&){return *__instance;}
	~ramOscManager() {};

	vector<ramOscReceiveTag*> receiverList;
	ofxOscReceiver receiver;

};

#endif /* defined(__RAMDanceToolkit__ramOscManager__) */
