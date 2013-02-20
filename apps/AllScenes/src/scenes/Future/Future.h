#pragma once

class Future : public ramBaseScene
{
	
	enum { NUM_FILTER_BUFFER = 3 };
	ramGhost mGhosts[NUM_FILTER_BUFFER];
	ramLowPassFilter mLowpass[NUM_FILTER_BUFFER];
	
public:
	
	bool draw_line;
	
	Future() {}

	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		gui.addToggle("draw_line", &draw_line);
		
		for(int i=0; i<3; i++)
		{
			mGhosts[i].setupControlPanel(panel);
			mLowpass[i].setupControlPanel(panel);
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
			const ramNodeArray &src = getNodeArray(i);
			mGhosts[i].update(src);
		}
	}

	void draw()
	{
		ramBeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
			ramNodeArray &NA = getNodeArray(i);
			const ramNodeArray &ghost = mLowpass[i].filter( mGhosts[i].get() );
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			ofPushStyle();
			ofNoFill();
			
			const ofColor gcolor =
				i==0 ? ramColor::RED_LIGHT :
				i==1 ? ramColor::YELLOW_DEEP : ramColor::BLUE_LIGHT;
			
			ramDrawNodes( ghost, gcolor );
			
			if (draw_line)
				ramDrawNodeCorresponds(NA, ghost);
			
			ofPopStyle();
			glPopAttrib();
		}
		ramEndCamera();
	}
	
	void onValueChanged(ofxUIEventArgs& e){}
	const string getName() { return "Future"; }
};

