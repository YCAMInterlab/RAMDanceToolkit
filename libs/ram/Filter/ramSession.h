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
	const string getName();
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
	void appendFrame(const ramNodeArray copy);
	const ramNodeArray& getFrame(int index) const;
	const ramNodeArray& getCurrentFrame() const;
	
	
#pragma mark -
#pragma mark getters, setters
	const int getFrameIndex() const;
	const int getNumFrames() const;
	const float getFrameTime() const;
	const float getDuration() const;
	const float getPlayhead() const;
	const string getNodeArrayName() const;
	
	void setLoop(const bool l);
	void setRate(const float r);
	void setPlayhead(const float t);
	
	const bool isPlaying() const;
	const bool isRecording() const;
	const bool isLoop() const;
	
	void setNodeArrayBuffer(ramNodeArrayBuffer &buffer);
	
	
protected:
	
	ramNodeArrayBuffer mBuffer;
	
	bool mLoop;
	float mRate;
	
	bool mRecording;
	bool mPlaying;
	
	float mPlayhead;
	float mPlayStartTime;
	float mRecStartTime;
	float mRecEndTime;
};
