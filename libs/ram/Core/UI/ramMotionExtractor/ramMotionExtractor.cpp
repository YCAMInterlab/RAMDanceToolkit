//
//  ramMotionExtractor.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#include "ramMotionExtractor.h"

using namespace rdtk;

static ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

void MotionExtractor::setup(BaseScene* scene_)
{
	mMotionSmooth = 10.0;
	Preview = true;
	mScenePtr = scene_;
}

/* Call setupControlPanel of each scenes. */
void MotionExtractor::setupControlPanel(BaseScene *scene_, ofVec2f canvasPos){

	called_ofxUI = true;
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

	vector<string> it = ActorManager::instance().getNodeArrayNames();
	actorList = mGui->addSortableList("actorList", it);

	mGui->setup();
	mCurrentCanvasPos = canvasPos;
	mGui->setPosition(canvasPos.x,
					  canvasPos.y);

	parentGui = GetGUI().getCurrentUIContext();
	parentGui->addWidget(mGui);

	mGui->autoSizeToFitWidgets();
	parentGui->autoSizeToFitWidgets();

	ofAddListener(mGui->newGUIEvent, this, &MotionExtractor::guiEvent);
	ofAddListener(ofEvents().mouseReleased, this, &MotionExtractor::mouseReleased);

	receiver.addAddress("/ram/MEX/"+scene_->getName());
	OscManager::instance().addReceiverTag(&receiver);

}

void MotionExtractor::drawImGui()
{
	if (gui_floating)
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5,0.5,0.5,1.0));
		ImGui::Begin("moition Extractor", &gui_floating, ImGuiWindowFlags_AlwaysAutoResize);
		
		if (ImGui::IsWindowHovered())
			CameraManager::instance().setEnableInteractiveCamera(false);
		
		drawImGuiObject();
		
		ImGui::End();
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5,0.5,0.5,1.0));
		
		bool collapse = ImGui::CollapsingHeader("motion Extractor");
		
		if (collapse)
		{
			if (ImGui::Button("Float Window")) gui_floating = true;
			ImGui::SameLine();
			drawImGuiObject();
			ImGui::Separator();
		}
		
		ImGui::PopStyleColor();
	}
}

void MotionExtractor::drawImGuiObject()
{
	ImGui::Checkbox("Visible", &Preview);
	
	if (ImGui::Button("Push Port"))
	{
		MotionPort* mp = new MotionPort(lastSelected);
		pushPort(mp);
	}
	
	ImGui::SameLine();
	if (ImGui::Button("Pop Port"))
	{
		NodeFinder nf = lastSelected;
		popPort(nf);
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear")) clearPorts();
	
	ImGui::Spacing();
	ImGui::Text("---Actors---");
	
	vector<string> swaper = listActor.actorNames;
	bool doSwap = false;
	for (int i = 0;i < listActor.actorNames.size();i++)
	{
		ImGui::PushID(("BT"+ofToString(i)).c_str());
		string ac = listActor.actorNames[i];
		if (ImGui::Button("Up") && (i > 0))
		{
			swap(swaper[i], swaper[i-1]);
			doSwap = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Dn") && (i < listActor.actorNames.size() - 1))
		{
			swap(swaper[i], swaper[i+1]);
			doSwap = true;
		}
		ImGui::SameLine();
		ImGui::Text(ac.c_str());
		ImGui::PopID();
	}
	
	if (doSwap)
	{
		listActor.actorNames = swaper;
		refleshActorFromList();
	}
	
	if (ImGui::Button("Save"))
		save("motionExt_"+mScenePtr->getName()+".xml");
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		load("motionExt_"+mScenePtr->getName()+".xml");
	ImGui::SliderFloat("Smooth", &mMotionSmooth, 1.0, 50.0);

	if (ofGetMousePressed())
	{
		lastSelected = ActorManager::instance().getLastSelectedNodeIdentifer();
	}
}

void MotionExtractor::pushFromID(int actorId, int jointId){
	NodeIdentifer id;
	id.set(jointId);

	MotionPort *mp = new MotionPort(id);
	pushPort(mp, actorId);

	refleshActorFromList();
}

void MotionExtractor::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		string myAddr = "/ram/MEX/"+mScenePtr->getName()+"/";

		if (m.getAddress() == myAddr+"push"){// アクターID, ジョイントID
			pushFromID(m.getArgAsInt32(0), m.getArgAsInt32(1));
		}

		if (m.getAddress() == myAddr+"pop"){// 名前、ID

			NodeFinder nf;
			nf.set(m.getArgAsString(0), m.getArgAsInt32(0));
			popPort(nf);

			refleshActorFromList();

		}

		if (m.getAddress() == myAddr+"clear"){
			clearPorts();
		}

		if (m.getAddress() == myAddr+"swap"){
			actorList->swapListItems(m.getArgAsInt32(0),
									 m.getArgAsInt32(1));
			
			listActor.swapListItems(m.getArgAsInt32(0),
									m.getArgAsInt32(1));
		}

		if (m.getAddress() == myAddr+"save"){
			if (m.getArgAsString(0) == "" ||
				m.getNumArgs() == 0){
				save("motionExt_"+mScenePtr->getName()+".xml");
			}else{
				save(m.getArgAsString(0));
			}
		}

		if (m.getAddress() == myAddr+"load"){
			if (m.getArgAsString(0) == "" ||
				m.getNumArgs() == 0){
				load("motionExt_"+mScenePtr->getName()+".xml");
			}else{
				load(m.getArgAsString(0));
			}
		}

		if (m.getAddress() == myAddr+"actorList"){

			vector<string> lst;
			for (int i = 0;i < m.getNumArgs();i++){
				lst.push_back(m.getArgAsString(i));
			}

			setActorList(&lst);
	
		}

		if (m.getAddress() == myAddr + "request"){

			ofxOscSender sender;
			sender.setup(m.getRemoteIp(), 10000);

			ofxOscMessage mClr;
			mClr.setAddress("/ram/MEX/" + m.getArgAsString(0) + "/clear");

			for (int i = 0;i < mMotionPort.size();i++){
				ofxOscMessage mPsh;
				mPsh.setAddress("/ram/MEX/" + m.getArgAsString(0) + "/push");
				mPsh.addIntArg(mMotionPort[i]->mActorIndex);
				mPsh.addIntArg(mMotionPort[i]->mFinder.index);
				sender.sendMessage(mPsh);
			}

			ofxOscMessage mLs;
			mLs.setAddress("/ram/MEX/" + m.getArgAsString(0) + "/actorList");
			for (int i = 0;i < actorList->getListItems().size();i++){
				mLs.addStringArg(actorList->getListItems()[i]->getName());
			}
			sender.sendMessage(mLs);
			
		}
		
		if (m.getAddress() == myAddr + "UI"){
			ofxUIWidget *w = parentGui->getWidget(m.getArgAsString(0));
			if (w != NULL){
				
				if (w->getKind() == OFX_UI_WIDGET_SLIDER_H ||
					w->getKind() == OFX_UI_WIDGET_SLIDER_V){
					((ofxUISlider*)(w))->setValue(m.getArgAsFloat(1));
				}
				
				if (w->getKind() == OFX_UI_WIDGET_TOGGLE){
					
					((ofxUIToggle*)(w))->setValue(m.getArgAsInt32(1));
					
				}
				
				if (w->getKind() == OFX_UI_WIDGET_BUTTON){
					((ofxUIButton*)(w))->triggerSelf();
				}
				
			}
		}
	}

	for (int i = 0;i < mMotionPort.size();i++){
		mMotionPort[i]->update(mMotionSmooth);
	}

	//new actor

	if (lastNumNodeArray != ActorManager::instance().getNumNodeArray()){

		vector<string> lst = ActorManager::instance().getNodeArrayNames();
		
		listActor.actorNames = lst;
		
		if (lst.size() == 2) lst.push_back("Dummy");

		if (called_ofxUI)
		{
			mGui->removeWidget(actorList);
			actorList = mGui->addSortableList("actorList", lst);
			
			mGui->autoSizeToFitWidgets();
			parentGui->autoSizeToFitWidgets();
		}
		

	}

	lastNumNodeArray = ActorManager::instance().getNumNodeArray();

	//actor removed

}

void MotionExtractor::draw(){
	if (!Preview) return;
	
	for (int i = 0;i < ActorManager::instance().getNumNodeArray();i++){
		NodeArray arr = ActorManager::instance().getNodeArray(i);
		ofVec3f tp = arr.getNode(Actor::JOINT_HEAD).getGlobalPosition();

		ofDrawBitmapString(ActorManager::instance().getNodeArrayNames()[i], tp);
	}

	for (int i = 0;i < mMotionPort.size();i++){
		if (!mMotionPort[i]->isBlank){
			ofSetColor(255);

			ofPushMatrix();
			ofTranslate(mMotionPort[i]->mCurrentNode.getGlobalPosition());

			ofSetColor(255,0,0);
			glLineWidth(3.0);
			ofDrawLine(ofVec3f(0,0,0),
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

void MotionExtractor::guiEvent(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "PushPort"){
		if (w->getState() == OFX_UI_STATE_DOWN){
			MotionPort* mp = new MotionPort(ActorManager::instance().getLastSelectedNodeIdentifer());
			pushPort(mp);
		}
	}

	if (w->getName() == "PopPort"){
		if (w->getState() == OFX_UI_STATE_DOWN){
			NodeFinder nf = ActorManager::instance().getLastSelectedNodeIdentifer();
			popPort(nf);
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

void MotionExtractor::pushPort(MotionPort *mp, int actorId){

	if (actorId == -1) mp->mActorIndex = getIndexFromName(mp->mFinder.name);
	else mp->mActorIndex = actorId;

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

	Node nd;
	if (mp->mFinder.findOne(nd) && !isDuplicate && isNoBlank){
		mMotionPort.push_back(mp);
	}else{
		if (mp != NULL) delete mp;
	}

}

void MotionExtractor::popPort(NodeFinder &nf){

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

void MotionExtractor::mouseReleased(ofMouseEventArgs &arg){

	refleshActorFromList();
	
}

void MotionExtractor::refleshActorFromList(){
	for (int i = 0;i < mMotionPort.size();i++){
		
		if (called_ofxUI)
		{
			if (mMotionPort[i]->mActorIndex < actorList->getListItems().size())
				mMotionPort[i]->mFinder.name = actorList->getListItems()[mMotionPort[i]->mActorIndex]->getName();
		}
		else
		{
			if (mMotionPort[i]->mActorIndex < listActor.actorNames.size())
				mMotionPort[i]->mFinder.name = listActor.actorNames[mMotionPort[i]->mActorIndex];
		}
	}
}

#pragma mark - utility

int MotionExtractor::getIndexFromName(string name){
	
	if (called_ofxUI)
	{
		for (int i = 0;i < actorList->getListItems().size();i++)
			if (actorList->getListItems()[i]->getName() == name) return i;
	}
	else
	{
		for (int i = 0;i < listActor.actorNames.size();i++)
			if (listActor.actorNames[i] == name) return i;
	}

	return 0;
}

void MotionExtractor::clearPorts(){
	while (mMotionPort.size() > 0){
		MotionPort* pt = mMotionPort[0];
		mMotionPort.erase(mMotionPort.begin());
		delete pt;
	}
}

void MotionExtractor::save(string file){

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

void MotionExtractor::load(string file){

	ofxXmlSettings xml;
	xml.load(file);

	clearPorts();
	mMotionSmooth = xml.getValue("Smooth", 10.0);
	for (int i = 0;i < xml.getNumTags("MPort");i++){
		NodeIdentifer nodeIdent;

		xml.pushTag("MPort",i);
		nodeIdent.set(xml.getValue("Name", ""),
					  xml.getValue("Joint", -1));

		MotionPort* mp = new MotionPort(nodeIdent);
		mp->mActorIndex = xml.getValue("ActorIndex", 0);
		int targIndex;
		
		if (called_ofxUI)
		{
			targIndex = ofClamp(mp->mActorIndex, 0, actorList->getListItems().size()-1);
			
			if (targIndex < actorList->getListItems().size())
				mp->mFinder.name = actorList->getListItems()[targIndex]->getName();

		}
		else
		{
			targIndex = ofClamp(mp->mActorIndex, 0, listActor.actorNames.size() - 1);

			if (targIndex < listActor.actorNames.size())
				mp->mFinder.name = listActor.actorNames[targIndex];

		}

		mMotionPort.push_back(mp);

		xml.popTag();
	}

}

#pragma mark - Getter

int MotionExtractor::getNumPort(){
	return mMotionPort.size();
}

bool MotionExtractor::getIsExist(int port){
	if ((0 <= port) && (port < mMotionPort.size())) return true;
	return false;
}

Node MotionExtractor::getNodeAt(int port){

	Node nd;

	if ((0 <= port) && (port < mMotionPort.size())){
		if (!mMotionPort[port]->isBlank){
			mMotionPort[port]->mFinder.findOne(nd);
		}
	}

	return nd;
}

string MotionExtractor::getActorNameAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		return mMotionPort[port]->mFinder.name;
	}

	return "N/A";
}

string MotionExtractor::getJointNameAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		int idx = mMotionPort[port]->mFinder.index;
		if (idx != -1) return Actor::getJointName(idx);
	}

	return "N/A";
}

int MotionExtractor::getJointIdAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		return mMotionPort[port]->mFinder.index;
	}
	return -1;

}

ofVec3f MotionExtractor::getPositionAt(int port,bool fixPosition){
    
    vector <string> names = ActorManager::instance().getNodeArrayNames();
    bool exist = false;
    string actorName = getActorNameAt(port);
    for (int i = 0;i < names.size();i++){
        if (names[i] == actorName) exist = true;
    }
    if (exist){
        NodeArray nd = ActorManager::instance().getNodeArray(getActorNameAt(port));
        ofVec3f pos = getNodeAt(port).getGlobalPosition();
        ofVec3f abd = nd.getNode(Actor::JOINT_ABDOMEN).getGlobalPosition();
        abd.y = 0;
        if (fixPosition){
            pos -= abd;
        }
        return pos;
    }else{
        return getNodeAt(port).getGlobalPosition();
    }
	

}

ofQuaternion MotionExtractor::getRotationAt(int port){

	return getNodeAt(port).getGlobalOrientation();

}

ofVec3f MotionExtractor::getVelocityAt(int port){

	ofVec3f v = ofVec3f(0,0,0);

	if ((0 <= port) && (port < mMotionPort.size())){
		v = mMotionPort[port]->mVelocitySmoothed;
	}

	return v;
}

float MotionExtractor::getVelocitySpeedAt(int port){
	return getVelocityAt(port).length();
}

ofQuaternion MotionExtractor::getRotateVelocityAt(int port){

	ofQuaternion q = ofQuaternion(0,0,0,0);

	if ((0 <= port) && (port < mMotionPort.size())){
		q = mMotionPort[port]->mRotateVecSmoothed;
	}

	return q;
}

float MotionExtractor::getDistanceAt(int port_A, int port_B){

	ofVec3f v1 = ofVec3f(0,0,0);
	ofVec3f v2 = ofVec3f(0,0,0);

	if ((0 <= port_A) && (port_A < mMotionPort.size()) &&
		(0 <= port_B) && (port_B < mMotionPort.size())){

		v1 = getPositionAt(port_A);
		v2 = getPositionAt(port_B);

		return (v1 - v2).length();

	}else{

		return 0.0f;

	}

}

float MotionExtractor::getAreaAt(int port_A, int port_B, int port_C){
	ofVec3f v1 = ofVec3f(0,0,0);
	ofVec3f v2 = ofVec3f(0,0,0);
	ofVec3f v3 = ofVec3f(0,0,0);
	
	if ((0 <= port_A) && (port_A < mMotionPort.size()) &&
		(0 <= port_B) && (port_B < mMotionPort.size()) &&
		(0 <= port_C) && (port_C < mMotionPort.size())){
		
		v1 = getPositionAt(port_A);
		v2 = getPositionAt(port_B);
		v3 = getPositionAt(port_C);
		
		ofVec3f mid = v1.interpolate(v2, 0.5);
		float area = v1.distance(v2) * mid.distance(v3) / 2.0;
		
		return area;
		
	}else{
		
		return 0.0f;
		
	}

}

void MotionExtractor::setActorList(vector<string> *lst){

	if (lst->size() == 2) lst->push_back("Dummy");
	
	if (called_ofxUI)
	{
		mGui->removeWidget(actorList);
		actorList = mGui->addSortableList("actorList", *lst);
		
		mGui->autoSizeToFitWidgets();
		parentGui->autoSizeToFitWidgets();
	}
	else
	{
		listActor.actorNames = *lst;
	}
	
	refleshActorFromList();

}

int MotionExtractor::getActorIndexAt(int port){

	if ((0 <= port) && (port < mMotionPort.size())){
		return mMotionPort[port]->mActorIndex;
	}
	return -1;

}

#pragma mark - motionPort

void MotionPort::init(NodeFinder nodeF){

	mFinder = nodeF;

	refleshActorFromName();

	mVelocitySmoothed.set(0,0,0);
	mVelocity.set(0,0,0);
	isBlank = false;
	vecInitialize = false;

}

void MotionPort::update(float smooth){

	mBefNode = mCurrentNode;
	Node cn;
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

void MotionPort::refleshActorFromIndex(){
	if ((0 <= mActorIndex) &&
		(mActorIndex < ActorManager::instance().getNumNodeArray())){
		mFinder.name = ActorManager::instance().getNodeArrayNames()[mActorIndex];
	}
}

void MotionPort::refleshActorFromName(){

}

