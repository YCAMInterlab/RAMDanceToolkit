#pragma once

#include "ramMain.h"

class Monster : public ramBaseScene
{
public:
	
	vector<int> treeBase, treeSwap;
	vector<float> lengths;
	ramNodeArray monsterArray;
	
	bool needToReset, randomizeTopology, randomizeGeometry, swapLeftRight, swapArmsLegs;
		
	void setupControlPanel(ofxUICanvas* panel)
	{
		randomizeTopology = false;
		randomizeGeometry = false;
		panel->addButton("Reset", &needToReset, 40, 40);
		panel->addButton("Randomize Topology", &randomizeTopology, 40, 40);
		panel->addButton("Randomize Geometry", &randomizeGeometry, 40, 40);
		panel->addButton("Swap Left/Right", &swapLeftRight, 40, 40);
		panel->addButton("Swap Arms/Legs", &swapArmsLegs, 40, 40);
	}
	
	void setup()
	{
		int treeArray[] = {
			-1,
			ramActor::JOINT_HIPS,
			ramActor::JOINT_ABDOMEN,
			ramActor::JOINT_CHEST,
			ramActor::JOINT_NECK,
			ramActor::JOINT_HIPS,
			ramActor::JOINT_LEFT_HIP,
			ramActor::JOINT_LEFT_KNEE,
			ramActor::JOINT_LEFT_ANKLE,
			ramActor::JOINT_HIPS,
			ramActor::JOINT_RIGHT_HIP,
			ramActor::JOINT_RIGHT_KNEE,
			ramActor::JOINT_RIGHT_ANKLE,
			ramActor::JOINT_CHEST,
			ramActor::JOINT_LEFT_COLLAR,
			ramActor::JOINT_LEFT_SHOULDER,
			ramActor::JOINT_LEFT_ELBOW,
			ramActor::JOINT_LEFT_WRIST,
			ramActor::JOINT_CHEST,
			ramActor::JOINT_RIGHT_COLLAR,
			ramActor::JOINT_RIGHT_SHOULDER,
			ramActor::JOINT_RIGHT_ELBOW,
			ramActor::JOINT_RIGHT_WRIST
		};
		treeBase.resize(ramActor::NUM_JOINTS);
		for(int i = 0; i < treeBase.size(); i++) {
			treeBase[i] = treeArray[i];
		}
		reset();
	}
	
	void reset() 
	{
		treeSwap = treeBase;
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
		int n = ramActor::NUM_JOINTS;
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
		}
		if(randomizeTopology) 
		{
			reset();
			int n = treeSwap.size();
			for(int i = 0; i < n; i++)
			{
				if(treeSwap[i] != -1) 
				{
					treeSwap[i] = getRandomNonChild(i);
				}
			}
		}
		if(randomizeGeometry) 
		{
		}
		if(swapLeftRight)
		{
			reset();
			attach(ramActor::JOINT_LEFT_KNEE, ramActor::JOINT_RIGHT_HIP);
			attach(ramActor::JOINT_RIGHT_KNEE, ramActor::JOINT_LEFT_HIP);
			attach(ramActor::JOINT_LEFT_ELBOW, ramActor::JOINT_RIGHT_SHOULDER);
			attach(ramActor::JOINT_RIGHT_ELBOW, ramActor::JOINT_LEFT_SHOULDER);
		}
		if(swapArmsLegs)
		{
			reset();
			attach(ramActor::JOINT_LEFT_KNEE, ramActor::JOINT_LEFT_SHOULDER);
			attach(ramActor::JOINT_RIGHT_KNEE, ramActor::JOINT_RIGHT_SHOULDER);
			attach(ramActor::JOINT_LEFT_ELBOW, ramActor::JOINT_LEFT_HIP);
			attach(ramActor::JOINT_RIGHT_ELBOW, ramActor::JOINT_RIGHT_HIP);
		}
	}
	
	void draw()
	{	
	}
	
	void drawActor(ramActor &actor)
	{	
		
		monsterArray = actor;
		
		for (int i=0; i < treeSwap.size(); i++)
		{
			if(treeSwap[i] != -1)
			{
				monsterArray.getNode(i).setParent(monsterArray.getNode(treeSwap[i]));
			}
		}
		
		ofPushStyle();
		ofNoFill();
		for (int i=0; i<monsterArray.getNumNode(); i++)
		{
			ramNode &node = monsterArray.getNode(i);
			ofSetColor(255);
			ofSetLineWidth(2);
			if(node.hasParent())
			{
				ofLine(node, *node.getParent());
			}
		}
		ofPopStyle();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Monster"; }
	
};
