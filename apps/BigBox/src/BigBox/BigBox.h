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
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addSlider(key_master_size, 400, 10, 1000);
		guiPtr->addSlider(key_line_width, 3, 1, 100);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			guiPtr->addSlider(ramActor::getJointName(i), 500, 0, 1000);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		bEnabled = guiPtr->getValueB(key_enabled);
		if (!bEnabled) return;
		
		// line width
		bigBoxLineWidth = guiPtr->getValueF(key_line_width);
		
		// box size
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			jointSizes.at(i) = guiPtr->getValueF(ramActor::getJointName(i));
		
		
		
		if (guiPtr->hasValueChanged(key_master_size))
		{
			for (int i=0; i<ramActor::NUM_JOINTS; i++)
			{
				float val = guiPtr->getValueF(key_master_size);
				guiPtr->setValueF(ramActor::getJointName(i), val);
				jointSizes.at(i) = val;
			}
		}
	}
	
	void draw()
	{
		if (!bEnabled) return;
		
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
				glDisable(GL_DEPTH_TEST);
				glPushMatrix();
				{
					glMultMatrixf(getMatrix().getPtr());
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
				}
				glPopMatrix();
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

