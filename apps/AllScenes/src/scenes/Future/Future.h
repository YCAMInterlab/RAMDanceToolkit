#pragma once

class Future : public ramBaseScene
{
	
	ramGhost mGhosts[5];

public:

	Future() {}

	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		for(int i=0; i<5; i++)
		{
			mGhosts[i].setupControlPanel(panel);
		}
		ofAddListener(panel->newGUIEvent, this, &Future::onValueChanged);
	}

	void setup()
	{

	}

	void update()
	{
		for (int i=0; i<getNumNodeArray(); i++)
		{
			ramNodeArray &src = getNodeArray(i);
			mGhosts[i].update(src);
		}
	}

	void draw()
	{
		ramBeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			ofPushStyle();
			ofNoFill();
			
			ramDrawNodes( (ramActor&)mGhosts[i].get() );
			
			ofPopStyle();
			glPopAttrib();
		}
		ramEndCamera();
	}
	
	void onValueChanged(ofxUIEventArgs& e){}
	const string getName() { return "Future"; }
};

