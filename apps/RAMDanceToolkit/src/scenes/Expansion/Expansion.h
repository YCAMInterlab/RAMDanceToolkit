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


class Expansion : public rdtk::BaseScene
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
    

	void drawImGui()
	{
		ImGui::Checkbox("Show Joint name", &mShowName);	ImGui::SameLine();
		ImGui::Checkbox("Show Box", &mShowBox);
		ImGui::Checkbox("Show Axis", &mShowAxis);		ImGui::SameLine();
		ImGui::Checkbox("Show Line", &mShowLine);
		ImGui::ColorEdit3("Box Color", &mBoxColor[0]);
		
		ImGui::DragFloat("Expansion Ratio", &mExpasionRatio, 0.5, 1.0, 10.0);
		ImGui::DragFloat("Box size", &mBoxSize, 1.0, 3.0, 100.0);
		ImGui::DragFloat("BigBox ratio", &mBoxSizeRatio, 0.5, 2.0, 10.0);
		
		static bool boxSize = false;
		static bool showAll = false;
		if (ImGui::Checkbox("Toggle box size", &boxSize)) seteAllSizeBigger(boxSize);
		ImGui::SameLine();
		if (ImGui::Checkbox("Show All", &showAll)) setAllVisiblity(showAll);
		
		ImGui::Columns(2, NULL, true);
		for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
		{
			ImGui::PushID(ofToString(i).c_str());
			ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
			ImGui::NextColumn();
			ImGui::Checkbox("Bigger", &mBiggerSize[i]);
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);
	}
	
	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
		
		panel->getRect()->width =500.0f;
		
		rdtk::GetGUI().addToggle("Show Joint name", &mShowName);
		rdtk::GetGUI().addToggle("Show Box", &mShowBox);
		rdtk::GetGUI().addToggle("Show Axis", &mShowAxis);
		rdtk::GetGUI().addToggle("Show Line", &mShowLine);
		rdtk::GetGUI().addColorSelector("Box Color", &mBoxColor);
		
		rdtk::GetGUI().addSlider("Expasion Ratio", 1.0, 10.0, &mExpasionRatio);
		rdtk::GetGUI().addSlider("Box size", 3.0, 100.0, &mBoxSize);
		rdtk::GetGUI().addSlider("Big Box ratio", 2.0, 10.0, &mBoxSizeRatio);
		
		panel->addToggle("Toggle box size", false, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addToggle("Show All", true, 20, 20);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		for(int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
		{
            mBiggerSize[i] = false;
            mNodeVisibility[i] = true;
            
            string name = (i<10 ? " " : "") + ofToString(i);
			ofxUIToggle *toggleSize = new ofxUIToggle("Size" + name, &mBiggerSize[i], 8, 8);
			panel->addWidgetDown(toggleSize);
			mToggleSize[i] = toggleSize;
			
			ofxUIToggle *toggleVisible = new ofxUIToggle(rdtk::Actor::getJointName(i), &mNodeVisibility[i], 8, 8);
			panel->addWidgetRight(toggleVisible);
			mToggleDraw[i] = toggleVisible;
		}
		
		ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
		
#endif
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
	
	
	
	void draw()
	{
        vector<rdtk::NodeArray> expNAs = mExpansion.update(getAllNodeArrays());
        vector<rdtk::NodeArray> lpNAs = mLowpass.update(expNAs);

		rdtk::BeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
            rdtk::NodeArray &src = getNodeArray(i);
            rdtk::NodeArray &processedNA = lpNAs.at(i);
			
			ofPushStyle();
			ofNoFill();
			for (int nodeId=0; nodeId<processedNA.getNumNode(); nodeId++)
			{
				if (mNodeVisibility[nodeId] == false) continue;
                
				rdtk::Node &node = processedNA.getNode(nodeId);
				
				node.beginTransform();
				
				int boxSize = mBoxSize * (mBiggerSize[nodeId] ? mBoxSizeRatio : 1);
				
				if (mShowBox)
				{
					ofSetColor(mBoxColor);
					ofDrawBox(boxSize);
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
					ofDrawLine(src.getNode(nodeId), processedNA.getNode(nodeId));
				}
				
				
				if (mShowName)
				{
					ofSetColor(255);
					node.drawNodeName(mBoxSize+20);
				}
			}
			ofPopStyle();
		}
		rdtk::EndCamera();
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
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            mToggleDraw[i]->setValue(b);
	}
	
	void seteAllSizeBigger(bool b)
	{
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            mToggleSize[i]->setValue(b);
	}
    
    
private:
    
	ramFilterEach<rdtk::Expansion> mExpansion;
	ramFilterEach<rdtk::LowPassFilter> mLowpass;
    
	ofxUIToggle *mToggleDraw[rdtk::Actor::NUM_JOINTS];
	bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
	
	ofxUIToggle *mToggleSize[rdtk::Actor::NUM_JOINTS];
	bool mBiggerSize[rdtk::Actor::NUM_JOINTS];
	
	bool mShowAxis;
	bool mShowBox;
	bool mShowName;
	bool mShowLine;
	float mExpasionRatio;
	float mBoxSize;
	float mBoxSizeRatio;
	ofFloatColor mBoxColor;
};

