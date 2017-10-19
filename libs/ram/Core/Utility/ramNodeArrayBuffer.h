// 
// ramNodeArrayBuffer.h - RAMDanceToolkit
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

#include "ramActor.h"

namespace rdtk{
	
	class NodeArrayBuffer
	{
	public:
		
		NodeArrayBuffer() : capacity(100000) {}
		
		inline void add(const NodeArray& arr)
		{
			prepend(arr);
		}
		
		void prepend(const NodeArray& arr)
		{
			buffer.push_front(arr);
			if (buffer.size() > capacity)
				buffer.pop_back();
		}
		
		void append(const NodeArray& arr)
		{
            buffer.push_back(arr);
			if (buffer.size() > capacity)
				buffer.pop_front();
		}
		
		void clear()
		{
			buffer.clear();
		}
		
		void setCapacity(size_t capacity)
		{
			assert(capacity >= 0);
			this->capacity = capacity;
			while (buffer.size() > capacity)
				buffer.pop_back();
		}
		
		size_t getCapacity() const { return capacity; }
		
		// TODO: return linear interpolated nodeArray
		
		NodeArray& get(size_t index)
		{
			if (index >= buffer.size()) return buffer.back();
			return buffer.at(index);
		}
		
		inline const NodeArray& get(size_t index) const
		{
			if (index >= buffer.size()) return buffer.back();
			return buffer.at(index);
			
		}
		
		size_t getSize() const { return buffer.size(); }
		
	private:
		
		size_t capacity;
		deque<NodeArray> buffer;
	};
}

typedef rdtk::NodeArrayBuffer RAMDEPRECATED(ramNodeArrayBuffer);
