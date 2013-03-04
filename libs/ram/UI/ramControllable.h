#pragma once

#include "ofMain.h"

class ramControllable
{
public:
	virtual ~ramControllable() {}
	virtual const string getName() = 0;
	virtual void setupControlPanel() {}
};