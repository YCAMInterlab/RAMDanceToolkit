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
		clear();
	}
	virtual ~ramGhost() {}
	
	void clear()
	{
		recordedActors.clear();
		recordedRigids.clear();
	}
	
	void update(ramActor &present)
	{
		if (present.getNumNode() != 0)
			recordedActors.push_back(present);
		
		if (recordedActors.size() > max_entities)
			recordedActors.pop_front();
		
		ramNodeArray &presentArray = recordedActors.back();
		ramNodeArray &pastArray = recordedActors.front();
		
		for (int i = 0; i < presentArray.getNumNode(); i++)
		{
			ofVec3f p0 = presentArray.getNode(i).getPosition();
			ofVec3f p1 = pastArray.getNode(i).getPosition();
			
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= freshness;
			ofVec3f v = p0 + d;
			ofVec3f f = ghostActor.getNode(i).getPosition();
			
			f += (v - f) * emphasis * 0.001;
			
			ghostActor.getNode(i).setPosition(f);
		}
	}
	
	void update(ramRigidBody &present)
	{
		if (present.getNumNode() != 0)
			recordedRigids.push_back(present);
		
		if (recordedRigids.size() > max_entities)
			recordedRigids.pop_front();
		
		ramNodeArray &presentArray = recordedRigids.back();
		ramNodeArray &pastArray = recordedRigids.front();
		
		for (int i = 0; i < presentArray.getNumNode(); i++)
		{
			ofVec3f p0 = presentArray.getNode(i).getPosition();
			ofVec3f p1 = pastArray.getNode(i).getPosition();
			
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= freshness;
			ofVec3f v = p0 + d;
			ofVec3f f = ghostRigidBody.getNode(i).getPosition();
			
			f += (v - f) * emphasis * 0.001;
			
			ghostRigidBody.getNode(i).setPosition(f);
		}
	}
	
	inline ramActor& getActor() { return ghostActor; }
	inline ramRigidBody& getRigidBody() { return ghostRigidBody; }
	
	inline void setFreshness(const float f) { freshness = f; }
	inline void setEmphasis(const float e) { emphasis = e; }
	inline void setMaxEntities(const unsigned int m) { max_entities = m; }
	
	inline float getFreshness() { return freshness; }
	inline float getEmphasis() { return emphasis; }
	inline unsigned int getMaxEntities() { return max_entities; }
	
protected:
	deque<ramActor> recordedActors;
	deque<ramRigidBody> recordedRigids;
	ramActor ghostActor;
	ramRigidBody ghostRigidBody;
	
	unsigned int max_entities;
	float freshness, emphasis;
};