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

struct ramOscSendTag{

	ramOscSendTag(int port_, const string& addr_){
		port = port_;
		address = addr_;
	}
	int port;
	string address;

};

class ramOscReceiveTag{
public:

	ramOscReceiveTag(const string& addr_){
		string ad = addr_;
		addr.push_back(ad);
	}
	ramOscReceiveTag(){
	}

	void addAddress(const string& addr_){
		string ad = addr_;
		addr.push_back(ad);
	}

	void addMessage(ofxOscMessage& m){
		msg.push_back(ofxOscMessage(m));

		while (msg.size() > 128){
			msg.erase(msg.begin());
		}
	}

	void getNextMessage(ofxOscMessage* m){
		*m = msg[0];
		msg.erase(msg.begin());
	}

	bool hasWaitingMessages() const {return (msg.size() > 0);};

	vector<ofxOscMessage> msg;
	vector<string> addr;

};

class ramOscManager{
public:

	void setup(int receivePort);
	void update();

	void sendMessage(ofxOscMessage& m);
	void addSenderTag(int port, const string& address);
	void addReceiverTag(ramOscReceiveTag* ptr);

	inline static ramOscManager& instance()
	{
		if (__instance == NULL)
			__instance = new ramOscManager;
		return *__instance;
	}

	ofxOscReceiver* getReceiver()	{return &receiver;};
	ofxOscSender*	getSender()		{return &sender;};

private:
	static ramOscManager *__instance;

	ramOscManager() {};
	ramOscManager(const ramOscManager&){}
	ramOscManager& operator=(const ramOscManager&){return *__instance;}
	~ramOscManager() {};

	vector<ramOscReceiveTag*> receiverList;
	vector<ramOscSendTag> senderlist;

	ofxOscSender sender;
	ofxOscReceiver receiver;

};

#endif /* defined(__RAMDanceToolkit__ramOscManager__) */
