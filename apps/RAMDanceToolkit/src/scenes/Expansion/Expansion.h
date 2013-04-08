#pragma once

class Expansion : public ramBaseScene
{
	
public:
    
    class JointUI
    {
        
    public:
        
        JointUI(int index) : bBigger(false), bVidibility(true)
        {
            idx = index;
            
			string nameBigger = (index<10 ? " " : "") + ofToString(index);
            toggleSize = new ofxUIToggle("Size:" + nameBigger, &bBigger, 8, 8);
            
			string nameDraw = ramActor::getJointName(index);
            toggleDraw = new ofxUIToggle("Show:" + nameDraw, &bVidibility, 8, 8);
        }
        
        ofxUIToggle *toggleDraw;
        ofxUIToggle *toggleSize;
        bool bBigger;
        bool bVidibility;
        int idx;
    };
    
    
	Expansion() :
    mShowName(true),
    mShowBox(true),
    mShowAxis(true),
    mShowLine(true),
    mExpasionRatio(1.5),
    mBoxSize(10.0),
    mBoxSizeRatio(5.0) {}
    

	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
		
		panel->getRect()->width =500.0f;
		
		ramGetGUI().addToggle("Show Joint name", &mShowName);
		ramGetGUI().addToggle("Show Box", &mShowBox);
		ramGetGUI().addToggle("Show Axis", &mShowAxis);
		ramGetGUI().addToggle("Show Line", &mShowLine);
		ramGetGUI().addColorSelector("Box Color", &mBoxColor);
		
		ramGetGUI().addSlider("Expasion Ratio", 1.0, 10.0, &mExpasionRatio);
		ramGetGUI().addSlider("Box size", 3.0, 100.0, &mBoxSize);
		ramGetGUI().addSlider("Big Box ratio", 2.0, 100.0, &mBoxSizeRatio);
		
		panel->addToggle("Toggle box size", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addToggle("Show All", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		for(int i=0; i<ramActor::NUM_JOINTS; i++)
		{
            JointUI *jointui = new JointUI(i);
            panel->addWidgetDown(jointui->toggleSize);
            panel->addWidgetRight(jointui->toggleDraw);
            jointUIs.push_back(jointui);
		}
		
		ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
		
#endif
	}
	
	void setup()
	{
        jointUIs.clear();
	}
	
	void draw()
	{
        vector<ramNodeArray> expNAs = mExpansion.update(getAllNodeArrays());
        vector<ramNodeArray> lpNAs = mLowpass.update(expNAs);

		ramBeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
            ramNodeArray &src = getNodeArray(i);
            ramNodeArray &processedNA = lpNAs.at(i);
			
			ofPushStyle();
			ofNoFill();
			for (int j=0; j<processedNA.getNumNode(); j++)
			{
                JointUI *jointui = jointUIs[j];
                
				if (jointui->bVidibility == false) continue;
                
				ramNode &node = processedNA.getNode(j);
				
				node.beginTransform();
				
				int boxSize = mBoxSize * (jointui->bBigger ? mBoxSizeRatio : 1);
				
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
					ofLine(src.getNode(j), processedNA.getNode(j));
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
        
		if (name == "Expasion Ratio")
		{
            ofxUISlider *slider = (ofxUISlider *)e.widget;
            setExpasionRatio(slider->getValue());
		}
        
		if (name == "Show All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			bool newValue = t->getValue();
			
			setAllVisiblity(newValue);
		}
		
		if (name == "Toggle box size")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			bool newValue = t->getValue();
			
			toggleAllSize(newValue);
		}
	}
    
	string getName() const { return "Expansion"; }

	
#pragma mark - 
	
	
	void setExpasionRatio(float ratio)
	{
		for (int i=0; i<mExpansion.getNumFilters(); i++)
        {
            mExpansion.getFilter(i).setExpand(ratio);
        }
	}
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
        {
            jointUIs.at(i)->bVidibility = b;
        }
	}
	
	void toggleAllSize(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
        {
            jointUIs.at(i)->bBigger = b;
        }
	}
    
    
private:
    
    ramFilterEach<ramExpansion> mExpansion;
    ramFilterEach<ramLowPassFilter> mLowpass;
	
    vector<JointUI*> jointUIs;
	
	bool mShowAxis;
	bool mShowBox;
	bool mShowName;
	bool mShowLine;
	float mExpasionRatio;
	float mBoxSize;
	float mBoxSizeRatio;
	ofFloatColor mBoxColor;
};

