#pragma once

#include "ofMain.h"

class ramFilter
{
public:
	
	virtual ~ramFilter() {}
	
	virtual const ramNodeArray& update(const ramNodeArray& src) { return src; }
	virtual const ramNodeArray& getResult() {}
	
};