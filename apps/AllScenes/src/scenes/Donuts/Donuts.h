#pragma once

#include "ramActor.h"

class Donuts : public ramBaseScene
{
	
	ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];

	float mNumDuplicate;
	float mRadius;
	float mScale;
	float mBoxSize;
	ofVec3f mTranslate;
	bool mShowActor;
	
	int mNumDuplicates;
	float mRadian;
	
public:
	
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel("Original me", OFX_UI_FONT_MEDIUM));
		panel->addSlider("Scale", 0.1, 10.0, &mScale, gui.kLength, gui.kDim);
		panel->addSlider("Position: X", -300.0, 300.0, &mTranslate.x, gui.kLength, gui.kDim);
		panel->addSlider("Position: Z", -300.0, 300.0, &mTranslate.z, gui.kLength, gui.kDim);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUILabel("Duplicate me", OFX_UI_FONT_MEDIUM));
		panel->addSlider("Duplicate", 1, 200, &mNumDuplicate, gui.kLength, gui.kDim);
		panel->addSlider("Radius", 10.0, 1000.0, &mRadius, gui.kLength, gui.kDim);
		panel->addSlider("Box Size", 1.0, 1000.0, &mBoxSize, gui.kLength, gui.kDim);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Reset all settings", false, 20, 20);
		panel->addToggle("Show Actor", &mShowActor, 20, 20);
		panel->addToggle("Toggle All", false, 20, 20);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
		clear();
		
		ofAddListener(panel->newGUIEvent, this, &Donuts::onValueChanged);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		mNumDuplicate = floor(mNumDuplicate);
		mRadian = 2 * M_PI / mNumDuplicate;
	}
	
	void drawDonuts(ramNodeArray &nodeArray)
	{
		ofPushStyle();
		ofNoFill();
		
		if (mShowActor)
		{
			for (int n=0; n<mNumDuplicate; n++)
			{
				ofColor c1 = ramColor::RED_DEEP;
				ofColor c2 = ramColor::BLUE_LIGHT;
				c1.g += n*3;
				c2.b += n*3;
				
				float x = cos( mRadian * n ) * mRadius;
				float z = sin( mRadian * n ) * mRadius;
				
				ofPushMatrix();
				ofTranslate(x, 0, z);
				if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
				
				ofRotateY(360/mNumDuplicate * n);
				if( n==0 ) ofScale(mScale, mScale, mScale);
				
				if (nodeArray.isActor())
				{
					ramDrawBasicActor((ramActor&)nodeArray, c1, c2);
				}
				else
				{
					ramDrawBasicRigid((ramRigidBody&)nodeArray, c1);
				}
				
				ofPopMatrix();
			}
		}
		
		else
		{
			for (int index=0; index<nodeArray.getNumNode(); index++)
			{
				if (mNodeVisibility[index] == false) continue;
				
				for (int n=0; n<mNumDuplicate; n++)
				{
					ramNode &node = nodeArray.getNode(index);
					
					ofPushMatrix();
					{
						ofColor c1 = ramColor::RED_DEEP;
						ofColor c2 = ramColor::BLUE_LIGHT;
						c1.g += n*3;
						c2.b += n*3;
						
						float x = cos( mRadian * n ) * mRadius;
						float z = sin( mRadian * n ) * mRadius;
						
						ofMatrix4x4 m = node.getOrientationQuat();
						glMultMatrixf(m.getPtr());
						
						ofTranslate(x, 0, z);
						if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
						
						ofRotateY(360/mNumDuplicate * n);
						if( n==0 ) ofScale(mScale, mScale, mScale);
						
						node.beginTransform();
						ofSetColor(c2);
						ofBox(mBoxSize);
						node.endTransform();
					}
					ofPopMatrix();
				}
			}
		}
		
		
		ofPopStyle();

	}
	
	void draw()
	{

	}
	
	void drawActor( ramActor& actor )
	{
		drawDonuts( actor );
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		drawDonuts( rigid );
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Toggle All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			setAllVisiblity(t->getValue());
		}
		
		if (name == "Reset all settings")
		{
			clear();
		}
	}
	
	void clear()
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i]->setValue(mNodeVisibility[i]);
		}
		
		mNumDuplicate = 20;
		mRadius = 110;
		mScale = 1.0;
		mBoxSize = 3.0;
		mTranslate = ofVec3f::zero();
		mShowActor = false;
	}
	
	const string getName() { return "Donuts"; }
	
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			mToggles[i]->setValue(b);
	}
};

