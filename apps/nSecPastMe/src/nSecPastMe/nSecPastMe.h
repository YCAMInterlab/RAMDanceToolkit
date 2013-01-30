#pragma once


static float REC_SPAN = 15.0;

class nSecPastMe : public ramSceneBase
{
	
	ramSession pastMe[2];
    float lastRecordTime;
	int curRecIndex, curPlayingIndex;
	
public:
	
	nSecPastMe()
	{
		setSceneName("nSec Past Me");
	}
	
	
	void setup()
	{
		lastRecordTime = ofGetElapsedTimef() + REC_SPAN;
		
		pastMe[0].setup(myActorName, ramSession::RAM_ACTOR);
		pastMe[1].setup(myActorName, ramSession::RAM_ACTOR);
		
		curRecIndex = 0;
		curPlayingIndex = 1;
		pastMe[curRecIndex].startRecording();
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		
		guiPtr = &gui;
	}
	
	void update()
	{
		if ( getActorManager().getNumActor() > 0 )
		{
			if ( ofGetElapsedTimef() > lastRecordTime )
			{
				lastRecordTime += REC_SPAN;
				
				// stop rec & start playing
				pastMe[curRecIndex].stopRecording();
				pastMe[curRecIndex].play();
				
				
				// stop playing & start recording
				pastMe[curPlayingIndex].startRecording();
				
				// refresh indexes
				curRecIndex ^= 1;
				curPlayingIndex ^= 1;
				
			}
		}
		
		pastMe[0].update();
		pastMe[1].update();
	}
	
	void draw()
	{
		ramCameraBegin();
		
		if (pastMe[curPlayingIndex].isPlaying())
		{
			ramActor& actor = pastMe[curPlayingIndex].getNextFrameActor();
			ramBasicActor(actor, ramColor::BLACK);
		}
		ramCameraEnd();
	}
	
	void drawActor( ramActor& actor )
	{

	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
};

