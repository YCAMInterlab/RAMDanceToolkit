//
//  dpVisBlinkFrequency.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#include "dpVisBlinkFrequency.h"


void BlinkFrequency::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/toVis/meme");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
    for (int i = 0; i < 11; i++) {
        position[i].x = ofRandom(ofGetWidth());
        position[i].y = ofRandom(ofGetHeight());
        //color
        red[i] = ofRandom(0,255);
        green[i] = ofRandom(0,255);
        blue[i] = ofRandom(0,255);
        
        velocity[i].x = i;
        velocity[i].y = i;
      }
    
}

void BlinkFrequency::update()
{
    receiveOSC();
    caculateFrequency();
    
    for (int i = 0; i < 11; i++) {
        position[i] += velocity[i];
        if (position[i].x < 0 || position[i].x > ofGetWidth()) {
            velocity[i].x *= -1;
        }
        if (position[i].y < 0 || position[i].y > ofGetHeight()) {
            velocity[i].y *= -1;
        }
    }
}

void BlinkFrequency::draw()
{
    for (int i = 0;i < 11; i++) {
        ofSetColor(red[i], green[i], blue[i]);
        ofCircle(position[i],4*frequncy_array[i]);
    }
    
}

void BlinkFrequency::receiveOSC(){
    while(mReceiver.hasWaitingMessages()){
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        if(m.getAddress() == "/dp/toVis/meme"){
            for(int i = 0; i < MEME_NUM; i++){
                int idx = m.getArgAsFloat(0);
                if(idx == i){
                    blinkSpeed[i] = m.getArgAsFloat(1);
                    blinkInterval[i] = m.getArgAsFloat(6);
                    printf("%f\n",blinkSpeed[0]);
                }
            }
        }
    }
}


void BlinkFrequency::caculateFrequency(){
    
    frequency = 1 / blinkInterval[0];
    if (blinkSpeed[0]> 0) {
        if (frequency < 0.1) {
            frequncy_array[0] ++ ;
        }else if(frequency >= 0.1 && frequency <0.2){
            frequncy_array[1] ++ ;
        }else if (frequency >= 0.2 && frequency < 0.3){
            frequncy_array[2] ++ ;
        }else if (frequency >= 0.3 && frequency < 0.4){
            frequncy_array[3] ++ ;
        }else if (frequency >= 0.4 && frequency < 0.5){
            frequncy_array[4] ++ ;
        }else if (frequency >= 0.5 && frequency < 0.6){
            frequncy_array[5] ++ ;
        }else if (frequency >= 0.6 && frequency < 0.7){
            frequncy_array[6] ++ ;
        }else if (frequency >= 0.7 && frequency < 0.8){
            frequncy_array[7] ++ ;
        }else if (frequency >= 0.8 && frequency < 0.9){
            frequncy_array[8] ++ ;
        }else if (frequency >= 0.9 && frequency < 1.0){
            frequncy_array[9] ++ ;
        }else if (frequency >= 1.0){
            frequncy_array[10] ++ ;
        }
    }
}









