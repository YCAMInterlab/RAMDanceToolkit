//
//  dpVisBlinkSyncAlgorithm.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/27/16.
//
//

#include "dpVisBlinkSyncAlgorithm.h"


void BlinkSyncAlgorithm::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/toVis/meme");
    
    sender.setup(HOST, PORT);
    
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
}

void BlinkSyncAlgorithm::update()
{
    receiveOSC();
    BlinkSyncEffect();
}

void BlinkSyncAlgorithm::draw()
{

}


void BlinkSyncAlgorithm::receiveOSC(){
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


void BlinkSyncAlgorithm::BlinkSyncEffect(){
    for (int i = 0; i < MEME_NUM; i++) {
        if (0.01 < blinkInterval[0] && blinkInterval[0]< 0.4 && 0.01 < blinkInterval[1] && blinkInterval[1] < 0.4) {
            ofxOscMessage m;
            m.setAddress("/dp/light/meme");
            m.addIntArg(0);
            m.addIntArg(100);
            sender.sendMessage(m);
            printf("sync");
        }
    }
}












