//
//  dpVisMEME.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#include "dpVisMEME.h"

void MEME::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
        position[0].x = ofGetWidth() / 4.0;
        position[0].y = ofGetHeight() / 3.0;
    
        position[1].x = ofGetWidth() / 2.0;
        position[1].y = ofGetHeight() / 3.0;
}

void MEME::update()
{
    receiveOSC();
    radiusCaculate();
}

void MEME::draw()
{
    ofSetColor(20, 30, 240, 80);
    ofCircle(position[0], radius[0]);
    
    ofSetColor(20, 130, 240, 80);
    ofCircle(position[1], radius[1]);
}

void MEME::receiveOSC(){
    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
            blinkSpeed[0] = m1.getArgAsFloat(1);
            blinkInterval[0] = m1.getArgAsFloat(6);
            printf("%f\n",blinkInterval[0]);
    }
    while (mReceiver2.hasWaitingMessages()) {
        ofxOscMessage m2;
        mReceiver2.getNextMessage(&m2);
            blinkSpeed[1] = m2.getArgAsFloat(1);
            blinkInterval[1] = m2.getArgAsFloat(6);
    }
}

void MEME::radiusCaculate(){
    radius[0] = 4 * blinkInterval[0];
    radius[1] = 4 * blinkInterval[1];
}









