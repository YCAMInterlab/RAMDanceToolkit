//
//  HakoniwaColorOfWater.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "HakoniwaColorOfWater.h"

void HakoniwaColorOfWater::Valve::update(const ramNode& n0, const ramNode& n1)
{
    nodeA = n0;
    nodeB = n1;
    
    distance = n0.getGlobalPosition().distance(n1.getGlobalPosition());
    pState = state;
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
    
    mValves[0].threshould = 110;
    mValves[1].threshould = 110;
    mValves[2].threshould = 110;
    
    node0a = ramActor::JOINT_LEFT_ELBOW;
    node0b = ramActor::JOINT_RIGHT_WRIST;
    node1a = ramActor::JOINT_LEFT_ELBOW;
    node1b = ramActor::JOINT_LEFT_KNEE;
    node2a = ramActor::JOINT_RIGHT_KNEE;
    node2b = ramActor::JOINT_LEFT_SHOULDER;
}

HakoniwaColorOfWater::~HakoniwaColorOfWater()
{
    
}

void HakoniwaColorOfWater::update()
{
    mValves[0].update(mActor.getNode(node0a), mActor.getNode(node0b));
    mValves[1].update(mActor.getNode(node1a), mActor.getNode(node1b));
    mValves[2].update(mActor.getNode(node2a), mActor.getNode(node2b));
    
    for (int i=0; i<kNumValves; i++) {
        if (mValves[i].stateChanged()) {
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/colorOfWater");
            m.addIntArg(i);
            m.addIntArg(mValves[i].state);
            mOscSender.sendMessage(m);
        }
    }
}

void HakoniwaColorOfWater::draw()
{
    ofDisableDepthTest();
    for (int i=0; i<kNumValves; i++) {
        mValves[i].draw(i, ofGetWidth()/4*(i+1), ofGetHeight()-150.f);
    }
}

void HakoniwaColorOfWater::setupControlPanel()
{
    ramGetGUI().addSlider("Threshould L", 40, 140, &mValves[0].threshould);
    ramGetGUI().addSlider("Threshould C", 40, 140, &mValves[1].threshould);
    ramGetGUI().addSlider("Threshould R", 40, 140, &mValves[2].threshould);
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    
    ofxUIRadio *radio = NULL;
    
    const float dim = 16.0f;
    
    panel->getRect()->width =500.0f;
    
    radio = new ofxUIRadio("L JOINT A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node0a)->setValue(true);
    panel->addWidgetDown(radio);
    
    radio = new ofxUIRadio("L JOINT B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node0b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C JOINT A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node1a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C JOINT B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node1b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R JOINT A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node2a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R JOINT B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(node2b)->setValue(true);
    panel->addWidgetRight(radio);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HakoniwaColorOfWater::onPanelChanged);
}

static int _getJointIdFromName(const string& name)
{
    const vector<string>& names = ramActor::getJointNames();
    int id = 0;
    for (int i=0; i<names.size(); i++) {
        if (names.at(i) == name) {
            id = i;
            break;
        }
    }
    return id;
}

void HakoniwaColorOfWater::onPanelChanged(ofxUIEventArgs& e)
{
    const string radioName = e.widget->getParent()->getName();
    const string toggleName = e.widget->getName();
    if (radioName == "L JOINT A") {
        node0a = _getJointIdFromName(toggleName);
    }
    else if (radioName == "L JOINT B") {
        node0b = _getJointIdFromName(toggleName);
    }
    else if (radioName == "C JOINT A") {
        node1a = _getJointIdFromName(toggleName);
    }
    else if (radioName == "C JOINT B") {
        node1b = _getJointIdFromName(toggleName);
    }
    else if (radioName == "R JOINT A") {
        node2a = _getJointIdFromName(toggleName);
    }
    else if (radioName == "R JOINT B") {
        node2b = _getJointIdFromName(toggleName);
    }
    
}

void HakoniwaColorOfWater::drawActor(const ramActor &actor)
{
    mActor = actor;
}
