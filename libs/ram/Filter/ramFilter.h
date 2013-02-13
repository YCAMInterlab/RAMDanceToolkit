#pragma once

#include "ofMain.h"

#include "ramControllable.h"
#include "ramControlPanel.h"

class ramFilter : public ramControllable
{
public:
	
	virtual ~ramFilter() {}
	
	virtual void setupControlPanel(ramControlPanel& gui) {}
	virtual void setup() {}
	virtual const ramNodeArray& update(const ramNodeArray& src) { return src; }
	virtual const ramNodeArray& getResult() { static ramNodeArray arr; return arr; }
};

class ramBusSend : public ramFilter
{
public:
	
	ramBusSend() {}
	ramBusSend(const string &bus_name) { setBusName(bus_name); }
	
	void setBusName(const string &bus_name) { this->bus_name = bus_name; }
	const string& getBusName() const { return bus_name; }
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		ramActorManager::instance().setBus(bus_name, src);
		return src;
	}
	
private:
	
	string bus_name;
};
