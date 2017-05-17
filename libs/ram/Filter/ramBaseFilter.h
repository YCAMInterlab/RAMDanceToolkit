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

namespace rdtk{
	
	class BaseFilter : public Unit{
	public:
		const NodeArray& operator()(const NodeArray& src);
		const NodeArray& update(const NodeArray& src);
		virtual const NodeArray& get(size_t index = 0) const;
		virtual size_t getSize() const;
		
	protected:
		NodeArray cache;
		virtual const NodeArray& filter(const NodeArray& src) = 0;
		
	private:
		int last_update_frame;
	};
	
	//
	
	class BusSend : public BaseFilter
	{
	public:
		
		string getName() const;
		BusSend();
		BusSend(const string &bus_name);
		
		void setBusName(const string &bus_name);
		const string& getBusName() const;
		
		const NodeArray& get(size_t index = 0) const;
		size_t getSize() const;
		
	protected:
		
		const NodeArray& filter(const NodeArray& src);
		
	private:
		
		string bus_name;
	};
	
	//
	
	template <typename FilterClass>
	class FilterEach
	{
	public:
		
		void clear()
		{
			cache.clear();
			filters.clear();
		}
		
		const vector<NodeArray>& update(const vector<NodeArray> &src)
		{
			if (ofGetFrameNum() == last_updated_time) return cache;
			
			last_updated_time = ofGetFrameNum();
			
			cache.clear();
			
			for (int i = 0; i < src.size(); i++)
			{
				const NodeArray &NA = src[i];
				const string &name = NA.getName();
				
				if (!filters.hasKey(name))
					filters.set(name, FilterClass());
				
				FilterClass &filter = filters[name];
				cache.push_back(filter.update(NA));
			}
			
			return cache;
		}
		
		const vector<NodeArray>& get(size_t index = 0) const { return cache[index]; }
		const size_t getNumFilters() const { return filters.size(); }
		
		FilterClass& getFilter(int index) { return filters[index]; }
		const FilterClass& getFilter(int index) const { return filters[index]; }
		FilterClass& getFilter(const string& name) { return filters[name]; }
		const FilterClass& getFilter(const string& name) const { return filters[name]; }
		
	protected:
		
		CompoundContainer<FilterClass> filters;
		vector<NodeArray> cache;
		
		unsigned int last_updated_time;
		
	};
}

using RAMDEPRECATED(ramBaseFilter)	= rdtk::BaseFilter;
using RAMDEPRECATED(ramBusSend)		= rdtk::BusSend;
template <typename FilterClass>
using RAMDEPRECATED(ramFilterEach) = rdtk::FilterEach<FilterClass>;
