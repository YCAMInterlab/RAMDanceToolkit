// 
// ramSession.h - RAMDanceToolkit
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

#include "ofxUI.h"
#include "ramActorManager.h"
#include "ramActor.h"
#include "ramGlobal.h"
#include "ramNodeArrayBuffer.h"
#include "ramBaseFilter.h"

namespace rdtk{
	class Session : public BaseFilter
	{
		
	public:
		
		
#pragma mark -
#pragma mark constructor
		Session();
		Session(const Session &copy);
		Session(const NodeArrayBuffer &buf);
		
		
#pragma mark -
#pragma mark gui settings
		void setupControlPanel();
		void onPanelChanged(ofxUIEventArgs& e);
		
		
#pragma mark -
#pragma mark override ramBaseFilter
		string getName() const;
		const NodeArray& filter(const NodeArray &src);
		
		
#pragma mark -
#pragma mark session handling
		void startRecording();
		void stopRecording();
		void play();
		void stop();
		
		// --
		
		void prepareForPlay();
		void updatePlayhead();
		
		// --
		
		void clear();
		void appendFrame(const NodeArray& copy);
		NodeArray& getFrame(int index);
		inline const NodeArray& getFrame(int index) const {
			return const_cast<NodeArray &>(getFrame(index));
		}
		NodeArray& getCurrentFrame();
		inline const NodeArray& getCurrentFrame() const {
			return const_cast<NodeArray &>(getCurrentFrame());
		}
		
#pragma mark -
#pragma mark getters, setters
		int getCurrentFrameIndex() const;
		int getNumFrames() const;
		float getAverageFrameTime() const;
		float getDuration() const;
		float getPlayhead() const;
		string getNodeArrayName() const;
		
		void setLoop(const bool l);
		void setRate(const float r);
		void setPlayhead(const float t);
		void setFreeze(const bool playing);
		
		bool isPlaying();
		bool isRecording();
		bool isLoop();
		
		void setNodeArrayBuffer(NodeArrayBuffer &buffer);
		
	protected:
		
		NodeArrayBuffer mBuffer;
		
		bool mLoop;
		float mRate;
		
		bool mRecording;
		bool mPlaying;
		
		float mPlayhead;
		float mRecStartTime;
		float mRecEndTime;
		
		NodeArray mCurrentFrame;
	};

}

typedef rdtk::Session RAMDEPRECATED(ramSession);

