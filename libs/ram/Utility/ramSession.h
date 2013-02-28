#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
#include "ramGlobal.h"
#include "ramNodeArrayBuffer.h"
#include "ramBaseFilter.h"


class ramSession : public ramBaseFilter
{

public:
	
	const string getName() { return "ramSession"; }
	
	ramSession() {}
	ramSession(const ramSession &copy) { *this = copy; }
	ramSession(const ramNodeArrayBuffer &buf) { mBuffer = buf; }
	~ramSession() {}

	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();

#ifdef RAM_GUI_SYSTEM_OFXUI

		ofxUICanvas* panel = gui.getCurrentUIContext();

		panel->addToggle("Rec", false, 40, 40);
		panel->addToggle("Play", false, 40, 40);
		panel->addToggle("Loop", true, 40, 40);

		ofAddListener(panel->newGUIEvent, this, &ramSession::onPanelChanged);

#endif
	}

	void clear()
	{
		mBuffer.clear();
		
		mNodeArrayName.clear();
		
		mPlayhead = 0;
		mDuration = 0;
		mFrameTime = 0;
		mPlayStartTime = 0;
		mRecStartTime = 0;

		mFrameTime = 0;
		mNumFrames = 0;
		mRate = 1.0;
		
		mLoop = true;
		mRecording = false;
		mPlaying = false;
	}

	int getFrameIndex()
	{
		return floor(mPlayhead / mFrameTime);
	}

	void setup()
	{
		clear();
	}

	void onPanelChanged(ofxUIEventArgs& e)
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
	
	const ramNodeArray& filter(const ramNodeArray &src)
	{
		if (isRecording())
		{
			mBuffer.add(src);
		}
		
		if (isPlaying())
		{
			mPlayhead = (ofGetElapsedTimef() - mPlayStartTime) * mRate;
			mFrameIndex = getFrameIndex();
			
			if (mFrameIndex >= mNumFrames)
			{
				mFrameIndex = 0;
				
				if (isLoop())
				{
					mPlayStartTime = ofGetElapsedTimef();
				}
				else
				{
					stop();
				}
			}
			
			return mBuffer.get(mFrameIndex);
		}
		else
		{
			return src;
		}
		
		return src;
	}

#pragma mark - recorder handling

	void startRecording()
	{
		if (isRecording()) return;

		cout << "recording start " << mNodeArrayName << "." << endl;

		clear();
		mPlaying = false;
		mRecording = true;
		mRecStartTime = ofGetElapsedTimef();
	}

	void stopRecording()
	{
		if (!isRecording() || mBuffer.getSize() <= 0) return;
		
		mRecording = false;
		mNodeArrayName = mBuffer.get(0).getName();
		mDuration = ofGetElapsedTimef() - mRecStartTime;
		mNumFrames = mBuffer.getSize();
		mFrameTime = mDuration / mNumFrames;
		
		cout
		<< "Recording finish. ["
		<< mNodeArrayName
		<< "] Total:" << mBuffer.getSize() << " frames." << endl;
	}

	void play()
	{
		if (mNumFrames <= 0) return;

		cout << "start playing " << mNodeArrayName << "." << endl;

		mRecording = false;
		mPlaying = true;
		mPlayStartTime = ofGetElapsedTimef();
	}

	void stop()
	{
		if (!isPlaying()) return;
		
		cout << "stop playing " << mNodeArrayName << "." << endl;

		mPlaying = false;
	}
	
	void setNodeArrayBuffer(ramNodeArrayBuffer &buffer)
	{
		if (buffer.getSize() <= 0)
		{
			ofLogError("RAMDanceToolkit::ramSession") << "invalid buffer was passed to setNodeArrayBuffer";
			return;
		}
		
		clear();
		mBuffer = buffer;
	}
	
	inline void setLoop(bool l) { mLoop = l; };
	inline void setRate(const float r) { mRate = r; };
	inline void setPlayhead(const float t) { mPlayhead = t; };

	inline bool isPlaying() { return mPlaying; }
	inline bool isRecording() { return mRecording; }
	inline bool isLoop() { return mLoop; }

	inline ramNodeArray& getNextFrame() { return mBuffer.get(mFrameIndex); }

	inline float getDuration() const {return mDuration;}
	inline float getPlayhead() const {return mPlayhead;}
	inline string getNodeArrayName() const {return mNodeArrayName;}
	inline int getNumFrames() const { return mNumFrames; }
	
	
protected:

	ramNodeArrayBuffer mBuffer;
	
	string mNodeArrayName;
	
	bool mLoop;
	float mRate;

	bool mRecording;
	bool mPlaying;

	float mPlayhead;
	float mDuration;
	float mFrameTime;
	float mPlayStartTime;
	float mRecStartTime;

	int mFrameIndex;
	int mNumFrames;
};
