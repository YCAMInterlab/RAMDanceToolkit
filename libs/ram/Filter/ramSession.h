#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
//#include "ofxXmlSettings.h"


class ramSession : public ramBaseFilter
{
	
public:
	
	ramSession() : mLoop(true), mRecording(false), mPlaying(false) {}
	~ramSession() {}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		gui.addSection(getName());
		
//		gui.addToggle("Rec", )
		
		ofxUICanvas* panel = gui.getCurrentUIContext();
		
		panel->addToggle("Rec", false, 40, 40);
		panel->addToggle("Play", false, 40, 40);
		panel->addToggle("Loop", true, 40, 40);
		
		ofAddListener(panel->newGUIEvent, this, &ramSession::onPanelChanged);
		
#endif
	}
	
	void clear()
	{
		mNodeArray.clear();
		
		mPlayhead = 0;
		mDuration = 0;
		mFrameTime = 0;
		mPlayStartTime = 0;
		mRecStartTime = 0;
		
		mFrameTime = 0;
		mNumFrames = 0;
		
		mRate = 1.0;
	}
	
	int getFrameIndex()
	{
		return floor(mPlayhead / mFrameTime);
	}
	
	void setup()
	{
		clear();
	}
	
	const ramNodeArray& update(const ramNodeArray &src)
	{
		if (isRecording())
		{
			appendFrame(src);
		}
		
		if (isPlaying())
		{
			mPlayhead = (ofGetElapsedTimef() - mPlayStartTime) * mRate;
			mFrameIndex = getFrameIndex();
			
			if(mFrameIndex >= mNumFrames)
			{
				mFrameIndex = 0;
				
				if (isLoop())
				{
					mPlayStartTime = ofGetElapsedTimef();
				}
				else
				{
					mPlaying = false;
				}
			}
			
			return mNodeArray.at(mFrameIndex);
		}
		else
		{
			return src;
		}
		
		return src;
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Rec")
		{
			ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
			
			if (toggle->getValue())
				startRecording();
			
			else
				stopRecording();
		}
		
		if (name == "Play")
		{
			ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
			
			if (toggle->getValue())
				play();
			
			else
				stop();
		}
		
		if (name == "Loop")
		{
			ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
			setLoop(toggle->getValue());
		}
	}
	
	const string getName() { return "ramSession"; }
	
	
	
#pragma mark - recorder handling
	
	
	
	void startRecording()
	{
		if (isRecording()) return;
		
		cout << "recording start." << endl;
		
		clear();
		mPlaying = false;
		mRecording = true;
		mRecStartTime = ofGetElapsedTimef();
	}
	
	void stopRecording()
	{
		if (!isRecording()) return;
		
		cout
		<< "recording finish. ["
		<< mNodeArray.at(0).getName()
		<< "] Total:" << mNodeArray.size() << " frames." << endl;
		
		mRecording = false;
		
		mDuration = ofGetElapsedTimef() - mRecStartTime;
		mNumFrames = mNodeArray.size();
		mFrameTime = mDuration / mNumFrames;
	}
	
	void play()
	{
		if (mNumFrames <= 0) return;
		
		cout << "start playing." << endl;
		
		mRecording = false;
		mPlaying = true;
		mPlayStartTime = ofGetElapsedTimef();
	}
	
	void stop()
	{
		if (!isPlaying()) return;
		
		cout << "stop playing." << endl;
		
		mPlaying = false;
	}

	void appendFrame(ramNodeArray copy)
	{
		mNodeArray.push_back(copy);
	}
	
	void load(const string path)
	{
		
	}
	
	void save(const string path)
	{
		
	}

	inline void setLoop(bool l) {mLoop = l;};
	inline void setRate(const float r) {mRate = r;};
	inline void setPlayhead(const float t){mPlayhead = t;};
	
	inline bool isPlaying() { return mPlaying; }
	inline bool isRecording() { return mRecording; }
	inline bool isLoop() { return mLoop; }
	
	inline ramNodeArray& getNextFrame() { return mNodeArray.at(mFrameIndex); }
	
//	inline float getDuration() {return duration;}
//	inline float getPlayhead() {return playhead;}
//	inline string getSessionName() {return sessionName;}
	
//	inline ramActor& getNextFrameActor() {return actors.at(frame_index);}
//	inline ramRigidBody& getNextFrameRigid() {return rigids.at(frame_index);}
	
	
protected:
	
	vector<ramNodeArray> mNodeArray;
	
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

