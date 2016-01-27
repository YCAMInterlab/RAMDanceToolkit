//
//  dpVisBlinkSync.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#include "dpVisBlinkSync.h"

void BlinkSync::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
    position[0].x = ofGetWidth() / 5.0;
    position[0].y = ofGetHeight() / 3.0;
    
    position[1].x = ofGetWidth() / 2.0;
    position[1].y = ofGetHeight() / 3.0;
    
    position[2].x = ofGetWidth() / 3.0;
    position[2].y = ofGetHeight() / 4.0;
    
}

void BlinkSync::update()
{
    receiveOSC();
    radiusCaculate();
    BlinkSyncEffect();

}

void BlinkSync::draw()
{
    ofSetColor(20, 30, 240, 80);
    ofCircle(position[0], 10 + radius[0]);
    
    ofSetColor(20, 130, 240, 80);
    ofCircle(position[1], 10 + radius[1]);
    
    ofSetColor(0, 0, 240, 180);
    ofCircle(position[2], 10 + radius[2]);
}

void BlinkSync::receiveOSC(){
    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
        blinkInterval[0] = m1.getArgAsFloat(6);
    }
    while (mReceiver2.hasWaitingMessages()) {
        ofxOscMessage m2;
        mReceiver2.getNextMessage(&m2);
        blinkInterval[1] = m2.getArgAsFloat(6);
    }
}

void BlinkSync::radiusCaculate(){
    radius[0] = 4 * blinkInterval[0];
    radius[1] = 4 * blinkInterval[1];
}

void BlinkSync::BlinkSyncEffect(){
    for (int i = 0; i < NUM; i++) {
        if (blinkInterval[0] > 0 && blinkInterval[1] < 0.5 && blinkInterval[1] > 0 && blinkInterval[1]< 0.5) {
            printf("sync");
            radius[2] = radius[2] + 0.1;
        }
    }
}







