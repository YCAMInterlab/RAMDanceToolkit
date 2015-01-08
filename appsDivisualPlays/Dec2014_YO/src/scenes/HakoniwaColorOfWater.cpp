//
//  HakoniwaColorOfWater.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "HakoniwaColorOfWater.h"

HakoniwaColorOfWater::Valve::Valve() :
on(false),
time(0.f),
state(false),
pState(false),
prevTime(0.f),
distance(0.f),
threshould(200.f),
blinkOpen(0.5f),
blinkClose(1.0f),
openingDuration(0.f),
sender(NULL),
nOpen(0),
enableOsc(false)
{
    
}

void HakoniwaColorOfWater::Valve::update(const ramNode& n0, const ramNode& n1)
{
    nodeA = n0;
    nodeB = n1;
    
    distance = n0.getGlobalPosition().distance(n1.getGlobalPosition());
    
    pState = state;
    
    time += ofGetLastFrameTime();
    
    if (!state && time - prevTime >= blinkClose) {
        state = true;
        prevTime = time;
        openingDuration = 0.f;
        
        if (distance >= threshould) {
            on = true;
        }
        
        if (on) {
            nOpen++;
            if (enableOsc) {
                ofxOscMessage m;
                m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
                m.addIntArg(1);
                sender->sendMessage(m);
            }
        }
    }
    if (state && time - prevTime >= blinkOpen) {
        state = false;
        prevTime = time;
        
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
        m.addIntArg(0);
        sender->sendMessage(m);
        
        if (distance < threshould) {
            on = false;
        }
    }
    
    if (state) {
        openingDuration += ofGetLastFrameTime();
    }
}

void HakoniwaColorOfWater::Valve::draw(int color, float x, float y)
{
    ofPushStyle();
    
    switch (color) {
        case 0:
            ofSetColor(ofColor::blue, 128);
            break;
        case 1:
            ofSetColor(ofColor::red, 128);
            break;
        case 2:
            ofSetColor(ofColor::green, 128);
            break;
    }
    
    ofSetLineWidth(2.f);
    
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    
    ramBeginCamera();
    ofLine(nodeA, nodeB);
    ramEndCamera();
    
    ofSetCircleResolution(64);
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    (on && state) ? ofFill() : ofNoFill();
    ofCircle(x, y, 100);
    
    const float lineHeight = 12.f;
    ofSetColor(ofColor::white);
    ofPushMatrix();
    ofDrawBitmapString(ofToString(distance), x, y);
    ofTranslate(0.f, lineHeight);
    ofDrawBitmapString("state: " + ofToString(state), x, y);
    ofTranslate(0.f, lineHeight);
    ofDrawBitmapString( ofToString(openingDuration), x, y);
    ofTranslate(0.f, lineHeight);
    ofDrawBitmapString( ofToString(nOpen), x, y);
    
    ofPopMatrix();
    
    ofPopStyle();
}

HakoniwaColorOfWater::HakoniwaColorOfWater()
{
    try {
        mOscSender.setup("192.168.20.60", 8528);
    }
    catch (exception& e) {
        ofLogWarning("HakoniwaColorOfWater") << e.what();
    }
    
    mNode0a = ramActor::JOINT_LEFT_HAND;
    mNode0b = ramActor::JOINT_RIGHT_TOE;
    mNode1a = ramActor::JOINT_LEFT_HAND;
    mNode1b = ramActor::JOINT_RIGHT_HAND;
    mNode2a = ramActor::JOINT_RIGHT_HAND;
    mNode2b = ramActor::JOINT_LEFT_TOE;
    
    mBlinkOpen = 0.1f;
    mBlinkClose = 1.5f;
    
    for (int i=0; i<kNumValves; i++) {
        mValves[i].pin = 6+i;
        mValves[i].sender = &mOscSender;
        mValves[i].blinkOpen = mBlinkOpen;
        mValves[i].blinkClose = mBlinkClose;
        mValves[i].threshould = 150.f;
    }
    
    mValves[0].threshould = 150.f;
    mValves[1].threshould = 100.f;
    mValves[2].threshould = 150.f;
    
    mEnableOsc = false;
}

HakoniwaColorOfWater::~HakoniwaColorOfWater()
{
    
}

void HakoniwaColorOfWater::update()
{
    mValves[0].update(mActor.getNode(mNode0a), mActor.getNode(mNode0b));
    mValves[1].update(mActor.getNode(mNode1a), mActor.getNode(mNode1b));
    mValves[2].update(mActor.getNode(mNode2a), mActor.getNode(mNode2b));
    
    for (int i=0; i<kNumValves; i++) {
        
        if ((ofGetFrameNum()+i*30) % 60 == 0) {
            if (mValves[i].state == false) {
                ofxOscMessage m;
                m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(6+i));
                m.addIntArg(0);
                mOscSender.sendMessage(m);
            }
        }
    }
}

void HakoniwaColorOfWater::draw()
{
    for (int i=0; i<kNumValves; i++) {
        mValves[i].draw(i, ofGetWidth()/4*(i+1), ofGetHeight()-150.f);
    }
}

void HakoniwaColorOfWater::exit()
{
    for (int i=0; i<kNumValves; i++) {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(6+i));
        m.addIntArg(0);
        mOscSender.sendMessage(m);
    }
}

void HakoniwaColorOfWater::setupControlPanel()
{
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    ofxUIRadio *radio = NULL;
    ofxUISlider *slider = NULL;
    const float dim = 12.0f;
    
    const float thickness = 12.f;
    const float length = 500.f;
    
    panel->addWidgetDown(new ofxUIToggle("Enable OSC", &mEnableOsc, 24.f, 24.f));
    panel->addWidgetDown(new ofxUISlider("Blink Open", 0.1f, 5.f, &mBlinkOpen, length, thickness));
    panel->addWidgetDown(new ofxUISlider("Blink Close", 0.1f, 5.f, &mBlinkClose, length, thickness));
    panel->addWidgetDown(new ofxUISpacer(0.f, 0.f, 840.f, 1.f));
    panel->addWidgetDown(new ofxUISlider("L Distance", 30, 300, &mValves[0].distance, length, thickness))->setColorFill(ofColor::blue);
    panel->addWidgetDown(new ofxUISlider("L Threshould", 30, 300, &mValves[0].threshould, length, thickness))->setColorFill(ofColor::blue);
    panel->addWidgetDown(new ofxUISlider("C Distance", 30, 300, &mValves[1].distance, length, thickness))->setColorFill(ofColor::red);
    panel->addWidgetDown(new ofxUISlider("C Threshould", 30, 300, &mValves[1].threshould, length, thickness))->setColorFill(ofColor::red);
    panel->addWidgetDown(new ofxUISlider("R Distance", 30, 300, &mValves[2].distance, length, thickness))->setColorFill(ofColor::green);
    panel->addWidgetDown(new ofxUISlider("R Threshould", 30, 300, &mValves[2].threshould, length, thickness))->setColorFill(ofColor::green);
    panel->addWidgetDown(new ofxUISpacer(0.f, 0.f, 840.f, 1.f));
    
    radio = new ofxUIRadio("L Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode0a)->setValue(true);
    panel->addWidgetDown(radio);
    
    radio = new ofxUIRadio("L Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode0b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode1a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode1b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode2a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode2b)->setValue(true);
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
    const string name = e.widget->getName();
    const string radioName = e.widget->getParent()->getName();
    
    if (radioName == "L Joint A") {
        mNode0a = _getJointIdFromName(name);
    }
    else if (radioName == "L Joint B") {
        mNode0b = _getJointIdFromName(name);
    }
    else if (radioName == "C Joint A") {
        mNode1a = _getJointIdFromName(name);
    }
    else if (radioName == "C Joint B") {
        mNode1b = _getJointIdFromName(name);
    }
    else if (radioName == "R Joint A") {
        mNode2a = _getJointIdFromName(name);
    }
    else if (radioName == "R Joint B") {
        mNode2b = _getJointIdFromName(name);
    }
    else if (name == "Blink Open") {
        for (int i=0; i<kNumValves; i++) {
            mValves[i].blinkOpen = mBlinkOpen;
        }
    }
    else if (name == "Blink Close") {
        for (int i=0; i<kNumValves; i++) {
            mValves[i].blinkClose = mBlinkClose;
        }
    }
    else if (name == "Enable OSC") {
        for (int i=0; i<kNumValves; i++) {
            mValves[i].enableOsc = mEnableOsc;
        }
    }
}

void HakoniwaColorOfWater::drawActor(const ramActor &actor)
{
    mActor = actor;
}
