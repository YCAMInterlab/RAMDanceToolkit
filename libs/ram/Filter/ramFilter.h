#pragma once

#include "ofMain.h"

#include "ramControllable.h"

class ramFilter : public ramControllable
{
public:
	
	virtual ~ramFilter() {}
	
	virtual const ramNodeArray& update(const ramNodeArray& src) { return src; }
	virtual const ramNodeArray& getResult() {}
	
};