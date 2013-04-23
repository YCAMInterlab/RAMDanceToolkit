// 
// Expansion.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once


class Expansion : public ramBaseScene
{
	
public:
    
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
		ramGetGUI().addSlider("Big Box ratio", 2.0, 10.0, &mBoxSizeRatio);
		
		panel->addToggle("Toggle box size", false, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addToggle("Show All", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		for(int i=0; i<ramActor::NUM_JOINTS; i++)
		{
            mBiggerSize[i] = false;
            mNodeVisibility[i] = true;
            
            string name = (i<10 ? " " : "") + ofToString(i);
			ofxUIToggle *toggleSize = new ofxUIToggle("Size" + name, &mBiggerSize[i], 8, 8);
			panel->addWidgetDown(toggleSize);
			mToggleSize[i] = toggleSize;
			
			ofxUIToggle *toggleVisible = new ofxUIToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
			panel->addWidgetRight(toggleVisible);
			mToggleDraw[i] = toggleVisible;
		}
		
		ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
		
#endif
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
			for (int nodeId=0; nodeId<processedNA.getNumNode(); nodeId++)
			{
				if (mNodeVisibility[nodeId] == false) continue;
                
				ramNode &node = processedNA.getNode(nodeId);
				
				node.beginTransform();
				
				int boxSize = mBoxSize * (mBiggerSize[nodeId] ? mBoxSizeRatio : 1);
				
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
					ofLine(src.getNode(nodeId), processedNA.getNode(nodeId));
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
			
			seteAllSizeBigger(newValue);
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
            mToggleDraw[i]->setValue(b);
	}
	
	void seteAllSizeBigger(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
            mToggleSize[i]->setValue(b);
	}
    
    
private:
    
    ramFilterEach<ramExpansion> mExpansion;
    ramFilterEach<ramLowPassFilter> mLowpass;
    
	ofxUIToggle *mToggleDraw[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	ofxUIToggle *mToggleSize[ramActor::NUM_JOINTS];
	bool mBiggerSize[ramActor::NUM_JOINTS];
	
	bool mShowAxis;
	bool mShowBox;
	bool mShowName;
	bool mShowLine;
	float mExpasionRatio;
	float mBoxSize;
	float mBoxSizeRatio;
	ofFloatColor mBoxColor;
};

