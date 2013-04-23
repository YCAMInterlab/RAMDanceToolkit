// 
// ramBaseFilter.h - RAMDanceToolkit
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
#include "ramUnit.h"
#include "ramActor.h"

class ramBaseFilter : public ramUnit
{
public:
	const ramNodeArray& operator()(const ramNodeArray& src);
	const ramNodeArray& update(const ramNodeArray& src);
	virtual const ramNodeArray& get(size_t index = 0) const;
	virtual size_t getSize() const;

protected:
	ramNodeArray cache;
	virtual const ramNodeArray& filter(const ramNodeArray& src) = 0;

private:
	int last_update_frame;
};

//

class ramBusSend : public ramBaseFilter
{
public:

	string getName() const;
	ramBusSend();
	ramBusSend(const string &bus_name);

	void setBusName(const string &bus_name);
	const string& getBusName() const;

	const ramNodeArray& get(size_t index = 0) const;
	size_t getSize() const;

protected:

	const ramNodeArray& filter(const ramNodeArray& src);

private:

	string bus_name;
};

//

template <typename FilterClass>
class ramFilterEach
{
public:

	void clear()
	{
		cache.clear();
		filters.clear();
	}

	const vector<ramNodeArray>& update(const vector<ramNodeArray> &src)
	{
		if (ofGetFrameNum() == last_updated_time) return cache;

		last_updated_time = ofGetFrameNum();

		cache.clear();

		for (int i = 0; i < src.size(); i++)
		{
			const ramNodeArray &NA = src[i];
			const string &name = NA.getName();

			if (!filters.hasKey(name))
				filters.set(name, FilterClass());

			FilterClass &filter = filters[name];
			cache.push_back(filter.update(NA));
		}

		return cache;
	}
	
	const vector<ramNodeArray>& get(size_t index = 0) { return cache[index]; }
	const size_t getNumFilters() { return filters.size(); }
	
	FilterClass& getFilter(int index) { return filters[index]; }
	FilterClass& getFilter(const string& name) { return filters[name]; }

protected:

	ramCompoundContainer<FilterClass> filters;
	vector<ramNodeArray> cache;

	unsigned int last_updated_time;

};