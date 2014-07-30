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


class ramSession : public ramBaseFilter
{

public:
	
	
#pragma mark -
#pragma mark constructor
	ramSession();
	ramSession(const ramSession &copy);
	ramSession(const ramNodeArrayBuffer &buf);
	
	
#pragma mark -
#pragma mark gui settings
	void setupControlPanel();
	void onPanelChanged(ofxUIEventArgs& e);
	
	
#pragma mark -
#pragma mark override ramBaseFilter
	string getName() const;
	const ramNodeArray& filter(const ramNodeArray &src);

	
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
	void appendFrame(const ramNodeArray& copy);
	ramNodeArray& getFrame(int index);
	ramNodeArray& getCurrentFrame();
	
	
#pragma mark -
#pragma mark getters, setters
	int getCurrentFrameIndex();
	int getNumFrames();
	float getAverageFrameTime();
	float getDuration();
	float getPlayhead();
	string getNodeArrayName();
	
	void setLoop(const bool l);
	void setRate(const float r);
	void setPlayhead(const float t);
	void setFreeze(const bool playing);
	
	bool isPlaying();
	bool isRecording();
	bool isLoop();
	
	void setNodeArrayBuffer(ramNodeArrayBuffer &buffer);
	
	
protected:
	
	ramNodeArrayBuffer mBuffer;
	
	bool mLoop;
	float mRate;
	
	bool mRecording;
	bool mPlaying;
	
	float mPlayhead;
	float mRecStartTime;
	float mRecEndTime;
	
	ramNodeArray mCurrentFrame;
};
