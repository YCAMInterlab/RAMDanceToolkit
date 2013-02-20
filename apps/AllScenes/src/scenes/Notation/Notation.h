#pragma once

#include "ramMain.h"

float maxNotationLife = 1;

class HistoryPoint
{
public:
	ofVec3f point;
	float birth;
	
	HistoryPoint(ofVec3f point)
	:point(point)
	{
		birth = ofGetElapsedTimef();
	}
	float getLife() const
	{
		return 1. - ((ofGetElapsedTimef() - birth) / maxLabanMomentLife);
	}
	bool isDead() const
	{
		return getLife() < 0;
	}
};

bool HistoryPointIsDead(const HistoryPoint& historyPoint)
{
	return historyPoint.isDead();
}

class Notation : public ramBaseScene
{
public:
	
	map<int, list<HistoryPoint> > history;
	bool onlyLimbs;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		onlyLimbs = true;
		panel->addToggle("Only limbs", &onlyLimbs, 20, 20);
	}
	
	void setup()
	{
	}
	
	void update()
	{
		// remove any dead points
		map<int, list<HistoryPoint> >::iterator itr;
		for(itr = history.begin(); itr != history.end(); itr++)
		{
			list<HistoryPoint>& cur = itr->second;
			cur.erase(remove_if(cur.begin(), cur.end(), HistoryPointIsDead), cur.end());
		}
	}
	
	void draw()
	{	
		ramBeginCamera();
		ofEnableAlphaBlending();
		glDisable(GL_DEPTH_TEST);
		
		map<int, list<HistoryPoint> >::iterator itr;
		for(itr = history.begin(); itr != history.end(); itr++)
		{
			list<HistoryPoint>& cur = itr->second;
			ofMesh mesh;
			mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
			list<HistoryPoint>::iterator curItr;
			for(curItr = cur.begin(); curItr != cur.end(); curItr++)
			{
				mesh.addVertex(curItr->point);
			}
			mesh.draw();
		}
		
		ramEndCamera();
	}
	
	void drawActor(ramActor &actor)
	{	
		for (int i=0; i<actor.getNumNode(); i++)
		{
			if(onlyLimbs)
			{
				if(i != ramActor::JOINT_LEFT_ANKLE &&
				   i != ramActor::JOINT_RIGHT_ANKLE &&
				   i != ramActor::JOINT_LEFT_HAND &&
				   i != ramActor::JOINT_RIGHT_HAND)
				{
					continue;
				}
			}
			list<HistoryPoint>& curHistory = history[i];
			curHistory.push_back(actor.getNode(i).getGlobalPosition());
		}
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Memory"; }
	
};
