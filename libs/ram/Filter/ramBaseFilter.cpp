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

using namespace rdtk;

const NodeArray& BaseFilter::operator()(const NodeArray& src)
{
	return update(src);
}

const NodeArray& BaseFilter::update(const NodeArray& src)
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

const NodeArray& BaseFilter::get(size_t index) const { return cache; }
size_t BaseFilter::getSize() const { return 1; }

//
string BusSend::getName() const { return "ramBusSend"; }

BusSend::BusSend() {}
BusSend::BusSend(const string &bus_name) { setBusName(bus_name); }

void BusSend::setBusName(const string &bus_name) { this->bus_name = bus_name; }
const string& BusSend::getBusName() const { return bus_name; }

const NodeArray& BusSend::get(size_t index) const { static NodeArray arr; return arr; }
size_t BusSend::getSize() const { return 1; }

const NodeArray& BusSend::filter(const NodeArray& src)
{
	ActorManager::instance().setBus(bus_name, src);
	return src;
}

