#pragma once

#include "ramControlPanel.h"

class ramControlPanel;

class ramControllable
{
public:
	
	virtual void setupControlPanel() {}
	
	virtual const string getName() = 0;
};