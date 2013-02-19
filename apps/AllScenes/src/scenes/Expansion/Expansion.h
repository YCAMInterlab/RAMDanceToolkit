#pragma once

class Expansion : public ramBaseScene
{
	
	ramExpansion mExpantion;
	ramLowPassFilter lowpass;
	
	ofxUIToggle *mToggleDraw[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	ofxUIToggle *mToggleSize[ramActor::NUM_JOINTS];
	bool mBiggerSize[ramActor::NUM_JOINTS];
	
	bool mShowAxis;
	bool mShowBox;
	bool mShowName;
	bool mShowLine;
	bool mShowExtendedLine;
	float mBoxSize;
	float mBoxSizeRatio;
	float r, g, b;
	
public:
	
	Expansion() : mShowName(true), mShowBox(true), mShowAxis(true), mShowLine(true), mShowExtendedLine(true), mBoxSize(10.0), mBoxSizeRatio(5.0), r(250), g(250), b(250) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();;
		
		panel->getRect()->width =500.0f;
		
		panel->addToggle("Show Joint name", &mShowName, 20, 20);
		panel->addToggle("Show Box", &mShowBox, 20, 20);
		panel->addToggle("Show Axis", &mShowAxis, 20, 20);
		panel->addToggle("Show Line1", &mShowLine, 20, 20);
		panel->addToggle("Show Line2", &mShowExtendedLine, 20, 20);
		
		panel->addSlider("ExpBox R", 0, 255, &r, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addSlider("ExpBox G", 0, 255, &g, 95, gui.kDim);
		panel->addSlider("ExpBox B", 0, 255, &b, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		panel->addSlider("Box size", 3.0, 100.0, &mBoxSize, gui.kLength, gui.kDim);
		panel->addSlider("Box size ratio", 2.0, 100.0, &mBoxSizeRatio, gui.kLength, gui.kDim);
		mExpantion.setupControlPanel(panel);
		
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
		
		lowpass.setupControlPanel(panel);
		
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
		ramActor &expandedActor = (ramActor&) lowpass.filter( mExpantion.filter(actor) ) ;
		
		
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
				ofSetColor(r, g, b);
				ofBox(boxSize);
			}
			
			if (mShowAxis)
			{
				ofDrawAxis(boxSize);
			}
			
			if (mShowExtendedLine)
			{
				ofSetLineWidth(2);
				ofLine(actor.getNode(i), expandedActor.getNode(i));
			}
			
			node.endTransform();
			
			if (mShowLine)
			{
				ofSetColor(100);
				ofSetLineWidth(1);
				ofLine(actor.getNode(i), expandedActor.getNode(i));
			}
			
			
			if (mShowName)
			{
				ofSetColor(255);
				node.drawName(mBoxSize+20);
			}
		}
		ofPopStyle();
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
	
	const string getName() { return "Expansion"; }
	
	
	
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

