//
//  floorline_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/02/01.
//
//

#include "floorline_ext.h"

void floorline_ext::setupControlPanel()
{
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	
	gui->addSlider("LeftShoulder",1.0,10.0,&mLShoulderScale);
	gui->addSlider("RightShoulder",1.0,10.0,&mRShoulderScale);
	
	gui->addSlider("LeftHip",1.0,10.0,&mLHipScale);
	gui->addSlider("RightHip",1.0,10.0,&mRHipScale);
	
	gui->addToggle("isSoundSphere", &mIsSphere);
	
	gui->addButton("randomizePosOnGround", false);
	gui->addButton("randomizePos", false);

	mex.setupControlPanel(this);
	ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &floorline_ext::onPanelChanged);
}

void floorline_ext::setup()
{
	for(int i = 0; i < NUM_SPHERE; i++){
		mSpheres.push_back(SoundSphere(i));
	}
	
	setupOscSenders();
}

void floorline_ext::update()
{
	for(auto &v:mSpheres){
		v.resetTouched();
	}
	mex.update();
}

void floorline_ext::draw()
{
	ramBeginCamera();
	
	if (mex.getNumPort() > 0)
		drawActorSelected(ramGlobalShortcut::getActorManager().getNodeArray(mex.getActorNameAt(0)));
	
	mex.draw();
	ramEndCamera();
}

void floorline_ext::drawActorSelected(const ramActor &actor)
{
	const ramNodeArray &array = actor;
	
	ramActor centered = mCentered.update(array);
	
	for(int j = 0; j < centered.getNumNode(); j++){
		
		ramNode &node = centered.getNode(j);
		
		if(j == ramActor::JOINT_LEFT_SHOULDER)node.setScale(mLShoulderScale);
		
		if(j == ramActor::JOINT_RIGHT_SHOULDER)node.setScale(mRShoulderScale);
		
		if(j == ramActor::JOINT_LEFT_HIP)node.setScale(mLHipScale);
		
		if(j == ramActor::JOINT_RIGHT_HIP)node.setScale(mRHipScale);
		
		ofPoint pos = node.getGlobalPosition();
		
		if(pos.y < 0.0)node.setGlobalPosition(pos.x, 0.0, pos.z);
		
	}
	
	if(mIsSphere){
		for(auto &v:mSpheres){
			v.update(centered.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition());
			v.update(centered.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition());
			v.update(centered.getNode(ramActor::JOINT_LEFT_TOE).getGlobalPosition());
			v.update(centered.getNode(ramActor::JOINT_RIGHT_TOE).getGlobalPosition());
		}
	}
	
	drawExtendActor(centered);
	drawSpheres();
}

void floorline_ext::drawSpheres()
{
	if(mIsSphere){
		ofEnableLighting();
		ofLight light;
		light.enable();
		for(auto &v:mSpheres){
			v.draw();
			v.checkTouched();
		}
		ofDisableLighting();
	}
}

void floorline_ext::drawExtendActor(ramActor &centered)
{
	for(int i = 0; i < centered.getNumNode(); i++){
		
		ramNode &node = centered.getNode(i);
		
		ofPushMatrix();
		float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
		ofDrawBox(node.getGlobalPosition(), jointSize);
		ramLine(node);
		ofPopMatrix();
	}
}

void floorline_ext::onDisabled()
{
	for(auto &v:mSpheres){
		v.reset();
	}
}

void floorline_ext::onPanelChanged(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	
	if(name == "randomizePosOnGround"){
		
		ofxUIButton *b = static_cast<ofxUIButton *>(e.widget);
		
		if(b->getValue()){
			
			for(auto &v:mSpheres){
				v.randomizePosOnGround();
			}
		}
	}
	
	if(name == "randomizePos"){
		
		ofxUIButton *b = static_cast<ofxUIButton *>(e.widget);
		
		if(b->getValue()){
			
			for(auto &v:mSpheres){
				v.randomizePos();
			}
		}
	}
}

void floorline_ext::setupOscSenders()
{
	mSenderToSound[0].setup("192.168.20.9",10000);
	mSenderToSound[1].setup("192.168.20.10",10000);
	
	for(auto &v:mSpheres){
		v.setSenders(&mSenderToSound[0], &mSenderToSound[1]);
	}
}