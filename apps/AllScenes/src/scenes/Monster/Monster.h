#pragma once

#include "ramMain.h"

class Monster : public ramBaseScene
{
public:
	
	bool reset, randomizeTopology, randomizeGeometry, swapLeftRight, swapArmsLegs;
	ramMonster monster;
		
	void setupControlPanel(ofxUICanvas* panel)
	{
		randomizeTopology = false;
		randomizeGeometry = false;
		panel->addButton("Reset", &reset, 40, 40);
		panel->addButton("Randomize Topology", &randomizeTopology, 40, 40);
		panel->addButton("Randomize Geometry", &randomizeGeometry, 40, 40);
		panel->addButton("Swap Left/Right", &swapLeftRight, 40, 40);
		panel->addButton("Swap Arms/Legs", &swapArmsLegs, 40, 40);
	}
	
	void setup()
	{
		monster.setup();
	}
	
	void update()
	{
		if(reset)
		{
			monster.reset();
		}
		if(randomizeTopology) 
		{
			monster.randomizeTopology();
		}
		if(randomizeGeometry) 
		{
			monster.randomizeGeometry();
		}
		if(swapLeftRight)
		{
			monster.reset();
			monster.attach(ramActor::JOINT_LEFT_KNEE, ramActor::JOINT_RIGHT_HIP);
			monster.attach(ramActor::JOINT_RIGHT_KNEE, ramActor::JOINT_LEFT_HIP);
			monster.attach(ramActor::JOINT_LEFT_ELBOW, ramActor::JOINT_RIGHT_SHOULDER);
			monster.attach(ramActor::JOINT_RIGHT_ELBOW, ramActor::JOINT_LEFT_SHOULDER);
		}
		if(swapArmsLegs)
		{
			monster.reset();
			monster.attach(ramActor::JOINT_LEFT_KNEE, ramActor::JOINT_LEFT_SHOULDER);
			monster.attach(ramActor::JOINT_RIGHT_KNEE, ramActor::JOINT_RIGHT_SHOULDER);
			monster.attach(ramActor::JOINT_LEFT_ELBOW, ramActor::JOINT_LEFT_HIP);
			monster.attach(ramActor::JOINT_RIGHT_ELBOW, ramActor::JOINT_RIGHT_HIP);
		}
	}
	
	void draw()
	{	
	}
	
	void drawActor(ramActor &actor)
	{	
		ramActor &monsterActor = (ramActor&) monster.update(actor);
		
		ofPushStyle();
		ofNoFill();
		for (int i=0; i<monsterActor.getNumNode(); i++)
		{
			ramNode &node = monsterActor.getNode(i);
			ofSetColor(255);
			ofSetLineWidth(2);
			if(node.hasParent())
			{
				ofLine(node, *node.getParent());
			}
		}
		ofPopStyle();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Monster"; }
	
};
