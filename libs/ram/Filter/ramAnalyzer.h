#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"
#include "ramNodeFinder.h"

class ramBalancer : public ramBaseFilter
{
	
public:
	ramBalancer()
	{
		resetWeightBalance();
	}
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		ofVec3f avarage;
		ofVec3f axis = src.getNode(ramActor::JOINT_CHEST).getGlobalPosition();
		avarage += axis;
		
		const int size = src.getNumNode();
		
		for (int i=0; i<size; i++)
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
	
	inline ofVec3f getAvarage() { return balance; }
	inline ofVec3f getAngle() { return balance; }
	
private:
	
	ofVec3f balance;
	
	vector<float> weightBalance;
	
	float calcAngle2D(ofVec3f v1, ofVec3f v2)
	{
		float angle = ofRadToDeg( atan((v2.z - v1.z)/(v2.x - v1.x)) );
		
		if (v2.z < v1.z && v2.x > v1.x)
			return angle;
		
		else if((v2.z < v1.z && v2.x < v1.x) || (v2.z > v1.z && v2.x < v1.x))
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
};

//

class ramMovementAnalyser : public ramNodeFinder
{
public:
	
	ramMovementAnalyser() : threshold(2), state(false) {}
	
	void setThreshold(float v) { threshold = v; }
	float getThreshold() { return threshold; }
	
	virtual float update()
	{
		if (!found()) return 0;
		
		float total_dist = 0;
		vector<ramNode> found_nodes = get();
		
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
		
		ofTranslate(center_pos);
		billboard();
		
		ofColor c = ofGetStyle().color;
		
		ofSetColor(c, c.a * 0.3);
		ofNoFill();
		ofCircle(0, 0, current_value * 10);
		
		ofSetColor(c, c.a * 0.5);
		ofNoFill();
		ofCircle(0, 0, threshold * 10);
		
		glPopMatrix();
	}
	
protected:
	
	ofVec3f center_pos;
	
	inline void billboard()
	{
		ofMatrix4x4 m;
		glGetFloatv(GL_MODELVIEW_MATRIX, m.getPtr());
		
		ofVec3f s = m.getScale();
		
		m(0, 0) = s.x;
		m(0, 1) = 0;
		m(0, 2) = 0;
		
		m(1, 0) = 0;
		m(1, 1) = s.y;
		m(1, 2) = 0;
		
		m(2, 0) = 0;
		m(2, 1) = 0;
		m(2, 2) = s.z;
		
		glLoadMatrixf(m.getPtr());
	}
	
	virtual void onMove() {}
	virtual void onStop() {}
	
	bool getState() { return state; }
	
private:
	
	float threshold;
	float current_value;
	
	bool state;
	
};

class ramTimerdMovementAnalyser : public ramMovementAnalyser
{
public:
	
	ramTimerdMovementAnalyser() : hold_time(1), current_time(0) {}
	
	void setTime(float t) { hold_time = t; }
	float getTime() { return hold_time; }
	
	virtual float update()
	{
		float dist = ramMovementAnalyser::update();
		
		if (hold_state == getState())
		{
			current_time = 0;
		}
		else
		{
			current_time += ofGetLastFrameTime();
			
			if (current_time > hold_time
				&& hold_state != getState())
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
		
		ofTranslate(center_pos);
		billboard();
		
		ofColor c = ofGetStyle().color;
		
		ofSetColor(c, c.a * 0.5);
		ofFill();
		ofCircle(0, 0, 10 * getThreshold() * (current_time / hold_time));
		
		glPopMatrix();

	}
	
protected:
	
	virtual void onTimerdMove() {}
	virtual void onTimerdStop() {}
	
private:
	
	bool hold_state;
	float hold_time;
	float current_time;
	
};
