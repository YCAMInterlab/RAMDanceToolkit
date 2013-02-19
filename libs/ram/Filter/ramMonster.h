#pragma once

#include "ramMain.h"
#include "ramBaseFilter.h"
#include "ramControlPanel.h"

class ramMonster : public ramBaseFilter
{
	
public:
	
	ramMonster()
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
	
	void setupControlPanel(ofxUICanvas* panel)
	{
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
	
	void randomizeTopology()
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
	
	void randomizeGeometry() 
	{
		
	}
	
	void attach(int child, int parent)
	{
		treeSwap[child] = parent;
	}
	
	const ramNodeArray& get(size_t index) const { return monsterArray; }
	size_t getSize() const { return 1; }
	
	inline const string getName() { return "ramMonster"; };
	
protected:
	
	vector<int> treeBase, treeSwap;
	vector<float> lengths;
	ramNodeArray monsterArray;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		monsterArray = src;
		
		for (int i=0; i < treeSwap.size(); i++)
		{
			if(treeSwap[i] != -1)
			{
				monsterArray.getNode(i).setParent(monsterArray.getNode(treeSwap[i]));
			}
		}
		
		return monsterArray;
	}
	
};