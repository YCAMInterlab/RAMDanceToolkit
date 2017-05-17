// 
// ramTimeFilter.h - RAMDanceToolkit
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

#include "ramNodeArrayBuffer.h"
#include "ramBaseFilter.h"

namespace rdtk{
	class Delay : public BaseFilter
	{
	public:
		
		string getName() const { return "ramDelay"; }
		
		Delay(size_t delay_frame = 60) : delay_frame(delay_frame) {}
		
		void setFrame(size_t num) { delay_frame = num; if (delay_frame < 1) delay_frame = 1; };
		size_t getFrame() const { return delay_frame; }
		
		size_t getSize() const { return buffer.getSize(); }
		
		void setupControlPanel()
		{
			GetGUI().addSection(getName());
			GetGUI().addSlider("Delay", 1, 1000, &delay_frame);
		}
		
	protected:
		
		NodeArrayBuffer buffer;
		
		float delay_frame;
		
		const NodeArray& filter(const NodeArray& src)
		{
			buffer.setCapacity(delay_frame);
			buffer.add(src);
			return buffer.get(delay_frame);
		}
	};
	
	class TimeShifter : public BaseFilter
	{
	public:
		
		string getName() const { return "ramTimeShifter"; }
		
		TimeShifter(size_t buffer_frame = 300) : rate(1), play_head(0), buffer_frame(buffer_frame)
		{
			setNumBufferFrame(buffer_frame);
		}
		
		void setNumBufferFrame(size_t buffer_frame) { this->buffer_frame = buffer_frame; }
		size_t getNumBufferFrame() const { return buffer_frame; }
		
		void setRate(float rate) { this->rate = rate; }
		float getRate() const { return rate; }
		
		void clear() { buffer.clear(); }
		
		void setupControlPanel()
		{
			GetGUI().addSection(getName());
			GetGUI().addSlider("Rate", -3, 3, &rate);
			GetGUI().addSlider("Buffer Frame", 1, 1000, &buffer_frame);
		}
		
	protected:
		
		NodeArrayBuffer buffer;
		
		float rate;
		float play_head;
		float buffer_frame;
		
		const NodeArray& filter(const NodeArray& src)
		{
			buffer.setCapacity(buffer_frame);
			
			buffer.add(src);
			
			play_head += (60. / buffer.getSize()) / 60. * (-rate + 1);
			while (play_head > 1) play_head -= 1;
			while (play_head <= 0) play_head += 1;
			
			return buffer.get(((int)buffer.getSize() - 1) * play_head);
		}
		
	};
}

typedef rdtk::Delay		RAMDEPRECATED(ramDelay);
typedef rdtk::TimeShifter	RAMDEPRECATED(ramTimeShifter);
