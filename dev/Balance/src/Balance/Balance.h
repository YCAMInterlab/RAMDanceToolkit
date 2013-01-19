#pragma once


class Balance : public ramSceneBase
{
	ramBalancer balancer;
	
public:
	Balance()
	{
		sceneName = "Balance";
	}
	
	void setup()
	{
		
	}
	
	void refreshControlPanel(ofxAutoControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addSlider("master size", 10, 10, 1000);
	}
	
	void update()
	{
		ramActor &actor = getActorManager().getActor(myActorName);
		balancer.update(actor);
	}
	
	void draw()
	{
		ofPushStyle();
		
		if (getActorManager().getNumActor() > 0)
		{
			ramCameraBegin();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			
			ofVec3f balance = balancer.getAvarage().getNormalized();
			float actorHeight = 156.;
			
			ofVec3f chest = getActorManager().getActor(myActorName).getNode(ramActor::JOINT_CHEST).getPosition();
			chest.y = 0.;
			
			ofSetColor(255, 0, 0);
			ofLine(chest, balance * actorHeight);
			
			glDisable(GL_DEPTH_TEST);
			glPopAttrib();
			ramCameraEnd();
		}
		
		ofPopStyle();
	}
	
	void drawActor(ramActor &actor)
	{
		ofPushStyle();
		ofNoFill();
		ofSetColor(150);
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			
			ofBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
			
			if (node.hasParent())
				ofLine(node, *node.getParent());
		}
		
		ofPopStyle();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
		
	}
};








