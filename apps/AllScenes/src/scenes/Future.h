#pragma once

class Future : public ramBaseScene
{
	
	ramGhost mGhost;
	
public:
	
	Future() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		panel->addButton("Ghost", false, 50, 50);
		panel->addButton("Slow", false, 50, 50);
		panel->addButton("Normal", false, 50, 50);
		panel->addButton("Fast", false, 50, 50);
		
		mGhost.setupControlPanel(panel);
		ofAddListener(panel->newGUIEvent, this, &Future::onValueChanged);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		mGhost.update( actor );

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		ofPushStyle();
		ofNoFill();
		
		ramDrawBasicActor( (ramActor&)mGhost.get() );

		ofPopStyle();
		glPopAttrib();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Ghost")
		{
			mGhost.setSpeed(1.5);
			mGhost.setDistance(240);
		}
		if (name == "Slow")
		{
			mGhost.setSpeed(8.3);
			mGhost.setDistance(74.4);
		}
		if (name == "Normal")
		{
			mGhost.setSpeed(9.4);
			mGhost.setDistance(150);
		}
		if (name == "Fast")
		{
			mGhost.setSpeed(38.9);
			mGhost.setDistance(211.1);
		}
	}
	
	const string getName() { return "Future"; }
};

