// 
// ramBaseFilter.cpp - RAMDanceToolkit
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

#include "ramBaseFilter.h"

const ramNodeArray& ramBaseFilter::operator()(const ramNodeArray& src)
{
	return update(src);
}

const ramNodeArray& ramBaseFilter::update(const ramNodeArray& src)
{
	
	if (cache.getNumNode() != src.getNumNode())
		cache = src;
	
	const int f = ofGetFrameNum();
	if (last_update_frame != f)
	{
		last_update_frame = f;
		cache = filter(src);
	}
	
	return cache;
}

const ramNodeArray& ramBaseFilter::get(size_t index) const { return cache; }
size_t ramBaseFilter::getSize() const { return 1; }

//
string ramBusSend::getName() const { return "ramBusSend"; }

ramBusSend::ramBusSend() {}
ramBusSend::ramBusSend(const string &bus_name) { setBusName(bus_name); }

void ramBusSend::setBusName(const string &bus_name) { this->bus_name = bus_name; }
const string& ramBusSend::getBusName() const { return bus_name; }

const ramNodeArray& ramBusSend::get(size_t index) const { static ramNodeArray arr; return arr; }
size_t ramBusSend::getSize() const { return 1; }

const ramNodeArray& ramBusSend::filter(const ramNodeArray& src)
{
	ramActorManager::instance().setBus(bus_name, src);
	return src;
}

