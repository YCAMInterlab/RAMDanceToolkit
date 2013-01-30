#pragma once
#include "ramMain.h"


class ramGhost
{
public:
	ramGhost() :
	max_entities(10),
	distance(150),
	speed(27)
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
			
			const ofVec3f& p0 = presentArray.getNode(i).getGlobalPosition();
			const ofVec3f& p1 = pastArray.getNode(i).getGlobalPosition();
			
			// position
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= distance;
			ofVec3f v = p0 + d;
			
			ofVec3f vec = ghostActor.getNode(i).getGlobalPosition();
			vec += (v - vec) * speed * 0.001;
			
			// quaternion
			const ofQuaternion& quat = presentArray.getNode(i).getGlobalOrientation();
			
			ramNode& node = ghostActor.getNode(i);
			node.setGlobalPosition(vec);
			node.setGlobalOrientation(quat);
			node.getAccerelometer().update(vec, quat);
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
			const ofVec3f& p0 = presentArray.getNode(i).getGlobalPosition();
			const ofVec3f& p1 = pastArray.getNode(i).getGlobalPosition();
			
			// position
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= distance;
			ofVec3f v = p0 + d;
			
			ofVec3f vec = ghostRigidBody.getNode(i).getGlobalPosition();
			vec += (v - vec) * speed * 0.001;
			
			// quaternion
			const ofQuaternion& quat = presentArray.getNode(i).getGlobalOrientation();
			
			ramNode& node = ghostRigidBody.getNode(i);
			node.setPosition(vec);
			node.setOrientation(quat);
			node.getAccerelometer().update(vec, quat);
		}
	}
	
	inline ramActor& getActor() { return ghostActor; }
	inline ramRigidBody& getRigidBody() { return ghostRigidBody; }
	
	inline void setDistance(const float d) { distance = d; }
	inline void setSpeed(const float s) { speed = s; }
	inline void setMaxEntities(const unsigned int m) { max_entities = m; }
	
	inline float getdistance() { return distance; }
	inline float getspeed() { return speed; }
	inline unsigned int getMaxEntities() { return max_entities; }
	
protected:
	deque<ramActor> recordedActors;
	deque<ramRigidBody> recordedRigids;
	ramActor ghostActor;
	ramRigidBody ghostRigidBody;
	
	unsigned int max_entities;
	float distance, speed;
};