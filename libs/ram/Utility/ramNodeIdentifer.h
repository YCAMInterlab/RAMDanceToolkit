// 
// ramNodeIdentifer.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

	bool isValid() const
	{
		if (name == "" && index == -1) return false;
		else return true;
	}

	friend ostream& operator<<(ostream &os, const ramNodeIdentifer& o)
	{
		if (o.isValid())
			os << "['" << o.name << "' : " << o.index << "]";
		else
			os << "(null)";
		return os;
	}
};