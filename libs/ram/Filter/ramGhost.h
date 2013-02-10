#pragma once

#include "ramMain.h"

#include "ramFilter.h"

class ramGhost : public ramFilter
{
public:
	
	ramGhost() : historySize(10), distance(150), speed(27)
	{
		clear();
	}
	
	void clear()
	{
		record.clear();
	}
	
	const ramNodeArray& update(const ramNodeArray &present)
	{
		if (present.getNumNode() != 0)
			record.push_back(present);
		
		if (record.size() > historySize)
			record.pop_front();
		
		ramNodeArray &presentArray = record.back();
		ramNodeArray &pastArray = record.front();
		
		for (int i = 0; i < presentArray.getNumNode(); i++)
		{
			
			const ofVec3f& p0 = presentArray.getNode(i).getGlobalPosition();
			const ofVec3f& p1 = pastArray.getNode(i).getGlobalPosition();
			
			// position
			ofVec3f d = (p0 - p1);
			d.normalize();
			d *= distance;
			ofVec3f v = p0 + d;
			
			ofVec3f vec = ghost.getNode(i).getGlobalPosition();
			vec += (v - vec) * speed * 0.001;
			
			// quaternion
			const ofQuaternion& quat = presentArray.getNode(i).getGlobalOrientation();
			
			ramNode& node = ghost.getNode(i);
			node.setGlobalPosition(vec);
			node.setGlobalOrientation(quat);
			node.getAccerelometer().update(vec, quat);
		}
		
		return ghost;
	}
	
	inline const ramNodeArray& getResult() { return ghost; }
	
	inline void setDistance(const float d) { distance = d; }
	inline void setSpeed(const float s) { speed = s; }
	inline void setHistorySize(const unsigned int m) { historySize = m; }
	
	inline float getdistance() { return distance; }
	inline float getspeed() { return speed; }
	inline unsigned int getHistorySize() { return historySize; }
	
protected:
	ramNodeArray ghost;
	deque<ramNodeArray> record;
	
	unsigned int historySize;
	float distance, speed;
};