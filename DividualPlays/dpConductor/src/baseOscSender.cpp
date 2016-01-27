//
//  baseOscSender.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#include "baseOscSender.h"

void baseOscSender::addSender(string addr, int port)
{
	senders.insert(pair<string, int>(addr, port));
}

void baseOscSender::sendSimpleMessage(string address)
{
	ofxOscMessage m;
	m.setAddress(address);	
	sendMessage(m);
}

void baseOscSender::sendMessage(ofxOscMessage m)
{
	map<string, int>::iterator it = senders.begin();
	
	ofxOscSender sender;
	
	while (it != senders.end())
	{
		sender.setup(it->first, it->second);
		++it;
	}
}