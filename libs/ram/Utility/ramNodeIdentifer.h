#pragma once

#include "ofMain.h"

struct ramNodeIdentifer
{
	string name;
	int index;
	
	ramNodeIdentifer() : name(""), index(-1) {}
	ramNodeIdentifer(const string &name, int index) : name(name), index(index) {}
	
	void set(const string &name, int index) { this->name = name; this->index = index; }
	
	void clear() { name = ""; index = -1; }
};
