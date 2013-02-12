#pragma once

#include "ofxBt.h"


class Bullet : public ramSceneBase
{
	
public:
	
	ramBoxPrimitive *cube;
	ramSpherePrimitive *sphere;
	
	string getSceneName() { return "Bullet"; }
	
	Bullet() {}
	
	void setup()
	{
		cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
		sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
	}
	
	void setupControlPanel(ramControlPanel& gui)
	{
	}
	
	void update()
	{

	}
	
	void draw()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		
		ramBeginCamera();
		{
			ofNoFill();
			cube->draw();
			sphere->draw();
			ramPhysics::instance().debugDraw();
		}
		ramEndCamera();
		
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	
	void drawActor(ramActor &actor)
	{
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
};
