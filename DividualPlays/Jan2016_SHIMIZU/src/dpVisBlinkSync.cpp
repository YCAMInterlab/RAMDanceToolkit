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
    while(mReceiver.hasWaitingMessages()){
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        if(m.getAddress() == "/dp/toVis/meme"){
            for(int i = 0; i < MEME_NUM; i++){
                int idx = m.getArgAsFloat(0);
                if(idx == i){
                    blinkInterval[i] = m.getArgAsFloat(6);
                }
            }
        }
    }

}

void BlinkSync::radiusCaculate(){
    radius[0] = 4 * blinkInterval[0];
    radius[1] = 4 * blinkInterval[1];
}

void BlinkSync::BlinkSyncEffect(){
    for (int i = 0; i < MEME_NUM; i++) {
        if (blinkInterval[0] > 0 && blinkInterval[1] < 0.5 && blinkInterval[1] > 0 && blinkInterval[1]< 0.5) {
            printf("sync");
            radius[2] = radius[2] + 0.1;
        }
    }
}







