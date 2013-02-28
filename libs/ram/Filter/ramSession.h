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
	
	ramSession() { clear(); }
	ramSession(const ramSession &copy) { clear(); *this = copy; }
	ramSession(const ramNodeArrayBuffer &buf) { clear(); mBuffer = buf; }
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
		mPlayStartTime = 0;
		mRecStartTime = 0;
		mRecEndTime = 0;
		
		mRate = 1.0;
		mLoop = true;
		mRecording = false;
		mPlaying = false;
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
			appendFrame(src);
		}
		
		if (isPlaying())
		{
			mPlayhead = (ofGetElapsedTimef() - mPlayStartTime) * mRate;
			
			if (getFrameIndex() <= 0)
			{
//				mFrameIndex = getNumFrames();
				
				if (isLoop())
				{
					mPlayStartTime = ofGetElapsedTimef();
				}
				else
				{
					stop();
				}
			}
			
			return mBuffer.get(getFrameIndex());
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
		mRecEndTime = ofGetElapsedTimef();
		
		mNodeArrayName = mBuffer.get(0).getName();
		
		
		cout << mNodeArrayName << " Recording finished." << endl;
		cout << "Duration: " << getDuration() << "sec"<< endl;
		cout << "Frames: " << getNumFrames() << endl;
	}

	void play()
	{
		if (getNumFrames() <= 0) return;

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
	
	const ramNodeArray& getFrame(int index) const
	{
		if (index > getNumFrames()) index = getNumFrames();
		return mBuffer.get(index);
	}
	void appendFrame(const ramNodeArray copy)
	{
		mBuffer.add(copy);
	}
	
	inline void setLoop(const bool l) { mLoop = l; };
	inline void setRate(const float r) { mRate = r; };
	inline void setPlayhead(const float t) { mPlayhead = t; };

	inline const bool isPlaying() const { return mPlaying; }
	inline const bool isRecording() const { return mRecording; }
	inline const bool isLoop() const { return mLoop; }
	
	inline const float getPlayhead() const {return mPlayhead;}
	
	const int getFrameIndex() const
	{
		return getNumFrames() - floor(mPlayhead / getFrameTime());
	}
	
	const int getNumFrames() const
	{
		return mBuffer.getSize();
	}
	
	const float getFrameTime() const
	{
		return getDuration() / getNumFrames();
	}
	
	const float getDuration() const
	{
		assert(getNumFrames() > 1);
		
		const ramNodeArray &frontFrame = mBuffer.get( 0 );
		const ramNodeArray &backFrame = mBuffer.get( getNumFrames() );
		
		return backFrame.getTimestamp() - frontFrame.getTimestamp();
	}
	
	const string getNodeArrayName() const
	{
		assert(getNumFrames() > 0);
		
		return getNumFrames() > 0 ? mBuffer.get(0).getName() : "no name";
	}
	
protected:

	ramNodeArrayBuffer mBuffer;
	
	string mNodeArrayName;
	
	bool mLoop;
	float mRate;
	
	bool mRecording;
	bool mPlaying;
	
	float mPlayhead;
	float mPlayStartTime;
	float mRecStartTime;
	float mRecEndTime;
};
