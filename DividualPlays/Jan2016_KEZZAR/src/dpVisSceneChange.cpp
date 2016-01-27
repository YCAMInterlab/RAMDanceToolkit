//
//  dpVisSceneChange.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/27/16.
//
//

#include "dpVisSceneChange.h"

void SceneChange::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/toVis/Scene");
    
    sender.setup(HOST, PORT);
    
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
}

void SceneChange::update()
{
    receiveOSC();
    BlinkSyncEffect();
}

void SceneChange::draw()
{
    
}


void SceneChange::receiveOSC(){
    while(mReceiver.hasWaitingMessages()){
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        if(m.getAddress() == "/dp/toVis/Scene"){
            for(int i = 0; i < MEME_NUM; i++){
                int idx = m.getArgAsFloat(0);
                if(idx == i){
                    sceneChange[i] = m.getArgAsFloat(1);
                    printf("%f\n",sceneChange[0]);
                }
            }
        }
    }
}


void SceneChange::BlinkSyncEffect(){
    
    for (int i = 0; i < MEME_NUM; i++) {
        if (sceneChange[0] > 0 && sceneChange[1] > 0 && sceneChange[2] > 0) {
            ofxOscMessage m;
            m.setAddress("/dp/toVis/SceneChange");
            m.addIntArg(100); // sceneの切り替えメッセージ
            sender.sendMessage(m);
            printf("SceneChange");
        }
    }
    
    
}

