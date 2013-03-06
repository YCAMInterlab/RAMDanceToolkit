#pragma once

#include "ofMain.h"

class ramControllable
{
public:
	virtual ~ramControllable() {}
	virtual string getName() const = 0;
	virtual void setupControlPanel() {}
};