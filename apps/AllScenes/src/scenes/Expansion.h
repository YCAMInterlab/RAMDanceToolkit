#pragma once

class Expansion : public ramBaseScene
{
	
	ramExpansion mExpantion;
	ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	bool mShowName;
	float mBoxSize;
	
public:
	
	Expansion() : mShowName(true), mBoxSize(10.0) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();;
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Box size", 3.0, 100.0, &mBoxSize, gui.kLength, gui.kDim);
		panel->addToggle("Show Name", &mShowName, 30, 30);
		mExpantion.setupControlPanel(panel);
		
		panel->addToggle("Show All", true, 30, 30);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 12, 12);
		
		ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
	}
	
	void setup()
	{
		mExpantion.setup();
		
		setAllVisiblity(true);
	}
	
	
	void update()
	{
		
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		ramActor &expandedActor = (ramActor&)mExpantion.update(actor);
		
		ofPushStyle();
		ofNoFill();
		for (int i=0; i<expandedActor.getNumNode(); i++)
		{
			if (mNodeVisibility[i] == false) continue;
			
			ramNode &node = expandedActor.getNode(i);
			
			node.beginTransform();
			ofBox(mBoxSize);
			node.endTransform();
			
			if (mShowName)
			{
				node.drawName(mBoxSize+20);
			}
		}
		ofPopStyle();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Show All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			bool newValue = t->getValue();
			
			setAllVisiblity(newValue);
			for (int i=0; i<ramActor::NUM_JOINTS; i++)
			{
				mToggles[i]->setValue(newValue);
			}
		}
	}
	
	const string getName() { return "Expansion"; }
	
	
	
#pragma mark - 
	
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = b;
		}
	}
};

