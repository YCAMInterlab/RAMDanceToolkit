#pragma once

class Stamp : public ramBaseScene
{
	
	ramStamp mStamp;
	bool mShowActor;
	bool mShowBox;
	
public:
	
	Stamp() : mShowActor(true), mShowBox(true) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ofAddListener(panel->newGUIEvent, this, &Stamp::onValueChanged);
		mStamp.setupControlPanel(panel);
		
		panel->addToggle("Show Actor", &mShowActor, 30, 30);
		panel->addToggle("Show Box", &mShowBox, 30, 30);
	}

	void setup()
	{
		mStamp.clear();
	}
	
	void update()
	{
		mStamp.update( getNodeArray(0) );
	}
	
	void draw()
	{
		ramBeginCamera();
		
		for (int i=0; i<mStamp.getNumStamps(); i++)
		{
			ramActor& actor = (ramActor&)mStamp.getStamp(i);
			
			// actor
			if (mShowActor)
			{
				ramDrawBasicActor(actor);
				ramBeginShadow();
				ramDrawBasicActor(actor);
				ramEndShadow();
			}
			
			// shadow
			if (mShowBox)
			{
				ramDrawActorCube(actor);
				ramBeginShadow();
				ramDrawActorCube(actor);
				ramEndShadow();
			}
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
	
	void onValueChanged(ofxUIEventArgs& e)
	{
	}
	
	const string getName() { return "Stamp"; }
};

