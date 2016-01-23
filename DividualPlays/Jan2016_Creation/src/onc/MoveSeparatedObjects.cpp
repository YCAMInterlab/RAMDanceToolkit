//
//  MoveSeparatedObjects.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "MoveSeparatedObjects.h"

static ofVec3f pos0A(ofRandom(-300, 300), ofRandom(100, 300), ofRandom(-300, 300));
static ofVec3f pos0B = pos0A;

static ofVec3f pos1A(ofRandom(-300, 300), ofRandom(100, 300), ofRandom(-300, 300));
static ofVec3f pos1B = pos0A;

static ofVec3f pos2A(ofRandom(-300, 300), ofRandom(100, 300), ofRandom(-300, 300));
static ofVec3f pos2B = pos0A;

MoveSeparatedObjects::MoveSeparatedObjects()
{
    
}

MoveSeparatedObjects::~MoveSeparatedObjects()
{
    
}

void MoveSeparatedObjects::setupControlPanel()
{
    
}

void MoveSeparatedObjects::update()
{
    
}

void MoveSeparatedObjects::draw()
{
    ramBeginCamera();
    
    ofNoFill();
    
    ofSetColor(255);
    
    ofLine(pos0B, ofVec3f(pos0B.x, 600, pos0B.z));
    ofDrawBox(pos0B, 20.0);
    
    ofLine(pos1B, ofVec3f(600, pos1B.y, pos1B.z));
    ofDrawBox(pos1B, 20.0);
    
    ofLine(pos2B, ofVec3f(pos2B.x, pos2B.y, 600));
    ofDrawBox(pos2B, 20.0);
    
    ramEndCamera();
}

void MoveSeparatedObjects::onPanelChanged(ofxUIEventArgs& e)
{
}

void MoveSeparatedObjects::drawActor(const ramActor &actor)
{
    pos0B = pos0A;
    ofVec3f a = actor.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition();
    pos0B.y = pos0B.y - a.y;
    
    ofLine(a, ofVec3f(a.x , 600, a.z));
    
    pos1B = pos1A;
    ofVec3f b = actor.getNode(ramActor::JOINT_RIGHT_ELBOW).getGlobalPosition();
    pos1B.x = pos1B.x - b.x;
    
    ofLine(b, ofVec3f(600 , b.y, b.z));
    
    pos2B = pos2A;
    ofVec3f c = actor.getNode(ramActor::JOINT_RIGHT_HIP).getGlobalPosition();
    pos2B.z = pos2B.z - c.z;
    
    ofLine(c, ofVec3f(b.x , b.y, 600));
}






