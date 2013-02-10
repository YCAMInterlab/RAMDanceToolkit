#pragma once

#include "ofMain.h"

#include "ramControllable.h"
#include "ramControlPanel.h"

class ramFilter : public ramControllable
{
public:
	
	virtual ~ramFilter() {}
	
	virtual void setupControlPanel(ramControlPanel& gui) {}
	
	virtual const ramNodeArray& update(const ramNodeArray& src) { return src; }
	virtual const ramNodeArray& getResult() {}
};