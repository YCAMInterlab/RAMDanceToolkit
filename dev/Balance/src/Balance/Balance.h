#pragma once


class Balance : public ramSceneBase
{
	ramBalancer balancer;
	string key_line_width;
	
public:
	Balance()
	{
		setSceneName("Balance");
		
		// --
		
		key_line_width = "Balance line Width";
	}
	
	void setup()
	{
		
	}
	
	void refreshControlPanel(ofxAutoControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addToggle(key_enabled);
		guiPtr->addSlider(key_line_width, 1, 1, 50);
	}
	
	void update()
	{
		ramActor &actor = getActorManager().getActor(myActorName);
		balancer.update(actor);
	}
	
	void draw()
	{
		bEnabled = guiPtr->getValueB(key_enabled);
		if (!bEnabled) return;
		
		float lineWidth = guiPtr->getValueF(key_line_width);
		
		ofPushStyle();
		
		if (getActorManager().getNumActor() > 0)
		{
			ramBeginCamera();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			
			ofVec3f balance = balancer.getAvarage().getNormalized();
			float actorHeight = 156.;
			
			ofVec3f chest = getActorManager().getActor(myActorName).getNode(ramActor::JOINT_CHEST).getPosition();
			chest.y = 0.;
			
			ofSetColor(255, 0, 0);
			ofSetLineWidth(lineWidth);
			ofLine(chest, balance * actorHeight);
			
			glDisable(GL_DEPTH_TEST);
			glPopAttrib();
			ramEndCamera();
		}
		
		ofPopStyle();
	}
	
	void drawActor(ramActor &actor)
	{
		bEnabled = guiPtr->getValueB(key_enabled);
		if (!bEnabled) return;
		
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
