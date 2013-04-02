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
};
