//
//  dpHakoniwaRemainView.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#include "dpRemainedHakoniwaView.h"

void dpRemainedHakoniwaView::setup(){
    
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/score/numHakoniwaRemained");
    mReceiver.addAddress("/dp/remainedHakoniwaView/reset");
    mEndBar.setup();
    mEndNum = 0;
    
};

void dpRemainedHakoniwaView::update(){
    receieveOsc();
    mEndBar.update();
};

void dpRemainedHakoniwaView::receieveOsc(){
    
    while(mReceiver.hasWaitingMessages()){
        
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/dp/remainedHakoniwaView/reset"){
            mEndNum = 0;
        }
        
        if(m.getAddress() == "/dp/score/numHakoniwaRemained"){
            
            int remain = m.getArgAsInt32(0);
            mNumRemain = m.getArgAsInt32(0);
            
            if(remain == 1){
                mHasEnd = true;
            }
            
            if(mHasEnd && remain != 1){
                mHasEnd = false;
                mEndNum++;
                
                showEndNum();
            }
        }
    }
    
};

void dpRemainedHakoniwaView::showEndNum(){
    mEndBar.show();
};
void dpRemainedHakoniwaView::draw(){
    
    ofPushMatrix();
    ofPoint offset = SCREEN_POSITIONS[SCREEN_C];
    ofTranslate(offset.x,offset.y);
    mEndBar.draw(mEndNum);
    ofPopMatrix();
    
    mEndBar.draw(mEndNum);
    
};
