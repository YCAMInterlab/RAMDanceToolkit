// 
// Monster.h - RAMDanceToolkit
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

#include "ramMain.h"

class Monster : public rdtk::BaseScene
{
public:
	
	vector<int> treeBase, treeSwap;
	vector<float> lengthScale;
	rdtk::NodeArray monsterArray;
	float minScale, maxScale, randomizationAmount;
	bool needToReset, randomLine, randomizeTopology, randomizeGeometry;
		
	void setupControlPanel()
	{
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		needToReset = false;
		randomizeTopology = false;
		randomizeGeometry = false;
		randomLine = false;
		minScale = .5;
		maxScale = 2;
		randomizationAmount = .5;
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
		panel->addButton("Reset", &needToReset, 20, 20);
		panel->addButton("Random Line", &randomLine, 20, 20);
		panel->addButton("Randomize Topology", &randomizeTopology, 20, 20);
		panel->addButton("Randomize Geometry", &randomizeGeometry, 20, 20);
		panel->addSlider("Min scale", 0, 4, &minScale, 300, 20);
		panel->addSlider("Max scale", 0, 4, &maxScale, 300, 20);
		panel->addSlider("Randomization amount", 0, 1, &randomizationAmount, 300, 20);
		
#endif
	}
	
	void drawImGui()
	{
		if (ImGui::Button("Reset")) needToReset = true;
		if (ImGui::Button("Random Line")) randomLine = true;
		if (ImGui::Button("Randomize Topology")) randomizeTopology = true;
		if (ImGui::Button("Randomize Geometry")) randomizeGeometry = true;
		
		ImGui::SliderFloat("Min scale", &minScale, 0.0, 4.0);
		ImGui::SliderFloat("Max scale", &maxScale, 0.0, 4.0);
		ImGui::SliderFloat("Randomization amount", &randomizationAmount, 0, 1);
	}
	
	void setup()
	{
		int treeArray[] = {
			-1,
			rdtk::Actor::JOINT_HIPS,
			rdtk::Actor::JOINT_ABDOMEN,
			rdtk::Actor::JOINT_CHEST,
			rdtk::Actor::JOINT_NECK,
			rdtk::Actor::JOINT_HIPS,
			rdtk::Actor::JOINT_LEFT_HIP,
			rdtk::Actor::JOINT_LEFT_KNEE,
			rdtk::Actor::JOINT_LEFT_ANKLE,
			rdtk::Actor::JOINT_HIPS,
			rdtk::Actor::JOINT_RIGHT_HIP,
			rdtk::Actor::JOINT_RIGHT_KNEE,
			rdtk::Actor::JOINT_RIGHT_ANKLE,
			rdtk::Actor::JOINT_CHEST,
			rdtk::Actor::JOINT_LEFT_COLLAR,
			rdtk::Actor::JOINT_LEFT_SHOULDER,
			rdtk::Actor::JOINT_LEFT_ELBOW,
			rdtk::Actor::JOINT_LEFT_WRIST,
			rdtk::Actor::JOINT_CHEST,
			rdtk::Actor::JOINT_RIGHT_COLLAR,
			rdtk::Actor::JOINT_RIGHT_SHOULDER,
			rdtk::Actor::JOINT_RIGHT_ELBOW,
			rdtk::Actor::JOINT_RIGHT_WRIST
		};
		treeBase.resize(rdtk::Actor::NUM_JOINTS);
		lengthScale.resize(rdtk::Actor::NUM_JOINTS, 1);
		for(int i = 0; i < treeBase.size(); i++) {
			treeBase[i] = treeArray[i];
		}
		reset();
	}
	
	void reset() 
	{
		treeSwap = treeBase;
		lengthScale.clear();
		lengthScale.resize(rdtk::Actor::NUM_JOINTS, 1);
	}
	
	bool isAncestor(int ancestor, int child) {
		int cur = child;
		while(treeSwap[cur] != -1)
		{
			if(cur == ancestor) 
			{
				return true;
			}
			cur = treeSwap[cur];
		}
		return false;
	}
	
	vector<int> getNonChildren(int ancestor)
	{
		vector<int> nonchildren;
		int n = rdtk::Actor::NUM_JOINTS;
		for(int child = 0; child < n; child++)
		{
			if(!isAncestor(ancestor, child))
			{
				nonchildren.push_back(child);
			}
		}
		return nonchildren;
	}
	
	int getRandomNonChild(int i)
	{
		vector<int> nonchildren = getNonChildren(i);
		int nonchild = nonchildren[ofRandom(0, nonchildren.size())];
		return nonchild;
	}
	
	void attach(int child, int parent)
	{
		treeSwap[child] = parent;
	}
	
	void update()
	{
		if(needToReset)
		{
			reset();
			needToReset = false;
		}
		if(randomizeTopology) 
		{
			reset();
			int n = treeSwap.size();
			for(int i = 0; i < n; i++)
			{
				if(treeSwap[i] != -1 && ofRandom(1) < randomizationAmount) 
				{
					treeSwap[i] = getRandomNonChild(i);
				}
			}
			randomizeTopology = false;
		}
		if(randomizeGeometry) 
		{
			int n = lengthScale.size();
			for(int i = 0; i < n; i++)
			{
				if(ofRandom(1) < randomizationAmount)
				{
					lengthScale[i] = ofRandom(minScale, maxScale);
				} else {
					lengthScale[i] = 1;
				}
			}
			randomizeGeometry = false;
		}
		if(randomLine)
		{
			reset();
			vector<int> all;
			for(int i = 1; i < rdtk::Actor::NUM_JOINTS; i++) 
			{
				all.push_back(i);
			}
			ofRandomize(all);
			attach(all[0], rdtk::Actor::JOINT_HIPS);
			for(int i = 1; i < all.size(); i++)
			{
				attach(all[i], all[i - 1]);
			}
			randomLine = false;
		}
	}
	
	void draw()
	{	
	}
	
	void drawActor(const rdtk::Actor &actor)
	{	
		
		monsterArray = actor;
		
		for (int i=0; i < treeSwap.size(); i++)
		{
			if(treeSwap[i] != -1)
			{
				monsterArray.getNode(i).setParent(monsterArray.getNode(treeSwap[i]));
				monsterArray.getNode(i).setScale(lengthScale[i]);
			}
		}
		
		ofPushStyle();
		ofNoFill();
		for (int i=0; i<monsterArray.getNumNode(); i++)
		{
			rdtk::Node &node = monsterArray.getNode(i);
			ofSetColor(255);
			ofSetLineWidth(2);
			if(node.hasParent())
			{
				ofDrawLine(node, *node.getParent());
			}
		}
		ofPopStyle();
	}
	
	void drawRigid(const rdtk::RigidBody &rigid)
	{
	}
	
	string getName() const { return "Monster"; }
	
};
