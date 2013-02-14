#pragma once

static float REC_SPAN = 2.0;

class nSecPastMe : public ramBaseScene
{
	
	ramSession pastMe[2];
    float lastRecordTime;
	int curRecIndex, curPlayingIndex;
	
public:
	
	nSecPastMe() {}
	
	
	void setup()
	{
		lastRecordTime = ofGetElapsedTimef() + REC_SPAN;
		
		pastMe[0].setup(myActorName, ramSession::RAM_ACTOR);
		pastMe[1].setup(myActorName, ramSession::RAM_ACTOR);
		
		curRecIndex = 0;
		curPlayingIndex = 1;
		pastMe[curRecIndex].startRecording();
	}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
//		
//		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
//		panel->addSpacer(gui.kLength, 2);
//		panel->addSlider("Font Color", 0.0, 255.0, &fontColor, gui.kLength, gui.kDim);
//		
//		ofAddListener(panel->newGUIEvent, this, &BigBox::onPanelChanged);
	}
	
	void update()
	{
		if ( getActorManager().getNumActor() > 0 )
		{
			if ( ofGetElapsedTimef() >= lastRecordTime )
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
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
	}
	
	const string getName() { return "My scene"; }
};

