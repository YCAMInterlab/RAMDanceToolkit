//
//  ramMotionExtractor.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#include "ramMotionExtractor.h"

ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

/* Call setupControlPanel of each scenes. */
void ramMotionExtractor::setupControlPanel(ramBaseScene *scene_, ofVec2f canvasPos){

	mMotionSmooth = 10.0;
	mScenePtr = scene_;

	mGui = new ofxUICanvas();
	mGui->disableAppDrawCallback();
	mGui->disableMouseEventCallbacks();
	mGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
					 uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	mGui->addLabel("motionExtractor");
	mGui->addButton("Save", false);
	mGui->addButton("Load", false);
	mGui->addSpacer();
	mGui->addButton("PushPort", false);
	mGui->addButton("PopPort", false);
	mGui->addButton("Clear", false);
	mGui->addSlider("Smooth", 1.0, 50.0, &mMotionSmooth);

	mGui->setup();
	mGui->setPosition(canvasPos.x,
					  canvasPos.y);

	ofxUICanvasPlus* panel = ramGetGUI().getCurrentUIContext();
	panel->addWidget(mGui);
	ofAddListener(mGui->newGUIEvent, this, &ramMotionExtractor::guiEvent);

}

void ramMotionExtractor::update(){
	for (int i = 0;i < mMotionPort.size();i++){
		mMotionPort[i]->update(mMotionSmooth);
	}
}

void ramMotionExtractor::draw(){
	for (int i = 0;i < mMotionPort.size();i++){
		if (!mMotionPort[i]->isBlank){
			ofSetColor(255);

			ofPushMatrix();
			ofTranslate(mMotionPort[i]->mCurrentNode.getGlobalPosition());

			ofSetColor(255,0,0);
			glLineWidth(3.0);
			ofLine(ofVec3f(0,0,0),
				   mMotionPort[i]->mVelocitySmoothed * 30.0);
			glLineWidth(1.0);

			ofSetColor(255);
			ofDrawBitmapString("Port :" + ofToString(i), 0, 5);

			ofPopMatrix();

			ofPushMatrix();
			ofMultMatrix(mMotionPort[i]->mCurrentNode.getGlobalTransformMatrix());
			ofNoFill();
			ofDrawBox(15);
			ofFill();

			ofMatrix4x4 rotMat;
			mMotionPort[i]->mRotateVecSmoothed.get(rotMat);
			ofMultMatrix(rotMat);
			ofDrawAxis(15.0);
			ofPopMatrix();
		}
	}
}

void ramMotionExtractor::guiEvent(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "PushPort"){
		if (w->getState() == OFX_UI_STATE_DOWN){
			ramMotionPort* mp = new ramMotionPort(ramActorManager::instance().getLastSelectedNodeIdentifer());

			bool isDuplicate = false;
			bool isNoBlank = true;
			for (int i = 0;i < mMotionPort.size();i++){
				if ((mp->mFinder.name == mMotionPort[i]->mFinder.name) &&
					(mp->mFinder.index == mMotionPort[i]->mFinder.index)){
					isDuplicate = true;
					break;
				}

				if (mMotionPort[i]->isBlank){
					mMotionPort[i]->init(mp->mFinder);
					isNoBlank = false;
					break;
				}
			}

			ramNode nd;
			if (mp->mFinder.findOne(nd) && !isDuplicate && isNoBlank){
				mMotionPort.push_back(mp);
			}else{
				delete mp;
			}
		}
	}

	if (w->getName() == "PopPort"){
		if (w->getState() == OFX_UI_STATE_DOWN){
			ramNodeFinder nf = ramActorManager::instance().getLastSelectedNodeIdentifer();

			for (int i = 0;i < mMotionPort.size();i++){
				if ((nf.name == mMotionPort[i]->mFinder.name) &&
					(nf.index == mMotionPort[i]->mFinder.index)){
					mMotionPort[i]->isBlank = true;
					mMotionPort[i]->mFinder.setTargetName("");
					mMotionPort[i]->mFinder.setJointID(-1);
					break;
				}
			}
			
		}
	}

	if ((w->getName() == "Clear") && (w->getState() == OFX_UI_STATE_DOWN)){
		clearPorts();
	}

	if ((w->getName() == "Save") && (w->getState() == OFX_UI_STATE_DOWN)){
		save("motionExt_"+mScenePtr->getName()+".xml");
	}

	if ((w->getName() == "Load") && (w->getState() == OFX_UI_STATE_DOWN)){
		load("motionExt_"+mScenePtr->getName()+".xml");
	}

}

#pragma mark - utility

void ramMotionExtractor::clearPorts(){
	while (mMotionPort.size() > 0){
		ramMotionPort* pt = mMotionPort[0];
		mMotionPort.erase(mMotionPort.begin());
		delete pt;
	}
}

void ramMotionExtractor::save(string file){

	ofxXmlSettings xml;

	xml.setValue("Smooth", mMotionSmooth);
	for (int i = 0;i < mMotionPort.size();i++){
		int id = xml.addTag("MPort");
		xml.setValue("MPort:Name", mMotionPort[i]->mFinder.name, id);
		xml.setValue("MPort:Joint", mMotionPort[i]->mFinder.index, id);
	}

	xml.save(file);

}

void ramMotionExtractor::load(string file){

	ofxXmlSettings xml;
	xml.load(file);

	clearPorts();
	mMotionSmooth = xml.getValue("Smooth", 10.0);
	for (int i = 0;i < xml.getNumTags("MPort");i++){
		ramNodeIdentifer nodeIdent;

		xml.pushTag("MPort",i);
		nodeIdent.set(xml.getValue("Name", ""),
					  xml.getValue("Joint", -1));
		xml.popTag();

		ramMotionPort* mp = new ramMotionPort(nodeIdent);
		mMotionPort.push_back(mp);
	}

}

#pragma mark - Getter

int ramMotionExtractor::getNumPort(){
	return mMotionPort.size();
}

ramNode ramMotionExtractor::getNodeAt(int port){

	ramNode nd;

	if ((0 <= port) && (port < mMotionPort.size())){
		mMotionPort[port]->mFinder.findOne(nd);
	}

	return nd;
}

string ramMotionExtractor::getActorNameAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		return mMotionPort[port]->mFinder.name;
	}

	return "N/A";
}

string ramMotionExtractor::getJointNameAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		int idx = mMotionPort[port]->mFinder.index;

		if (idx == ramActor::JOINT_ABDOMEN) return "Abdomen";
		if (idx == ramActor::JOINT_CHEST)	return "Chest";
		if (idx == ramActor::JOINT_HEAD)	return "Head";
		if (idx == ramActor::JOINT_HIPS)	return "Hips";
		if (idx == ramActor::JOINT_LEFT_ANKLE)		return "Left_Ankle";
		if (idx == ramActor::JOINT_LEFT_COLLAR)		return "Left_Collar";
		if (idx == ramActor::JOINT_LEFT_ELBOW)		return "Left_Elbow";
		if (idx == ramActor::JOINT_LEFT_HAND)		return "Left_Hand";
		if (idx == ramActor::JOINT_LEFT_HIP)		return "Left_Hip";
		if (idx == ramActor::JOINT_LEFT_KNEE)		return "Left_Knee";
		if (idx == ramActor::JOINT_LEFT_SHOULDER)	return "Left_Shoulder";
		if (idx == ramActor::JOINT_LEFT_TOE)		return "Left_Toe";
		if (idx == ramActor::JOINT_LEFT_WRIST)		return "Left_Wrist";
		if (idx == ramActor::JOINT_NECK)			return "Neck";
		if (idx == ramActor::JOINT_RIGHT_ANKLE)		return "Right_Ankle";
		if (idx == ramActor::JOINT_RIGHT_COLLAR)	return "Right_Collar";
		if (idx == ramActor::JOINT_RIGHT_ELBOW)		return "Right_Elbow";
		if (idx == ramActor::JOINT_RIGHT_HAND)		return "Right_Hand";
		if (idx == ramActor::JOINT_RIGHT_HIP)		return "Right_Hip";
		if (idx == ramActor::JOINT_RIGHT_KNEE)		return "Right_Knee";
		if (idx == ramActor::JOINT_RIGHT_SHOULDER)	return "Right_Shoulder";
		if (idx == ramActor::JOINT_RIGHT_TOE)		return "Right_Toe";
		if (idx == ramActor::JOINT_RIGHT_WRIST)		return "Right_Wrist";

	}

	return "N/A";
}

int ramMotionExtractor::getJointIdAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		return mMotionPort[port]->mFinder.index;
	}
	return -1;

}

ofVec3f ramMotionExtractor::getPositionAt(int port){

	return getNodeAt(port).getGlobalPosition();

}

ofQuaternion ramMotionExtractor::getRotationAt(int port){

	return getNodeAt(port).getGlobalOrientation();

}

ofVec3f ramMotionExtractor::getVelocityAt(int port){

	ofVec3f v = ofVec3f(0,0,0);

	if ((0 <= port) && (port < mMotionPort.size())){
		v = mMotionPort[port]->mVelocitySmoothed;
	}

	return v;
}

float ramMotionExtractor::getVelocitySpeedAt(int port){
	return getVelocityAt(port).length();
}

ofQuaternion ramMotionExtractor::getRotateVelocityAt(int port){

	ofQuaternion q = ofQuaternion(0,0,0,0);

	if ((0 <= port) && (port < mMotionPort.size())){
		q = mMotionPort[port]->mRotateVecSmoothed;
	}

	return q;
}

float ramMotionExtractor::getDistanceAt(int port_A, int port_B){

	ofVec3f v1 = ofVec3f(0,0,0);
	ofVec3f v2 = ofVec3f(0,0,0);

	if ((0 < port_A) && (port_A < mMotionPort.size()) &&
		(0 < port_B) && (port_B < mMotionPort.size())){

		return (v1 - v2).length();

	}else{

		return 0.0f;

	}

}

#pragma mark - motionPort

void ramMotionPort::init(ramNodeFinder nodeF){

	mFinder = nodeF;
	mVelocitySmoothed.set(0,0,0);
	mVelocity.set(0,0,0);
	isBlank = false;
	vecInitialize = false;

}

void ramMotionPort::update(float smooth){

	mBefNode = mCurrentNode;
	ramNode cn;
	mFinder.findOne(cn);
	mCurrentNode.setTransformMatrix(cn.getGlobalTransformMatrix());
	if (!vecInitialize){
		mBefNode = mCurrentNode;
		vecInitialize = true;
	}

	mVelocity	= mCurrentNode.getGlobalPosition() - mBefNode.getGlobalPosition();
	mVelocitySmoothed += (mVelocity - mVelocitySmoothed) / smooth;

	mRotateVec	= mBefNode.getGlobalOrientation().inverse() * mCurrentNode.getGlobalOrientation();
	mRotateVecSmoothed.slerp(1.0 / smooth, mRotateVecSmoothed, mRotateVec);

}
