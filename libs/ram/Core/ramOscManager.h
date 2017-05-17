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

namespace rdtk{
	
	class OscReceiveTag{
	public:
		
		OscReceiveTag(const string& addr_){
			addr.push_back(addr_);
		}
		OscReceiveTag(){
		}
		
		void addAddress(const string& addr_){
			addr.push_back(addr_);
		}
		
		void addMessage(const ofxOscMessage& m){
			msg.push(m);
			
			while (msg.size() > 128){
				msg.pop();
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
	
	class OscManager{
	public:
		
		void setup(int receivePort);
		void update();
		
		void addReceiverTag(OscReceiveTag* ptr);
		
		inline static OscManager& instance()
		{
			if (__instance == NULL)
				__instance = new OscManager;
			return *__instance;
		}
		
	private:
		static OscManager *__instance;
		
		OscManager() {};
		OscManager(const OscManager&){}
		OscManager& operator=(const OscManager&){return *__instance;}
		~OscManager() {};
		
		vector<OscReceiveTag*> receiverList;
		ofxOscReceiver receiver;
		
	};

}

typedef rdtk::OscReceiveTag RAMDEPRECATED(ramOscReceiveTag);
typedef rdtk::OscManager	RAMDEPRECATED(ramOscManager);

#endif /* defined(__RAMDanceToolkit__ramOscManager__) */
