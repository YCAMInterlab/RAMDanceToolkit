// 
// ramStamp.h - RAMDanceToolkit
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
	class Stamp : public BaseFilter
	{
		
		deque<NodeArray> mStamps;
		float mLastRecordTime;
		float mRecSpan;
		
		int kMaxStamps;
		
	public:
		
		Stamp() : mLastRecordTime(0.0), mRecSpan(5.0), kMaxStamps(30) {}
		
		void setupControlPanel()
		{
			ofAddListener(GetGUI().addButton("Clear"), this, &Stamp::onClear);
			GetGUI().addSlider("Recording Span", 2.0, 60.0, &mRecSpan);
		}
		
		void drawImGui()
		{
			if (ImGui::Button("Clear")) clear();
			ImGui::DragFloat("Recording Span", &mRecSpan, 0.5, 2, 60);
		}
		
		void onClear(ofEventArgs &e)
		{
			clear();
		}
		
		void setup()
		{
			clear();
		}
		
#pragma mark -
		
		const NodeArray& get(size_t index = 0) const { return mStamps[index]; }
		size_t getSize() const { return mStamps.size(); }
		
		void clear()
		{
			mLastRecordTime = ofGetElapsedTimef();
			mStamps.clear();
		}
		
		const NodeArray createStamp(const NodeArray& src)
		{
			mLastRecordTime = ofGetElapsedTimef();
			NodeArray copy = src;
			mStamps.push_back(copy);
			
			return copy;
		}
		
		inline void setRecSpan(const float span) { mRecSpan = span; }
		inline deque<NodeArray>& getStamps() { return mStamps; }
		inline const deque<NodeArray>& getStamps() const { return mStamps; }
		inline NodeArray& getStamp(const int index) { return mStamps.at(index); }
		inline const NodeArray& getStamp(const int index) const { return mStamps.at(index); }
		
		string getName() const { return "ramStamp"; }
		
	protected:
		
		const NodeArray& filter(const NodeArray& src)
		{
			if (ofGetElapsedTimef() - mLastRecordTime > mRecSpan)
			{
				createStamp(src);
				if (kMaxStamps < mStamps.size())
					mStamps.pop_front();
			}
			
			return src;
		}
	};
}

typedef rdtk::Stamp RAMDEPRECATED(ramStamp);
