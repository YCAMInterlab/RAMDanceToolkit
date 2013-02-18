#pragma once

class Stamp : public ramBaseScene
{
	
	ramStamp mStamp;
	bool mShowActor;
	bool mShowBox;
	
	float r, g, b;
	float line_width;
public:
	
	Stamp() : mShowActor(true), mShowBox(true) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		mStamp.setupControlPanel(panel);
		
		panel->addToggle("Show Actor", &mShowActor, 30, 30);
		panel->addToggle("Show Box", &mShowBox, 30, 30);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUIToggle(32, 32, true, "Box line color"));
		
		panel->addSlider("Stamp R", 0, 255, &r, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addSlider("Stamp G", 0, 255, &g, 95, gui.kDim);
		panel->addSlider("Stamp B", 0, 255, &b, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		panel->addSlider("Line width", 0, 255, &line_width, gui.kLength, gui.kDim);
		
		ofAddListener(panel->newGUIEvent, this, &Stamp::onValueChanged);
	}
	
	void setup()
	{
		mStamp.clear();
	}
	
	void update()
	{
		const int numNudeArrays = getNumNodeArray();
		
		if(numNudeArrays > 0)
		{
			mStamp.update( getNodeArray(ofRandom(0, numNudeArrays)) );
		}
	}
	
	void draw()
	{
		ramBeginCamera();
		
		for (int i=0; i<mStamp.getNumStamps(); i++)
		{
			ramNodeArray& nodeArray = mStamp.getStamp(i);
			
			if (mShowActor)
			{
				ramDrawNodes(nodeArray);
			}
			
			if (mShowBox)
			{
				ofPushStyle();
				ofSetColor(r, g, b);
				ofSetLineWidth(line_width);
				ramDrawActorCube(nodeArray);
				ofPopStyle();
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

