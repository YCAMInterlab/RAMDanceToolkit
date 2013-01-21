#pragma once
#include "ramSceneBase.h"


class BigBox : public ramSceneBase
{
	string key_master_size, key_line_width;
	
public:
	BigBox()
	{
		setSceneName("Big Box");
		
		// ---
		
		key_master_size = "Master size";
		key_line_width = "line Width";
	}
	
	void refreshControlPanel(ofxAutoControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addSlider(key_master_size, 400, 10, 1000);
		guiPtr->addSlider(key_line_width, 3, 1, 100);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			guiPtr->addSlider(ramActor::getJointName(i), 0, 0, 1000);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		if(guiPtr->hasValueChanged(key_master_size))
		{
			for (int i=0; i<23; i++)
				guiPtr->setValueF(ramActor::getJointName(i), guiPtr->getValueF(key_master_size));
			
			guiPtr->clearAllChanged();
		}
		
		if(guiPtr->hasValueChanged( getSceneKey() ))
			bEnabled = guiPtr->getValueF( getSceneKey() );
	}
	
	void draw()
	{
		if (!bEnabled) return;
		
		ramActor &actor = getActor(myActorName);
		
		float lineWidth = guiPtr->getValueF(key_line_width);
		ofColor shadowColor = ramColors[ramBaseApp::COLOR_GRAY];
		shadowColor.a = 90;
		
		ramCameraBegin();
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			float boxSize = (i==ramActor::JOINT_HEAD) ? 6 : 3;
			float bigBoxSize = guiPtr->getValueF(getJointName(i));
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			{
				ofPushStyle();
				ofNoFill();
				
				glEnable(GL_DEPTH_TEST);
				
				/*!
				 big box
				 */
				ofSetColor(ramColors[ramBaseApp::COLOR_BLUE_DEEP]);
				ofSetLineWidth(lineWidth);
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
					ofSetLineWidth(lineWidth);
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

