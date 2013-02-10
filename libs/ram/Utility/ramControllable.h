#pragma once

#include "ramControlPanel.h"

class ramControlPanel;

class ramControllable
{
public:
	
	virtual void setupControlPanel(ofxUICanvas* panel) {}
	
	virtual const string getName() { return "unnamed"; };
};