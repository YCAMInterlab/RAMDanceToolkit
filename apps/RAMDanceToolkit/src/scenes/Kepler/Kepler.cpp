// 
// Kepler.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
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

//
//  Kepler.cpp
//  Kepler
//
//  Created by Onishi Yoshito on 2/18/13.
//
//

#include "Kepler.h"

#pragma mark -
//--------------------------------------------------------------
static void pushAll()
{
    ofPushView();
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glMatrixMode(GL_COLOR);
    glPushMatrix();
    
    glMatrixMode(matrixMode);
    
    ofPushStyle();
}

//--------------------------------------------------------------
static void popAll()
{
    ofPopStyle();
    
    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    
    glMatrixMode(GL_COLOR);
    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    glPopAttrib();
    
    ofPopView();
}

#pragma mark -
//--------------------------------------------------------------
void Kepler::setupControlPanel()
{
	
#ifdef RAM_GUI_SYSTEM_OFXUI
	
	ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
	
    ofAddListener(panel->newGUIEvent, this, &Kepler::onValueChanged);

    ofxUIRadio *radio = NULL;
        
    const float dim = 16.0f;
    
    panel->getRect()->width =500.0f;
    
    vector<string> names;
    
    names.clear();
    mNamesNodeA.clear();
    for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++) {
        mNamesNodeA["A "+rdtk::Actor::getJointName(i)] = i;
        names.push_back("A "+rdtk::Actor::getJointName(i));
    }
    
    radio = new ofxUIRadio("JOINT A", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(rdtk::Actor::JOINT_LEFT_HAND)->setValue(true);
    panel->addWidgetDown(radio);
    
    names.clear();
    mNamesNodeB.clear();
    for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++) {
        mNamesNodeB["B "+rdtk::Actor::getJointName(i)] = i;
        names.push_back("B "+rdtk::Actor::getJointName(i));
    }
    
    radio = new ofxUIRadio("JOINT B", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(rdtk::Actor::JOINT_RIGHT_HAND)->setValue(true);
    panel->addWidgetRight(radio);
    
    names.clear();
    mNamesEdgeA.clear();
    for (int i=0; i<KeplerCube::N_EDGES; i++) {
        mNamesEdgeA["A EDGE_"+ofToString(i)] = i;
        names.push_back("A EDGE_"+ofToString(i));
    }
    
    radio = new ofxUIRadio("EDGE A", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(KeplerCube::EDGE_9)->setValue(true);
    panel->addWidgetRight(radio);
    
    names.clear();
    mNamesEdgeB.clear();
    for (int i=0; i<KeplerCube::N_EDGES; i++) {
        mNamesEdgeB["B EDGE_"+ofToString(i)] = i;
        names.push_back("B EDGE_"+ofToString(i));
    }
    
    radio = new ofxUIRadio("EDGE B", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(KeplerCube::EDGE_11)->setValue(true);
    panel->addWidgetRight(radio);
	
#endif
}

#pragma mark -
//--------------------------------------------------------------
void Kepler::setup()
{
    mKeplerDynamics.setup();
        
    mPickerA.setWorld(mKeplerDynamics.getDynamicsWorld());
    mPickerB.setWorld(mKeplerDynamics.getDynamicsWorld());
    
    mNodeA = rdtk::Actor::JOINT_LEFT_HAND;
    mNodeB = rdtk::Actor::JOINT_RIGHT_HAND;
    mEdgeA = KeplerCube::EDGE_9;
    mEdgeB = KeplerCube::EDGE_11;
    
    attach();
}

//--------------------------------------------------------------
void Kepler::update()
{
    mKeplerDynamics.update();    
}

//--------------------------------------------------------------
void Kepler::draw()
{
    pushAll();
    rdtk::BeginCamera();
    mKeplerDynamics.draw();
    rdtk::EndCamera();
    popAll();
}


#pragma mark -
//--------------------------------------------------------------
void Kepler::drawActor(const rdtk::Actor &actor)
{
    pushAll();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    for (int i=0; i<actor.getNumNode(); i++)
	{
		const rdtk::Node &node = actor.getNode(i);
		float jointSize = (i==rdtk::Actor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.beginTransform();
		ofSetColor(rdtk::Color::BLUE_LIGHT);
        ofNoFill();
		ofDrawBox(jointSize);
        if (i==mNodeA || i==mNodeB) {
            ofSetColor(rdtk::Color::RED_LIGHT);
            ofDrawBox(jointSize*1.5f);
        }
		node.endTransform();
		
		if (node.hasParent())
		{
			ofSetColor(rdtk::Color::RED_LIGHT);
			ofDrawLine(node, *node.getParent());
		}
        
	}
    
    const ofVec3f lhp = actor.getNode(mNodeA).getGlobalPosition();
    const ofVec3f rhp = actor.getNode(mNodeB).getGlobalPosition();
    mPickerA.updatePosition(btVector3(lhp.x, lhp.y, lhp.z));
    mPickerB.updatePosition(btVector3(rhp.x, rhp.y, rhp.z));
    
    popAll();
}

#pragma mark -
//--------------------------------------------------------------
void Kepler::onValueChanged(ofxUIEventArgs& e)
{
    /// todo: to dynamic cast
    ofxUIToggle *t = static_cast<ofxUIToggle *>(e.widget);
    
    string name = e.widget->getName();
    
    map<string, int>::iterator it;
    
    it = mNamesNodeA.find(name);
    
    if (it!=mNamesNodeA.end()) {
        //cout << it->first << "/" << it->second << endl;
        mNodeA = it->second;
    }
    
    it = mNamesNodeB.find(name);
    
    if (it!=mNamesNodeB.end()) {
        //cout << it->first << "/" << it->second << endl;
        mNodeB = it->second;
    }
    
    it = mNamesEdgeA.find(name);
    
    if (it!=mNamesEdgeA.end()) {
        //cout << it->first << "/" << it->second << endl;
        mEdgeA = it->second;
        attach();
    }
    
    it = mNamesEdgeB.find(name);
    
    if (it!=mNamesEdgeB.end()) {
        //cout << it->first << "/" << it->second << endl;
        mEdgeB = it->second;
        attach();
    }
    

}

#pragma mark -
//--------------------------------------------------------------
void Kepler::spawnKepler()
{
    mKeplerDynamics.keyPressed('e');
}

#pragma mark -
//--------------------------------------------------------------
void Kepler::attach()
{
    if (mKeplerDynamics.getKepleres().size()) {
        btRigidBody *bdA = mKeplerDynamics.getKepleres().at(0)->m_bodies.at(mEdgeA);
        btRigidBody *bdB = mKeplerDynamics.getKepleres().at(0)->m_bodies.at(mEdgeB);
        
        mPickerA.attatchRigidBody(bdA);
        mPickerB.attatchRigidBody(bdB);
    }
}
