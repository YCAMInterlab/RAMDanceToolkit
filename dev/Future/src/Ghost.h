#pragma once
#include "ramMain.h"

class Ghost
{
public:
	Ghost() :
	max_entities(10),
	freshness(100), //動きの強調 
	emphasis(100) // ghostの生きの良さ
	{
		recordedNodes.clear();
	}
	virtual ~Ghost() {}
	
	void setup() {}
	
	void update(ramActor &present)
	{
		updateFutureNodes( (ramNodeArray&)present );
	}
	
	inline ramNodeArray& getFutureNodes() { return futureNodes; }
	
	
protected:
	int max_entities;
	float freshness, emphasis;
	
	void updateFutureNodes(ramNodeArray &nodeArray)
	{
		
		if (nodeArray.getNumNode() != 0)
			recordedNodes.push_back(nodeArray);
		
		if (recordedNodes.size() > max_entities)
			recordedNodes.pop_front();
		
		ramNodeArray &presentArray = recordedNodes.back();
		ramNodeArray &pastArray = recordedNodes.front();
		
		for (int i = 0; i < presentArray.getNumNode(); i++)
		{
			ofVec3f p0 = presentArray.getNode(i).getPosition();
			ofVec3f p1 = pastArray.getNode(i).getPosition();
			
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= freshness;
			ofVec3f v = p0 + d;
			ofVec3f f = futureNodes.getNode(i).getPosition();
			
			f += (v - f) * emphasis * 0.001;
			
			futureNodes.getNode(i).setPosition(f);
		}
	}
	
	deque<ramNodeArray> recordedNodes;
	ramActor futureNodes;
};