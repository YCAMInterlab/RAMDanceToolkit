// 
// ramRecognizer.h - RAMDanceToolkit
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

#include "ramBaseRecognizer.h"
#include "ramNodeFinder.h"
#include "ramTimeEvent.h"

class ramBalancer : public ramBaseRecognizer
{

public:
	ramBalancer()
	{
		resetWeightBalance();
	}

	inline ofVec3f getAverage() { return balance; }

private:

	ofVec3f balance;

	vector<float> weightBalance;

	float calcAngle2D(ofVec3f v1, ofVec3f v2)
	{
		float angle = ofRadToDeg(atan((v2.z - v1.z) / (v2.x - v1.x)));

		if (v2.z < v1.z && v2.x > v1.x)
			return angle;

		else if ((v2.z < v1.z && v2.x < v1.x) || (v2.z > v1.z && v2.x < v1.x))
			return angle + 180.;

		else
			return angle + 360.;
	}

	void resetWeightBalance()
	{
		weightBalance.clear();
		weightBalance.resize(ramActor::NUM_JOINTS);

		// cite: http://goo.gl/QcylU
		weightBalance.at(ramActor::JOINT_ABDOMEN) = 0.05;
		weightBalance.at(ramActor::JOINT_CHEST) = 0.05;
		weightBalance.at(ramActor::JOINT_NECK) = 0.05;
		weightBalance.at(ramActor::JOINT_HEAD) = 0.10;

		weightBalance.at(ramActor::JOINT_LEFT_HIP) = 0.05;
		weightBalance.at(ramActor::JOINT_LEFT_KNEE) = 0.045;
		weightBalance.at(ramActor::JOINT_LEFT_ANKLE) = 0.05;
		weightBalance.at(ramActor::JOINT_LEFT_TOE) = 0.015;

		weightBalance.at(ramActor::JOINT_RIGHT_HIP) = 0.05;
		weightBalance.at(ramActor::JOINT_RIGHT_KNEE) = 0.045;
		weightBalance.at(ramActor::JOINT_RIGHT_ANKLE) = 0.05;
		weightBalance.at(ramActor::JOINT_RIGHT_TOE) = 0.015;

		weightBalance.at(ramActor::JOINT_LEFT_COLLAR) = 0.04;
		weightBalance.at(ramActor::JOINT_LEFT_SHOULDER) = 0.04;
		weightBalance.at(ramActor::JOINT_LEFT_ELBOW) = 0.03;
		weightBalance.at(ramActor::JOINT_LEFT_WRIST) = 0.02;
		weightBalance.at(ramActor::JOINT_LEFT_HAND) = 0.01;

		weightBalance.at(ramActor::JOINT_RIGHT_COLLAR) = 0.04;
		weightBalance.at(ramActor::JOINT_RIGHT_SHOULDER) = 0.04;
		weightBalance.at(ramActor::JOINT_RIGHT_ELBOW) = 0.03;
		weightBalance.at(ramActor::JOINT_RIGHT_WRIST) = 0.02;
		weightBalance.at(ramActor::JOINT_RIGHT_HAND) = 0.01;
	}

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		ofVec3f avarage;
		ofVec3f axis = src.getNode(ramActor::JOINT_CHEST).getGlobalPosition();
		avarage += axis;

		const int size = src.getNumNode();

		for (int i = 0; i < size; i++)
		{
			ofVec3f pos = src.getNode(i).getGlobalPosition();

			ofVec3f dist(pos - axis);
			dist *= weightBalance.at(i);

			avarage.x += dist.x;
			avarage.y += dist.y;
			avarage.z += dist.z;
		}
		avarage.x /= size;
		avarage.y /= size;
		avarage.z /= size;

		balance = avarage;

		return src;
	}
};

//

// TODO: move to events
class ramMovementAnalyser : public ramNodeFinder, public ramBaseRecognizer
{
public:

	string getName() const { return "ramMovementAnalyser"; }

	ramMovementAnalyser() : threshold(2), state(false) {}

	void setThreshold(float v) { threshold = v; }
	float getThreshold() { return threshold; }

	virtual float update()
	{
		if (!found()) return 0;

		float total_dist = 0;
		vector<ramNode> found_nodes = findAll();

		if (found_nodes.size())
		{
			center_pos = found_nodes[0];

			for (int i = 0; i < found_nodes.size(); i++)
			{
				total_dist += found_nodes[i].getVelocity().length();
			}

			current_value += (log(total_dist + 1) - current_value) * 0.5;
		}

		bool b = current_value > threshold;
		if (b != state)
		{
			state = b;
			if (b) onMove();
			else onStop();
		}

		return total_dist;
	}

	virtual void draw()
	{
		glPushMatrix();
		ofPushStyle();
		
		ofTranslate(center_pos);
		ramBillboard();

		ofColor c = ofGetStyle().color;

		ofSetColor(c, c.a * 0.3);
		ofNoFill();
		ofCircle(0, 0, current_value * 10);

		ofSetColor(c, c.a * 0.5);
		ofNoFill();
		ofCircle(0, 0, threshold * 10);
		
		ofPopStyle();
		glPopMatrix();
	}

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());
		ramGetGUI().addSlider("Threshold", 0.1, 10, &threshold);
	}

protected:

	ofVec3f center_pos;

	float threshold;
	float current_value;

	bool state;

	virtual void onMove() {}
	virtual void onStop() {}

	bool getState() { return state; }

};

class ramTimerdMovementAnalyser : public ramMovementAnalyser
{
public:

	string getName() const { return "ramTimerdMovementAnalyser"; }

	ramTimerdMovementAnalyser() : hold_state(false)
	{
		timer.setDuration(1);
	}

	void setTime(float t) { timer.setDuration(t); }
	float getTime() { return timer.getDuration(); }

	virtual float update()
	{
		float dist = ramMovementAnalyser::update();

		timer.setDuration(hold_time);

		if (hold_state == getState())
		{
			timer.reset();
		}
		else
		{
			if (timer.update())
			{
				hold_state = getState();

				if (getState())
				{
					onTimerdMove();
				}
				else
				{
					onTimerdStop();
				}
			}
		}

		return dist;
	}

	void draw()
	{
		ramMovementAnalyser::draw();

		glPushMatrix();
		ofPushStyle();

		ofTranslate(center_pos);
		ramBillboard();

		ofColor c = ofGetStyle().color;

		ofSetColor(c, c.a * 0.5);
		ofFill();
		ofCircle(0, 0, 10 * getThreshold() * timer.getProgress());
		
		ofPopStyle();
		glPopMatrix();
	}

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());
		ramGetGUI().addSlider("Threshold", 0.1, 10, &threshold);
		ramGetGUI().addSlider("Hold time", 0.1, 10, &hold_time);
	}

protected:

	virtual void onTimerdMove() {}
	virtual void onTimerdStop() {}

private:

	bool hold_state;
	float hold_time;

	ramScheduledTimerEvent timer;

};