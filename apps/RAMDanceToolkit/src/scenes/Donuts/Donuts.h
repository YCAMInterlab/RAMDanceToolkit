// 
// Donuts.h - RamDanceToolkit
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

#include "ramActor.h"

class Donuts : public rdtk::BaseScene
{
	
	ofxUIToggle *mToggles[rdtk::Actor::NUM_JOINTS];
	bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];

	float mNumDuplicate;
	float mRadius;
	float mScale;
	float mBoxSize;
	ofVec3f mTranslate;
	bool mShowActor;
	bool mResetAll;
	bool mToggleAll;
	
	int mNumDuplicates;
	float mRadian;
	
public:
	
	
	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
		
		rdtk::GetGUI().addSection("Original me");
		rdtk::GetGUI().addSlider("Scale", 0.1, 10.0, &mScale);
		rdtk::GetGUI().addSlider("Position: X", -300.0, 300.0, &mTranslate.x);
		rdtk::GetGUI().addSlider("Position: Z", -300.0, 300.0, &mTranslate.z);
		
		rdtk::GetGUI().addSection("Duplicated me");
		rdtk::GetGUI().addSlider("Duplicate", 1, 200, &mNumDuplicate);
		rdtk::GetGUI().addSlider("Radius", 10.0, 1000.0, &mRadius);
		rdtk::GetGUI().addSlider("Box Size", 1.0, 1000.0, &mBoxSize);
		
		rdtk::GetGUI().addSection("Reset, Toggle visibility");
		panel->addButton("Reset all settings", false, 30, 30);
		rdtk::GetGUI().addToggle("Show Actor", &mShowActor);
		rdtk::GetGUI().addToggle("Toggle All", &mToggleAll);
		
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == rdtk::Actor::JOINT_RIGHT_HAND || i == rdtk::Actor::JOINT_LEFT_HAND);
			mToggles[i] = panel->addToggle(rdtk::Actor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
		clear();
		ofAddListener(panel->newGUIEvent, this, &Donuts::onValueChanged);
		
#endif
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Toggle All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			setAllVisiblity(t->getValue());
		}
		
		if (name == "Reset all settings")
		{
			clear();
		}
	}
	
	
	void drawImGui()
	{
		ImGui::Text("Original me");
		ImGui::DragFloat("Scale", &mScale, 0.1, 0.1, 10.0);
		ImGui::DragFloat("Position: X", &mTranslate.x, 1, -300.0, 300.0);
		ImGui::DragFloat("Position: Z", &mTranslate.z, 1, -300.0, 300.0);
		
		ImGui::Spacing();
		ImGui::Text("Duplicate me");
		ImGui::DragFloat("Duplicate", &mNumDuplicate, 1, 1, 200);
		ImGui::DragFloat("Radius", &mRadius, 1, 10.0, 1000.0);
		ImGui::DragFloat("Box Size", &mBoxSize, 1, 1, 1000.0);
		
		ImGui::Spacing();
		ImGui::Text("Reset, Toggle visibility");
		if (ImGui::Button("Reset all settings")) clear();
		ImGui::Checkbox("Show Actor", &mShowActor);
		if (ImGui::Checkbox("Toggle All", &mToggleAll)) setAllVisiblity(mToggleAll);
		
		ImGui::Columns(3, NULL, true);
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
		{
			ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}

	
	void setup()
	{
		
	}
	
	void update()
	{
		mNumDuplicate = floor(mNumDuplicate);
		mRadian = 2 * M_PI / mNumDuplicate;
	}
	
	void drawDonuts(const rdtk::NodeArray &nodeArray)
	{
		ofPushStyle();
		ofNoFill();
		
		if (mShowActor)
		{
			for (int n=0; n<mNumDuplicate; n++)
			{
				ofColor c1 = rdtk::Color::RED_DEEP;
				ofColor c2 = rdtk::Color::BLUE_LIGHT;
				c1.g += n*3;
				c2.b += n*3;
				
				float x = cos( mRadian * n ) * mRadius;
				float z = sin( mRadian * n ) * mRadius;
				
				ofPushMatrix();
				ofTranslate(x, 0, z);
				if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
				
				ofRotateY(360/mNumDuplicate * n);
				if( n==0 ) ofScale(mScale, mScale, mScale);
				

				ofPushStyle();
				for(int i=0; i<nodeArray.getNumNode(); i++)
				{
					const rdtk::Node& node = nodeArray.getNode(i);
					
					node.beginTransform();
					ofSetColor(c1);
					ofDrawBox( i==rdtk::Actor::JOINT_HEAD ? 8 : 5);
					node.endTransform();
					
					if (node.hasParent())
					{
						ofSetColor(c2);
						ofDrawLine(node, *node.getParent());
					}
				}
				ofPopStyle();
				
				ofPopMatrix();
			}
		}
		
		else
		{
			for (int index=0; index<nodeArray.getNumNode(); index++)
			{
				if (mNodeVisibility[index] == false) continue;
				
				for (int n=0; n<mNumDuplicate; n++)
				{
					const rdtk::Node &node = nodeArray.getNode(index);
					
					ofPushMatrix();
					{
						ofColor c1 = rdtk::Color::RED_DEEP;
						ofColor c2 = rdtk::Color::BLUE_LIGHT;
						c1.g += n*3;
						c2.b += n*3;
						
						float x = cos( mRadian * n ) * mRadius;
						float z = sin( mRadian * n ) * mRadius;
						
						ofMatrix4x4 m = node.getOrientationQuat();
						glMultMatrixf(m.getPtr());
						
						ofTranslate(x, 0, z);
						if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
						
						ofRotateY(360/mNumDuplicate * n);
						if( n==0 ) ofScale(mScale, mScale, mScale);
						
						node.beginTransform();
						ofSetColor(c2);
						ofDrawBox(mBoxSize);
						node.endTransform();
					}
					ofPopMatrix();
				}
			}
		}
		
		
		ofPopStyle();

	}
	
	void draw()
	{

	}
	
	void drawActor(const rdtk::Actor& actor)
	{
		drawDonuts( actor );
	}
	
	void drawRigid(const rdtk::RigidBody &rigid)
	{
		drawDonuts( rigid );
	}
	
	void clear()
	{
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == rdtk::Actor::JOINT_RIGHT_HAND || i == rdtk::Actor::JOINT_LEFT_HAND);
			mToggles[i]->setValue(mNodeVisibility[i]);
		}
		
		mNumDuplicate = 20;
		mRadius = 110;
		mScale = 1.0;
		mBoxSize = 3.0;
		mTranslate = ofVec3f::zero();
		mShowActor = false;
	}
	
	string getName() const { return "Donuts"; }
	
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
			mToggles[i]->setValue(b);
	}
};

