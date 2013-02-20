#pragma once

#include "ofMain.h"

#include "ramSimpleShadow.h"

string ramToResourcePath(string path);
void ramInitialize();

class ramSharedData
{
public:
	
	static ramSharedData& instance();
	
	ramSimpleShadow shadow;
	
	void setup();
	
private:
	
	static ramSharedData *_instance;
	
	// noncopyable
	ramSharedData() {}
	~ramSharedData() {}
	
	ramSharedData(const ramSharedData&);
	ramSharedData& operator=(const ramSharedData&);
	
};