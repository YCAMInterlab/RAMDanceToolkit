#pragma once
#include "ramSceneBase.h"


class BigBox : public ramSceneBase
{
	string key_master_size, key_line_width;
	
public:
	
	/*!
		Scene params
	 */
	vector<float> jointSizes;
	float bigBoxLineWidth;
	
	BigBox()
	{
		setSceneName("Big Box");
		key_master_size = "Master size";
		key_line_width = "Line Width";
		
		// ---
		
		bigBoxLineWidth = 2.0;
		
		jointSizes.clear();
		jointSizes.resize(ramActor::NUM_JOINTS);
		for (int i=0; i<jointSizes.size(); i++) jointSizes.at(i) = 300.0;
	}
	
	void setupControlPanel(ramControlPanel& gui)
	{
		gui.addPanel(getSceneName());
		gui.addSlider(key_master_size, 400, 10, 1000);
		gui.addSlider(key_line_width, 3, 1, 100);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			gui.addSlider(ramActor::getJointName(i), 500, 0, 1000);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		// line width
		bigBoxLineWidth = gui().getValueF(key_line_width);
		
		// box size
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			jointSizes.at(i) = gui().getValueF(ramActor::getJointName(i));
		
		
		
		if (gui().hasValueChanged(key_master_size))
		{
			for (int i=0; i<ramActor::NUM_JOINTS; i++)
			{
				float val = gui().getValueF(key_master_size);
				gui().setValueF(ramActor::getJointName(i), val);
				jointSizes.at(i) = val;
			}
		}
	}
	
	void draw()
	{
		ramActor &actor = getActor(myActorName);
		ofColor shadowColor = ramColor::GRAY;
		shadowColor.a = 90;
		
		ramCameraBegin();
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			float boxSize = (i==ramActor::JOINT_HEAD) ? 6 : 3;
			float bigBoxSize = jointSizes.at(i);
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			{
				ofPushStyle();
				ofNoFill();
				
				glEnable(GL_DEPTH_TEST);
				
				/*!
				 big box
				 */
				ofSetColor( ramColor::BLUE_DEEP );
				ofSetLineWidth(bigBoxLineWidth);
				node.transformBegin();
				ofBox(bigBoxSize);
				node.transformEnd();
				
				
				/*!
				 shadows
				 */
				
				ramGlobal().beginShadowMatrix();
				
				glDisable(GL_DEPTH_TEST);
				ofEnableAlphaBlending();
				ofSetColor(shadowColor);
				
				node.transformBegin();
				ofSetLineWidth(bigBoxLineWidth);
				ofBox(bigBoxSize);
				node.transformEnd();
				
				ofSetLineWidth(1);
				ofBox(node, boxSize);
				if (node.hasParent())
					ofLine(node, *node.getParent());
				
				ramGlobal().endShadowMatrix();
				
				ofPopStyle();
			}
			glPopMatrix();
			glPopAttrib();
		}
		
		ramCameraEnd();
	}
	
	
	void drawActor(ramActor& actor)
	{
		
	}
	
	void drawRigidBody(ramRigidBody& rigid)
	{
		
	}
	void drawFloor()
	{
		
	}
};

