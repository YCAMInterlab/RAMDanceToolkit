#pragma once

#include "ofxBt.h"


class Bullet : public ramSceneBase
{
	
public:
	
	ramBoxPrimitive *cube;
	ramSpherePrimitive *sphere;
	
	Bullet()
	{
		setSceneName("Bullet");
	}
	
	void setup()
	{
		cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
		sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
	}
	
	void update()
	{

	}
	
	void draw()
	{
		bEnabled = guiPtr->getValueB(key_enabled);
		if (!bEnabled) return;
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		ramCameraBegin();
		{
			ofNoFill();
			cube->draw();
			sphere->draw();
			ramPhysics::instance().debugDraw();
		}
		ramCameraEnd();
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	
	void drawActor(ramActor &actor)
	{
		bEnabled = guiPtr->getValueB(key_enabled);
		if (!bEnabled) return;
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		ofPushStyle();
		for (int i=0; i<actor.getNumNode(); i++)
		{
			ramNode &node = actor.getNode(i);
			
			ofNoFill();
			ofSetColor(0, 255, 0);
			ramBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
		}
		ofPopStyle();
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
		
	}
	
	
#pragma - Scene original events & methods
	
	
	void keyPressed(ofEventArgs &e)
	{
		cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
	}
};
