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

	vector<string> it = ramActorManager::instance().getNodeArrayNames();
	actorList = mGui->addSortableList("actorList", it);

	mGui->setup();
	mCurrentCanvasPos = canvasPos;
	mGui->setPosition(canvasPos.x,
					  canvasPos.y);

	parentGui = ramGetGUI().getCurrentUIContext();
	parentGui->addWidget(mGui);

	mGui->autoSizeToFitWidgets();
	parentGui->autoSizeToFitWidgets();

	ofAddListener(mGui->newGUIEvent, this, &ramMotionExtractor::guiEvent);
	ofAddListener(ofEvents().mouseReleased, this, &ramMotionExtractor::mouseReleased);
}

void ramMotionExtractor::update(){

	for (int i = 0;i < mMotionPort.size();i++){
		mMotionPort[i]->update(mMotionSmooth);
	}

	//new actor

	if (lastNumNodeArray != ramActorManager::instance().getNumNodeArray()){

		vector<string> lst = ramActorManager::instance().getNodeArrayNames();
		if (lst.size() == 2) lst.push_back("Dummy");

		mGui->removeWidget(actorList);
		actorList = mGui->addSortableList("actorList", lst);

		mGui->autoSizeToFitWidgets();
		parentGui->autoSizeToFitWidgets();

	}

	lastNumNodeArray = ramActorManager::instance().getNumNodeArray();

	//actor removed

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
			mp->mActorIndex = getIndexFromName(mp->mFinder.name);

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

void ramMotionExtractor::mouseReleased(ofMouseEventArgs &arg){

	for (int i = 0;i < mMotionPort.size();i++){
		if (mMotionPort[i]->mActorIndex < actorList->getListItems().size())
			mMotionPort[i]->mFinder.name = actorList->getListItems()[mMotionPort[i]->mActorIndex]->getName();
	}

}

#pragma mark - utility

int ramMotionExtractor::getIndexFromName(string name){
	for (int i = 0;i < actorList->getListItems().size();i++){
		if (actorList->getListItems()[i]->getName() == name) return i;
	}
	return 0;
}

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
		xml.setValue("MPort:ActorIndex", mMotionPort[i]->mActorIndex, id);
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

		ramMotionPort* mp = new ramMotionPort(nodeIdent);
		mp->mActorIndex = xml.getValue("ActorIndex", 0);
		int targIndex = ofClamp(mp->mActorIndex, 0, actorList->getListItems().size()-1);

		if (targIndex < actorList->getListItems().size()){
			mp->mFinder.name = actorList->getListItems()[targIndex]->getName();
		}

		mMotionPort.push_back(mp);

		xml.popTag();
	}

}

#pragma mark - Getter

int ramMotionExtractor::getNumPort(){
	return mMotionPort.size();
}

bool ramMotionExtractor::getIsExist(int port){
	if ((0 <= port) && (port < mMotionPort.size())) return true;
	return false;
}

ramNode ramMotionExtractor::getNodeAt(int port){

	ramNode nd;

	if ((0 <= port) && (port < mMotionPort.size())){
		if (!mMotionPort[port]->isBlank){
			mMotionPort[port]->mFinder.findOne(nd);
		}
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
		if (idx != -1) return ramActor::getJointName(idx);
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

	refleshActorFromName();

	mVelocitySmoothed.set(0,0,0);
	mVelocity.set(0,0,0);
	isBlank = false;
	vecInitialize = false;

}

void ramMotionPort::update(float smooth){

	mBefNode = mCurrentNode;
	ramNode cn;
	if (!isBlank){
		mFinder.findOne(cn);
		mCurrentNode.setTransformMatrix(cn.getGlobalTransformMatrix());
		if (!vecInitialize){
			mBefNode = mCurrentNode;
			vecInitialize = true;
		}
	}

	mVelocity	= mCurrentNode.getGlobalPosition() - mBefNode.getGlobalPosition();
	mVelocitySmoothed += (mVelocity - mVelocitySmoothed) / smooth;

	mRotateVec	= mBefNode.getGlobalOrientation().inverse() * mCurrentNode.getGlobalOrientation();
	mRotateVecSmoothed.slerp(1.0 / smooth, mRotateVecSmoothed, mRotateVec);

}

void ramMotionPort::refleshActorFromIndex(){
	if ((0 <= mActorIndex) &&
		(mActorIndex < ramActorManager::instance().getNumNodeArray())){
		mFinder.name = ramActorManager::instance().getNodeArrayNames()[mActorIndex];
	}
}

void ramMotionPort::refleshActorFromName(){

}