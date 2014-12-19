//
//  HakoniwaColorOfWater.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "HakoniwaColorOfWater.h"

static const int kNode0a = ramActor::JOINT_LEFT_ELBOW;
static const int kNode0b = ramActor::JOINT_RIGHT_WRIST;
static const int kNode1a = ramActor::JOINT_LEFT_ELBOW;
static const int kNode1b = ramActor::JOINT_LEFT_KNEE;
static const int kNode2a = ramActor::JOINT_RIGHT_KNEE;
static const int kNode2b = ramActor::JOINT_HEAD;

void HakoniwaColorOfWater::Valve::update(const ramNode& n0, const ramNode& n1)
{
    nodeA = n0;
    nodeB = n1;
    
    distance = n0.getGlobalPosition().distance(n1.getGlobalPosition());
    (distance >= threshould) ? (state = true) : (state = false);
}

void HakoniwaColorOfWater::Valve::draw(int color, float x, float y)
{
    ofSetCircleResolution(64);
    ofSetLineWidth(2.f);
    
    ofDisableAlphaBlending();
    
    switch (color) {
        case 0:
            ofSetColor(ofColor::yellow, 128);
            break;
        case 1:
            ofSetColor(ofColor::red, 128);
            break;
        case 2:
            ofSetColor(ofColor::blue, 128);
            break;
    }
    
    ramBeginCamera();
    ofLine(nodeA, nodeB);
    ramEndCamera();
    
    ofEnableAlphaBlending();
    
    state ? ofFill() : ofNoFill();
    ofCircle(x, y, 100);
    
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(distance), x, y);
}

HakoniwaColorOfWater::HakoniwaColorOfWater()
{
    mOscSender.setup("192.168.20.52", 8528);
    
    mValves[0].threshould = 58;
    mValves[1].threshould = 95;
    mValves[2].threshould = 103;
}

HakoniwaColorOfWater::~HakoniwaColorOfWater()
{
    
}

void HakoniwaColorOfWater::setupControlPanel()
{
}

void HakoniwaColorOfWater::update()
{
    mValves[0].update(mActor.getNode(kNode0a), mActor.getNode(kNode0b));
    mValves[1].update(mActor.getNode(kNode1a), mActor.getNode(kNode1b));
    mValves[2].update(mActor.getNode(kNode2a), mActor.getNode(kNode2b));
    
    for (int i=0; i<kNumValves; i++) {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater");
        m.addIntArg(i);
        m.addIntArg(mValves[i].state);
        mOscSender.sendMessage(m);
    }
}

void HakoniwaColorOfWater::draw()
{
    for (int i=0; i<kNumValves; i++) {
        mValves[i].draw(i, ofGetWidth()/4*(i+1), ofGetHeight()*0.5f);
    }
}


void HakoniwaColorOfWater::onPanelChanged(ofxUIEventArgs& e)
{
}

void HakoniwaColorOfWater::drawActor(const ramActor &actor)
{
    mActor = actor;
}
