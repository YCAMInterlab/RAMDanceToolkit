#pragma once

#include "ramBaseFilter.h"
#include "ramControlPanel.h"
#include "ramSession.h"

class ramPastMe : public ramBaseFilter
{
	float mRecSpan;
    float mLastRecordTime;
	int mCurRecIndex, mCurPlayingIndex;
	
	ramSession pastMe[2];
	
public:
	
	ramPastMe() : mRecSpan(5.0), mLastRecordTime(0.0), mCurRecIndex(0), mCurPlayingIndex(1)
	{
	}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
	}
	
	void setup()
	{
		mLastRecordTime = ofGetElapsedTimef() + mRecSpan;
		
		mCurRecIndex = 0;
		mCurPlayingIndex = 1;
		
		pastMe[0].setup(myActorName, ramSession::RAM_ACTOR);
		pastMe[1].setup(myActorName, ramSession::RAM_ACTOR);
		pastMe[mCurRecIndex].startRecording();
	}
	
	const ramNodeArray& update(const ramNodeArray &present)
	{
		pastMe = present;
		
		if ( getActorManager().getNumActor() > 0 )
		{
			if ( ofGetElapsedTimef() >= mLastRecordTime )
			{
				mLastRecordTime += mRecSpan;
				
				// stop rec & start playing
				pastMe[mCurRecIndex].stopRecording();
				pastMe[mCurRecIndex].play();
				
				
				// stop playing & start recording
				pastMe[mCurPlayingIndex].startRecording();
				
				// refresh indexes
				mCurPlayingIndex ^= 1;
				mCurPlayingIndex ^= 1;
			}
		}
		
		pastMe[0].update();
		pastMe[1].update();
		
		return pastMe;
	}
	
//	inline const ramNodeArray& getResult() { return ghost; }
	inline const string getName() { return "ramPastMe"; };

protected:

	ramNodeArray pastMe;
};