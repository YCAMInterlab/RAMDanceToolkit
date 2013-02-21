#pragma once

class Stamp : public ramBaseScene
{
	
	ramStamp mStamp;
	bool mShowActor;
	bool mShowBox;
	
	float line_width;
	
	ofFloatColor color;
	
	ramTimerdMovementAnalyser timer;
	float timer_duration;
	
public:
	
	Stamp() : mShowActor(true), mShowBox(true) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		mStamp.setupControlPanel(panel);
		
		gui.addToggle("Show Actor", &mShowActor);
		gui.addColorSelector("Box line color", &color);
		gui.addSlider("Line width", 0, 6, &line_width);
		
		gui.addSlider("timer_duration", 0, 4, &timer_duration);
		
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
		
		if (timer_duration > 0)
		{
			timer.setTime(timer_duration);
		}
	}
	
	void draw()
	{
		ramBeginCamera();
		
		for (int i=0; i<mStamp.getSize(); i++)
		{
			ramNodeArray& nodeArray = mStamp.getStamp(i);
			
			if (mShowActor)
			{
				ramDrawNodes(nodeArray);
			}
			
			if (color.a > 0)
			{
				ofPushStyle();
				ofSetColor(color);
				ofSetLineWidth(line_width);
				ramDrawActorCube(nodeArray, ramColor::RED_LIGHT);
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

