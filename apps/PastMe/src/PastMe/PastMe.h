#pragma once


static float REC_SPAN = 3.0;

class PastMe : public ramSceneBase
{
	
	string key_toggle, key_box, key_actor;
	
	deque<ramActor> pastMe;
    float lastRecordTime;
	bool showBox, showActor;
	
public:
	
	PastMe()
	{
		setSceneName("Past Me");
		
		// ---
		
		key_toggle = "Clear";
		key_box = "show box";
		key_actor = "show actor";
		
		showBox = true;
		showActor = false;
	}
	
	
	void setup()
	{
		lastRecordTime = ofGetElapsedTimef() + REC_SPAN;
		pastMe.clear();
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		gui.addToggle( key_toggle );
		gui.addToggle( key_box, showBox );
		gui.addToggle( key_actor, showActor );
		
		guiPtr = &gui;
	}
	
	void update()
	{
		
		if ( getActorManager().getNumActor() > 0 )
		{
			if ( ofGetElapsedTimef() > lastRecordTime )
			{
				lastRecordTime += REC_SPAN;
				pastMe.push_back( getActor( myActorName ) );
			}
		}
		
		if( guiPtr->hasValueChanged( key_toggle ) )
		{
			pastMe.clear();
		}
		
		if( guiPtr->hasValueChanged( key_actor ) )
		{
			showActor = guiPtr->getValueB( key_actor );
		}
		
		if( guiPtr->hasValueChanged( key_box ) )
		{
			showBox = guiPtr->getValueB( key_box );
		}
		
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		if (!bEnabled) return;
		
		
		ofPushStyle();
		for ( int i=0; i<pastMe.size(); i++ )
		{
			ramActor &skl = pastMe.at( i );
			
			if (showActor)
				ramBasicActor(skl, ramColor::BLUE_DEEP, ramColor::BLUE_DEEP, getMatrix().getPtr());
			
			if (showBox)
				ramActorCube(skl);
		}
		
		ofPopStyle();
		
		
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
};

