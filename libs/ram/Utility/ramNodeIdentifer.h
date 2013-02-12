#pragma once

#include "ofMain.h"

struct ramNodeIdentifer
{
	string name;
	int index;
	
	ramNodeIdentifer() : name(""), index(-1) {}
	ramNodeIdentifer(int index) : name(""), index(index) {}
	ramNodeIdentifer(const string &name) : name(name), index(-1) {}
	ramNodeIdentifer(const string &name, int index) : name(name), index(index) {}
	ramNodeIdentifer(const ramNodeIdentifer& copy) { *this = copy; }
	
	ramNodeIdentifer& operator=(const ramNodeIdentifer& copy)
	{
		name = copy.name;
		index = copy.index;
		return *this;
	}
	
	void set(const string &name, int index) { this->name = name; this->index = index; }
	void set(const string &name) { this->name = name; this->index = -1; }
	void set(int index) { this->name = ""; this->index = index; }

	void clear() { name = ""; index = -1; }
};
