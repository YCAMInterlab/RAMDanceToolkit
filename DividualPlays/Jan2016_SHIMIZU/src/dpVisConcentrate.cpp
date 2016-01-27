//
//  dpVisConcentrate.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#include "dpVisConcentrate.h"

void Concentrate::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    
    position[0].x = ofGetWidth() / 4.0;
    position[0].y = ofGetHeight() / 3.0;
    
    position[1].x = ofGetWidth() / 2.0;
    position[1].y = ofGetHeight() / 3.0;
}

void Concentrate::update()
{
    receiveOSC();
    
}

void Concentrate::draw()
{
    ofBackground(255, 255, 255);
    SceneChange();
}


void Concentrate::receiveOSC(){
    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
        ConcentrateLevel[0] = m1.getArgAsFloat(7);
    }
    while (mReceiver2.hasWaitingMessages()) {
        ofxOscMessage m2;
        mReceiver1.getNextMessage(&m2);
        ConcentrateLevel[1] = m2.getArgAsFloat(7);
    }
}

void Concentrate::SceneChange(){
    if (ConcentrateLevel[0] > 0) {
        ofSetColor(20, 30, 240, 80);
        ofCircle(position[0], 100);
    }else{
        ofCircle(position[0], 0);
    }
    
    if (ConcentrateLevel[1] < 0) {
        ofSetColor(20, 30, 240, 80);
        ofCircle(position[1], 100);
    }else{
        ofCircle(position[1], 0);
    }
}








