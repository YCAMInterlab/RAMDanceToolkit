// 
// ramGhost.h - RAMDanceToolkit
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

#include "ramBaseFilter.h"
#include "ramControlPanel.h"
#include "ramLowPassFilter.h"

class ramGhost : public ramBaseFilter
{
public:

	string getName() const { return "ramGhost"; };

	ramGhost() : historySize(10), distance(150), speed(27)
	{
		clear();
	}

	void clear()
	{
		record.clear();
	}

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

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());

		ofAddListener(ramGetGUI().addButton("Ghost"), this, &ramGhost::onPresetGhost);
		ofAddListener(ramGetGUI().addButton("Slow"), this, &ramGhost::onPresetSlow);
		ofAddListener(ramGetGUI().addButton("Normal"), this, &ramGhost::onPresetNormal);
		ofAddListener(ramGetGUI().addButton("Fast"), this, &ramGhost::onPresetFast);

		ramGetGUI().addSlider("Distance", 0.0, 255.0, &distance);
		ramGetGUI().addSlider("Speed", 0.0, 255.0, &speed);
	}
	
	void onPresetGhost(ofEventArgs &e)
	{
		setSpeed(1.5);
		setDistance(240);
	}

	void onPresetSlow(ofEventArgs &e)
	{
		setSpeed(8.3);
		setDistance(74.4);
	}
	
	void onPresetNormal(ofEventArgs &e)
	{
		setSpeed(9.4);
		setDistance(150);
	}
	
	void onPresetFast(ofEventArgs &e)
	{
		setSpeed(38.9);
		setDistance(211);
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
			node.getAccelerometer().update(vec, quat);
		}

		return ghost;
	}

};