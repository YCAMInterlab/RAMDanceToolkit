#pragma once

class Expansion : public ramBaseScene
{
	enum { NUM_FILTER_BUFFER = 3 };
	
	ramExpansion mExpantion[NUM_FILTER_BUFFER];
	ramLowPassFilter lowpass[NUM_FILTER_BUFFER];
	
	ofxUIToggle *mToggleDraw[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	ofxUIToggle *mToggleSize[ramActor::NUM_JOINTS];
	bool mBiggerSize[ramActor::NUM_JOINTS];
	
	bool mShowAxis;
	bool mShowBox;
	bool mShowName;
	bool mShowLine;
	float mBoxSize;
	float mBoxSizeRatio;
	ofFloatColor mBoxColor;
	
public:
	
	Expansion() : mShowName(true), mShowBox(true), mShowAxis(true), mShowLine(true), mBoxSize(10.0), mBoxSizeRatio(5.0) {}
	
	void setupControlPanel()
	{
//		ramControlPanel &gui = ramGetGUI();
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
		
		panel->getRect()->width =500.0f;
		
		ramGetGUI().addToggle("Show Joint name", &mShowName);
		ramGetGUI().addToggle("Show Box", &mShowBox);
		ramGetGUI().addToggle("Show Axis", &mShowAxis);
		ramGetGUI().addToggle("Show Line", &mShowLine);
		ramGetGUI().addColorSelector("ExtBox", &mBoxColor);
		
		ramGetGUI().addSlider("Box size", 3.0, 100.0, &mBoxSize);
		ramGetGUI().addSlider("Box size ratio", 2.0, 100.0, &mBoxSizeRatio);
		
		panel->addToggle("Toggle box size", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addToggle("Show All", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		// show / hide
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			string name = (i<10 ? " " : "") + ofToString(i);
			ofxUIToggle *toggleSize = new ofxUIToggle("Size" + name, &mBiggerSize[i], 8, 8);
			panel->addWidgetDown(toggleSize);
			mToggleSize[i] = toggleSize;
			
			ofxUIToggle *toggleVisible = new ofxUIToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
			panel->addWidgetRight(toggleVisible);
			mToggleDraw[i] = toggleVisible;
		}
		
		for(int i=0; i<NUM_FILTER_BUFFER; i++)
		{
			mExpantion[i].setupControlPanel();
		}
		
		ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
		
#endif
	}
	
	void setup()
	{
		setAllVisiblity(true);
	}
	
	void draw()
	{
		ramBeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
			ramActor &src = (ramActor&) getNodeArray(i);
			ramActor &expandedActor = (ramActor&) lowpass[i].filter( mExpantion[i].filter(src) );
			
			
			ofPushStyle();
			ofNoFill();
			for (int i=0; i<expandedActor.getNumNode(); i++)
			{
				if (mNodeVisibility[i] == false) continue;
				
				ramNode &node = expandedActor.getNode(i);
				
				node.beginTransform();
				
				int boxSize = mBoxSize * (mBiggerSize[i] ? mBoxSizeRatio : 1);
				
				if (mShowBox)
				{
					ofSetColor(mBoxColor);
					ofBox(boxSize);
				}
				
				if (mShowAxis)
				{
					ofDrawAxis(boxSize);
				}
				
				node.endTransform();
				
				if (mShowLine)
				{
					ofSetColor(100);
					ofSetLineWidth(1);
					ofLine(src.getNode(i), expandedActor.getNode(i));
				}
				
				
				if (mShowName)
				{
					ofSetColor(255);
					node.drawNodeName(mBoxSize+20);
				}
			}
			ofPopStyle();
		}
		ramEndCamera();
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
				mToggleDraw[i]->setValue(newValue);
		}
		
		if (name == "Toggle box size")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			bool newValue = t->getValue();
			
			toggleAllSize(newValue);
			
			for (int i=0; i<ramActor::NUM_JOINTS; i++)
				mToggleSize[i]->setValue(newValue);
		}
	}
	
	string getName() const { return "Expansion"; }
	
	
#pragma mark - 
	
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++) mNodeVisibility[i] = b;
	}
	
	void toggleAllSize(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++) mBiggerSize[i] = b;
	}
};

