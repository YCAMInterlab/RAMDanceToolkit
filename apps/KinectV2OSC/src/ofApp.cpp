// 
// ofApp.cpp - KinectV2OSC
// 
// Copyright 2018 nariakiiwatani
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ofApp.h"
#include "ramActor.h"

namespace {
	int kinectToRam(int joint_id) {
		switch (joint_id) {
		case JointType_SpineBase:		return ramActor::HIPS;
		case JointType_SpineMid:		return ramActor::ABDOMEN;
		case JointType_Neck:			return ramActor::NECK;
		case JointType_Head:			return ramActor::HEAD;
		case JointType_ShoulderLeft:	return ramActor::L_SHOULDER;
		case JointType_ElbowLeft:		return ramActor::L_ELBOW;
		case JointType_WristLeft:		return ramActor::L_WRIST;
		case JointType_HandLeft:		return ramActor::L_HAND;
		case JointType_ShoulderRight:	return ramActor::R_SHOULDER;
		case JointType_ElbowRight:		return ramActor::R_ELBOW;
		case JointType_WristRight:		return ramActor::R_WRIST;
		case JointType_HandRight:		return ramActor::R_HAND;
		case JointType_HipLeft:			return ramActor::L_HIP;
		case JointType_KneeLeft:		return ramActor::L_KNEE;
		case JointType_AnkleLeft:		return ramActor::L_ANKLE;
		case JointType_FootLeft:		return ramActor::L_TOE;
		case JointType_HipRight:		return ramActor::R_HIP;
		case JointType_KneeRight:		return ramActor::R_KNEE;
		case JointType_AnkleRight:		return ramActor::R_ANKLE;
		case JointType_FootRight:		return ramActor::R_TOE;
		case JointType_SpineShoulder:	return ramActor::CHEST;
		}
		return -1;
	}
	bool shouldUseJoint(int joint_id) {
		return kinectToRam(joint_id) != -1;
	}
}
void ofApp::save()
{
	ofBuffer buf(ofToString(virtual_camera_.getGlobalTransformMatrix()));
	ofBufferToFile("settings.txt", buf);
}
void ofApp::load()
{
	ofBuffer buf = ofBufferFromFile("settings.txt");
	virtual_camera_.setTransformMatrix(ofFromString<ofMatrix4x4>(buf.getText()));
}

//--------------------------------------------------------------
void ofApp::setup(){
	if (device_.setup() && body_.setup(device_)) {
		body_.open();
	}
	sender_.setup("localhost", 10000);
	gui_.setup();
	ofBackground(0);

	load();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		preview_camera_.enableMouseInput();
	}
	else {
		preview_camera_.disableMouseInput();
	}


	device_.update();
	auto bodies = body_.getBodies();
	for (auto &b : bodies) {
		float scale = 100;
		ramActor actor;
		for (int i = 0; i < JointType_Count; ++i) {
			if (!shouldUseJoint(i)) {
				continue;
			}
			const Joint &j = b.getJoint(i);
			ofVec3f pos;
			pos.x = j.Position.X;
			pos.y = j.Position.Y;
			pos.z = j.Position.Z;
			pos *= scale;
			ofQuaternion ori;
			ori.x() = 
			ori.y() = 
			ori.z() = 0;
			ori.w() = 1;
			int ram_joint = kinectToRam(i);
			actor.nodes_[ram_joint].setPosition(pos);
			actor.nodes_[ram_joint].setOrientation(ori);
		}
		actor.collection();
		ofxOscMessage m;
		m.setAddress("/ram/skeleton");
		m.addStringArg(ofToHex(b.getId()));
		m.addIntArg(ramActor::NUM_JOINTS);
		for (int i = 0; i < ramActor::NUM_JOINTS; ++i) {
			m.addStringArg(ramActor::getJointName(i));
			auto mat = actor.nodes_[i].getGlobalTransformMatrix();
			mat.postMult(virtual_camera_.getGlobalTransformMatrix().getInverse());
			auto pos = mat.getTranslation();
			auto ori = mat.getRotate();
			m.addFloatArg(pos.x);
			m.addFloatArg(pos.y);
			m.addFloatArg(pos.z);
			m.addFloatArg(ori.x());
			m.addFloatArg(ori.y());
			m.addFloatArg(ori.z());
			m.addFloatArg(ori.w());
		}
		m.addFloatArg(ofGetElapsedTimef());
		sender_.sendMessage(m);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (view_from_virtual_camera_) {
		virtual_camera_.begin();
	}
	else {
		preview_camera_.begin();
	}
	virtual_camera_.transformGL();
	ofPushMatrix();
	ofRotateZ(90);
	ofDrawGridPlane(50);
	ofPopMatrix();
	ofDrawAxis(100);
	virtual_camera_.restoreTransformGL();

	ofPushStyle();
	ofSetColor(ofColor::yellow);
	auto bodies = body_.getBodies();
	for (auto &b : bodies) {
		for(int i = 0, num = b.getNumJoints(); i < num; ++i) {
			ofPoint p = ofPoint(b.getJoint(i).Position.X, b.getJoint(i).Position.Y, b.getJoint(i).Position.Z)*preview_scale_;
			ofDrawBox(p,5);
		}
	}
	ofPopStyle();
	if (view_from_virtual_camera_) {
		virtual_camera_.end();
	}
	else {
		preview_camera_.end();
	}


	gui_.begin();
	if (ImGui::Begin("Settings")) {
		if (ImGui::Button("Save")) {
			save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			load();
		}
		if (ImGui::Button("Reset")) {
			virtual_camera_.resetTransform();
		}
		ofVec3f move(0,0,0);
		if (ImGui::DragFloat3("Move", &move[0], 0.01f)) {
			virtual_camera_.move(move);
		}
		float roll(0);
		if (ImGui::DragFloat("Roll", &roll, 0.01f)) {
			virtual_camera_.roll(roll);
		}
		float pitch(0);
		if (ImGui::DragFloat("Pitch", &pitch, 0.01f)) {
			virtual_camera_.tilt(pitch);
		}
		float yaw(0);
		if (ImGui::DragFloat("Yaw", &yaw, 0.01f)) {
			virtual_camera_.pan(yaw);
		}
		if (ImGui::Button("Mirror")) {
			ofVec3f scale = virtual_camera_.getScale();
			scale.x *= -1;
			virtual_camera_.setScale(scale);
		}
	}
	ImGui::End();
	gui_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 's': 
		save();
		break;
	case 'l': 
		load();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
