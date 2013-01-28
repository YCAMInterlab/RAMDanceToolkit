#pragma once

#include "ramMain.h"

class ramBalancer
{
	
public:
	ramBalancer()
	{
		resetWeightBalance();
	}
	
	void update(ramNodeArray& nodeArray)
	{
		ofVec3f avarage;
		ofVec3f axis = nodeArray.getNode(ramActor::JOINT_CHEST).getPosition();
		avarage += axis;
		
		const int size = nodeArray.getNumNode();
		
		for (int i=0; i<size; i++)
		{
			ramNode &node = nodeArray.getNode(i);
			
			ofVec3f pos = nodeArray.getNode(i).getPosition();
			
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
