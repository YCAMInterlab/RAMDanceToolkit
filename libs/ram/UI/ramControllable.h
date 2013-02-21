#pragma once

#include "ramControlPanel.h"

class ramControllable
{
public:
	
	virtual const string getName() = 0;
	
	virtual void setupControlPanel() {}
	ramControlPanel& gui() { return ramGetGUI(); }
	
};