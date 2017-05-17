// 
// ramLowPassFilter.h - RAMDanceToolkit
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


namespace rdtk{
	class LowPassFilter : public BaseFilter
	{
	public:
		
		LowPassFilter() : amount(0.1) {}
		
		const NodeArray& get(size_t index = 0) const { return copy; }
		size_t getSize() const { return 1; }
		
		void setupControlPanel()
		{
			GetGUI().addSlider("LowPass amount", 0.0, 1.0, &amount);
		}
		
		string getName() const { return "ramLowPassFilter"; }
		
#pragma mark -
		
		void setAmount(float a) { amount = a; }
		
		const NodeArray& filter(const NodeArray& src)
		{
			if (src.getNumNode() != copy.getNumNode()) copy = src;
			
			for (int i = 0; i < src.getNumNode(); i++)
			{
				ofVec3f input = src.getNode(i).getGlobalPosition();
				ofVec3f output = copy.getNode(i).getGlobalPosition();
				copy.getNode(i).setGlobalPosition((input * amount) + (output * (1 - amount)));
			}
			
			return copy;
		}
		
	protected:
		
		NodeArray copy;
		float amount;
	};
}

typedef rdtk::LowPassFilter RAMDEPRECATED(ramLowPassFilter);
