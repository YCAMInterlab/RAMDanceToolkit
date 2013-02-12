#pragma once




class nSecPastMe : public ramSceneBase
{
	
	ramSession pastMe[2];
    float lastRecordTime;
	int curRecIndex, curPlayingIndex;
	float recSpan;
	
public:

	
	inline void setRecSpan(float span) { recSpan = span; }
	inline float getRecSpan() { return recSpan; }
	
	
	nSecPastMe() : recSpan(10.0)
	{
		setSceneName("nSec Past Me");
	}
	
	
	void setup()
	{
		lastRecordTime = ofGetElapsedTimef() + getRecSpan();
		
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
				lastRecordTime += getRecSpan();
				
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
		ramBeginCamera();
		
		if (pastMe[curPlayingIndex].isPlaying())
		{
			ramActor& actor = pastMe[curPlayingIndex].getNextFrameActor();
			ramBasicActor(actor, ramColor::BLACK);
		}
		ramEndCamera();
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

