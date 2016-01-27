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
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/toVis/meme");

    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
    position[0].x = SINGLE_SCREEN_WIDTH / 5.0;
    position[0].y = ofGetHeight() / 3.0;
    
    position[1].x = SINGLE_SCREEN_WIDTH / 2.0;
    position[1].y = ofGetHeight() / 3.0;
    
    position[2].x = SINGLE_SCREEN_WIDTH / 3.0;
    position[2].y = ofGetHeight() / 4.0;
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
    
    ofSetColor(20, 230, 240, 80);
    ofCircle(position[2], radius[2]);
}

void MEME::receiveOSC(){
    while(mReceiver.hasWaitingMessages()){
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/toVis/meme"){
            for(int i = 0; i < MEME_NUM; i++){
                int idx = m.getArgAsFloat(0);
                if(idx == i){
                    blinkSpeed[i] = m.getArgAsFloat(1);
                    blinkInterval[i] = m.getArgAsFloat(6);
                }
            }
        }
    }
}

void MEME::radiusCaculate(){
    for (int i = 0; i < MEME_NUM; i++) {
        radius[i] = 4*blinkInterval[i];
    }
}









