#pragma once

class ramOscReceiver
{
	
public:
	ramOscReceiver(){}
	~ramOscReceiver(){}
	
	void setup(unsigned int port)
	{
		oscReceiver.setup(port);
	}
	
	void update()
	{
		while (oscReceiver.hasWaitingMessages())
		{
			ofxOscMessage m;
			oscReceiver.getNextMessage(&m);
			ramActorManager::instance().updateWithOscMessage(m);
		}
	}
	
private:
	
	ofxOscReceiver oscReceiver;
};