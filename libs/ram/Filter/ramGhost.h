#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"
#include "ramControlPanel.h"

class ramGhost : public ramBaseFilter
{
public:
	
	const string getName() { return "ramGhost"; };
	
	ramGhost() : historySize(10), distance(150), speed(27)
	{
		clear();
	}
	
	void clear()
	{
		record.clear();
	}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();
		
		gui.addSection(getName());
		
		struct Preset
		{
			ramGhost *self;
			float distance;
			float speed;
			
			Preset(ramGhost *self, float distance, float speed) : self(self), distance(distance), speed(speed) {}
			void operator()()
			{
				self->setSpeed(speed);
				self->setDistance(distance);
			}
		};
		
		gui.addButton("Ghost", Preset(this, 1.5, 240));
		gui.addButton("Slow", Preset(this, 8.3, 74.4));
		gui.addButton("Normal", Preset(this, 9.4, 150));
		gui.addButton("Fast", Preset(this, 38.9, 211.1));
		
		gui.addSlider("Distance", 0.0, 255.0, &distance);
		gui.addSlider("Speed", 0.0, 255.0, &speed);
	}
	
	inline void setDistance(const float d) { distance = d; }
	inline void setSpeed(const float s) { speed = s; }
	inline void setHistorySize(const unsigned int m) { historySize = m; }
	
	inline float getDistance() { return distance; }
	inline float getSpeed() { return speed; }
	inline unsigned int getHistorySize() { return historySize; }
	
protected:
	ramNodeArray ghost;
	deque<ramNodeArray> record;
	
	int historySize;
	float distance, speed;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		if (src.getNumNode() != 0)
			record.push_back(src);
		
		if (ghost.getNumNode() != src.getNumNode())
			ghost = src;
		
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

};