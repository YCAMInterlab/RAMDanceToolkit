#pragma once
#include "ramMain.h"

class ramGhost
{
public:
	ramGhost() :
	max_entities(10),
	freshness(150),
	emphasis(27)
	{
		recordedNodes.clear();
	}
	virtual ~ramGhost() {}
	
	void setup() {}
	
	void update(ramActor &present)
	{
		updateFutureNodes( (ramNodeArray&)present );
	}
	
	void update(ramRigidBody &present)
	{
		updateFutureNodes( (ramNodeArray&)present );
	}
	
	inline ramNodeArray& getFutureNodes() { return futureActor; }
	
	inline void setFreshness(const float f) { freshness = f; }
	inline void setEmphasis(const float e) { emphasis = e; }
	inline void setMaxEntities(const unsigned int m) { max_entities = m; }
	
	inline float getFreshness() { return freshness; }
	inline float getEmphasis() { return emphasis; }
	inline unsigned int getMaxEntities() { return max_entities; }
	
protected:
	deque<ramNodeArray> recordedNodes;
	ramActor futureActor;
	
	unsigned int max_entities;
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
			ofVec3f f = futureActor.getNode(i).getPosition();
			
			f += (v - f) * emphasis * 0.001;
			
			futureActor.getNode(i).setPosition(f);
		}
	}
};