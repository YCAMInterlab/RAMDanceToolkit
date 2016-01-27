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
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
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
    }
    
    velocity[0].x = 1;
    velocity[0].y = 1;
    velocity[1].x = 2;
    velocity[1].y = 2;
    velocity[2].x = 3;
    velocity[2].y = 3;
    velocity[3].x = 4;
    velocity[3].y = 4;
    velocity[4].x = 5;
    velocity[4].y = 5;
    velocity[5].x = 6;
    velocity[5].y = 6;
    velocity[6].x = 7;
    velocity[6].y = 7;
    velocity[7].x = 8;
    velocity[7].y = 8;
    velocity[8].x = 9;
    velocity[8].y = 9;
    velocity[9].x = 10;
    velocity[9].y = 10;
    velocity[10].x = 11;
    velocity[10].y = 11;
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
    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
        blinkSpeed[0] = m1.getArgAsFloat(1);
        blinkInterval[0] = m1.getArgAsFloat(6);
    }
    while (mReceiver2.hasWaitingMessages()) {
        ofxOscMessage m2;
        mReceiver2.getNextMessage(&m2);
        blinkSpeed[1] = m2.getArgAsFloat(1);
        blinkInterval[1] = m2.getArgAsFloat(6);
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









