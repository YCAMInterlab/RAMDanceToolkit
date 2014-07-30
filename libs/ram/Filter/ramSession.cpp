// 
// ramSession.cpp - RAMDanceToolkit
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

#include "ramSession.h"

#include "ramControlPanel.h"

#pragma mark -
#pragma mark constructor

ramSession::ramSession() { clear(); }
ramSession::ramSession(const ramSession &copy) { clear(); *this = copy; }
ramSession::ramSession(const ramNodeArrayBuffer &buf) { clear(); mBuffer = buf; }


#pragma mark -
#pragma mark gui settings

void ramSession::setupControlPanel()
{

#ifdef RAM_GUI_SYSTEM_OFXUI

	ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();

	panel->addToggle("Rec", false, 40, 40);
	panel->addToggle("Play", false, 40, 40);
	panel->addToggle("Loop", true, 40, 40);

	ofAddListener(panel->newGUIEvent, this, &ramSession::onPanelChanged);

#endif
}

void ramSession::onPanelChanged(ofxUIEventArgs& e)
{
	string name = e.widget->getName();
	
	if (name == "Rec")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		
		if (toggle->getValue())
			startRecording();
		
		else
			stopRecording();
	}
	
	if (name == "Play")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		
		if (toggle->getValue())
			play();
		
		else
			stop();
	}
	
	if (name == "Loop")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		setLoop(toggle->getValue());
	}
}



#pragma mark -
#pragma mark override ramBaseFilter

const ramNodeArray& ramSession::filter(const ramNodeArray &src)
{
	if (isRecording())
	{
		appendFrame(src);
	}
	
	if (isPlaying())
	{
		updatePlayhead();
		return getCurrentFrame();
	}
	
	return src;
}

string ramSession::getName() const
{
	return "ramSession";
}



#pragma mark -
#pragma mark session handling

void ramSession::startRecording()
{
	if (isRecording()) return;

	clear();
	mPlaying = false;
	mRecording = true;
	mRecStartTime = ofGetElapsedTimef();
	
	cout << "recording start." << endl;
}

void ramSession::stopRecording()
{
	if (!isRecording() || mBuffer.getSize() <= 0) return;
	
	mRecording = false;
	mRecEndTime = ofGetElapsedTimef();
	
	cout << "Recording finished." << endl;
	cout << "Actor: " << getNodeArrayName() << endl;
	cout << "Duration: " << getDuration() << " sec"<< endl;
	cout << "Frames: " << getNumFrames() << endl;
}

void ramSession::play()
{
	if (mCurrentFrame.getNumNode() == 0)
	{
		ofLogError("ramSession") << "You should call ramSession::prepareForPlay() before calling ramSession::play()";
		return;
	}
	if (getNumFrames() <= 0)
	{
		ofLogError("ramSession") << "session data is empty...";
		 return;
	}

	mRecording = false;
	mPlaying = true;
	
//	cout << "start playing " << getNodeArrayName() << "." << endl;
}

void ramSession::stop()
{
	if (!isPlaying()) return;

	mPlaying = false;
	
//	cout << "stop playing " << getNodeArrayName() << "." << endl;
}


// --

void ramSession::prepareForPlay()
{
	assert( getNumFrames() > 0 );
	
	mCurrentFrame = mBuffer.get(0);
}

void ramSession::updatePlayhead()
{
	if (!isPlaying()) return;
	
	mPlayhead += ofGetLastFrameTime() * mRate;
	
	bool wrapped = false;
	
	if (mPlayhead > getDuration())
	{
		mPlayhead = 0;
		wrapped = true;
	}
	
	if (mPlayhead < 0)
	{
		mPlayhead = getDuration();
		wrapped = true;
	}
	
	if (wrapped)
	{
		if (!isLoop())
		{
			stop();
		}
	}
}


// --

void ramSession::clear()
{
	mBuffer.clear();
	
	mPlayhead = 0;
	mRecStartTime = 0;
	mRecEndTime = 0;
	
	mRate = 1.0;
	mLoop = true;
	
	mRecording = false;
	mPlaying = false;
}

void ramSession::appendFrame(const ramNodeArray& copy)
{
	mBuffer.append(copy);
}

ramNodeArray& ramSession::getFrame(int index)
{
	mCurrentFrame = mBuffer.get(index);
	
	for (int i=0; i<mCurrentFrame.getNumNode(); i++)
	{
		const ofVec3f pos = mCurrentFrame.getNode(i).getGlobalPosition();
		const ofQuaternion quat = mCurrentFrame.getNode(i).getGlobalOrientation();
		
		ramNode &node = mCurrentFrame.getNode(i);
		node.setGlobalPosition(pos);
		node.setGlobalOrientation(quat);
		node.getAccelerometer().update(pos, quat);
	}
	
	return mCurrentFrame;
}

ramNodeArray& ramSession::getCurrentFrame()
{
	return getFrame(getCurrentFrameIndex());
}



#pragma mark -
#pragma mark getters, setters

void ramSession::setFreeze(const bool playing) { mPlaying = !playing; };
void ramSession::setLoop(const bool l) { mLoop = l; };
void ramSession::setRate(const float r) { mRate = r; };
void ramSession::setPlayhead(const float t) { mPlayhead = t; };


bool ramSession::isPlaying() { return mPlaying; }
bool ramSession::isRecording() { return mRecording; }
bool ramSession::isLoop() { return mLoop; }

float ramSession::getPlayhead()
{
	return mPlayhead;
}

int ramSession::getCurrentFrameIndex()
{
	return floor(mPlayhead / getAverageFrameTime());
//	return mPlayhead * (getNumFrames() - 1);
}

int ramSession::getNumFrames()
{
	return mBuffer.getSize();
}

float ramSession::getAverageFrameTime()
{
	return getDuration() / getNumFrames();
}

float ramSession::getDuration()
{
	ramNodeArray &frontFrame = mBuffer.get( 0 );
	ramNodeArray &backFrame = mBuffer.get( getNumFrames() );
	
	return backFrame.getTimestamp() - frontFrame.getTimestamp();
}

string ramSession::getNodeArrayName()
{
	return getNumFrames() > 0 ? mBuffer.get(0).getName() : "no name";
}

void ramSession::setNodeArrayBuffer(ramNodeArrayBuffer &buffer)
{
	if (buffer.getSize() <= 0)
	{
		ofLogError("RAMDanceToolkit::ramSession") << "invalid buffer was passed to setNodeArrayBuffer";
		return;
	}
	
	clear();
	mBuffer = buffer;
}

