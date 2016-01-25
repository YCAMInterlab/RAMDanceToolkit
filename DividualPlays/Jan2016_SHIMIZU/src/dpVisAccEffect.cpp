//
//  dpVisAccEffect.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/21/16.
//
//

#include "dpVisAccEffect.h"


void AccEffect::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
}

void AccEffect::update()
{
    receiveOSC();

}

void AccEffect::draw()
{    
    ofBackground(255, 255, 255);

}



void AccEffect::receiveOSC(){

    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
        if (m1.getAddress() == "/dp/toVis/meme1") {
            blinkSpeed[0] = m1.getArgAsFloat(0);
            printf("blink1 %f\n", blinkSpeed[0]);
        }
    }
}






