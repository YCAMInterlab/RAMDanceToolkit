//
//  baseOscSender.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef baseOscSender_h
#define baseOscSender_h

#include "ofMain.h"
#include "ofxOsc.h"

class baseOscSender{
public:
	
	void addSender(string addr, int port);
	
	void sendSimpleMessage(string address);
	void sendMessage(ofxOscMessage m);
	
	map<string, int> senders;
};

#endif /* baseOscSender_h */
