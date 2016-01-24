//
//  dpVisBlinkEffect.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/21/16.
//
//

#include "dpVisBlinkEffect.h"

void BlinkEffect::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver1);
    mReceiver1.addAddress("/dp/toVis/meme1");
    
    ramOscManager::instance().addReceiverTag(&mReceiver2);
    mReceiver2.addAddress("/dp/toVis/meme2");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    
    for (int i = 0; i < NUM; i++) {
        position[i].x = ofRandom(ofGetWidth());
        position[i].y = ofRandom(ofGetHeight());
        velocity[i].x = ofRandom(-10,10);
        velocity[i].y = ofRandom(-10,10);
        
        //color
        red[i] = ofRandom(0,255);
        green[i] = ofRandom(0,255);
        blue[i] = ofRandom(0,255);
    }
    
}

void BlinkEffect::update()
{
    receiveOSC();
    blinkDetection();
    
    for (int i = 0; i < NUM; i++) {
        position[i] += velocity[i];
        
        if (position[i].x < 0 || position[i].x > ofGetWidth()) {
            velocity[i].x *= -1;
        }
        if (position[i].y < 0 || position[i].y > ofGetHeight()) {
            velocity[i].y *= -1;
        }
    }
}

void BlinkEffect::draw()
{
    
    //    ofSetHexColor(0x3399cc);
    //
    //    for (int i = 0; i < NUM; i++) {
    //        ofCircle(position[i], 30);
    //    }
    
    ofBackground(255, 255, 255);
    blinkDetection();
    
}



void BlinkEffect::receiveOSC(){
    while (mReceiver1.hasWaitingMessages()) {
        ofxOscMessage m1;
        mReceiver1.getNextMessage(&m1);
        
        if (m1.getAddress() == "/dp/toVis/meme1") {
            blinkSpeed[0] = m1.getArgAsFloat(0);
            blinkStrength[0] = m1.getArgAsFloat(1);
            blinkInterval[0] = m1.getArgAsFloat(5);
            printf("blink1 %f\n", blinkSpeed[0]);

        }
    }
    
    while (mReceiver2.hasWaitingMessages()) {
        ofxOscMessage m2;
        mReceiver2.getNextMessage(&m2);
        
        if (m2.getAddress() == "/dp/toVis/meme2") {
            blinkSpeed[1] = m2.getArgAsFloat(0);
            blinkStrength[1] = m2.getArgAsFloat(1);
            blinkInterval[1] = m2.getArgAsFloat(5);
        }
    }
}

void BlinkEffect::blinkDetection(){
    
    if (blinkSpeed[0] > 0.0) {
        velocity[0].x = ofRandom(-10,10);
        velocity[0].y = ofRandom(-10,10);
        ofSetColor(red[0], green[0], blue[0]);
        ofCircle(position[0], 40);
        printf("blink1 %f\n", blinkSpeed[0]);
        
    }else{
        ofSetColor(red[0], green[0], blue[0]);
        ofCircle(position[0], 30);
    }
    
    if (blinkSpeed[1] > 0.0) {
        velocity[1].x = ofRandom(-10,10);
        velocity[1].y = ofRandom(-10,10);
        ofSetColor(red[1], green[1], blue[1]);
        ofCircle(position[1], 40);
        printf("blink2 %f\n", blinkSpeed[1]);
        
    }else{
        ofSetColor(red[1], green[1], blue[1]);
        ofCircle(position[1], 30);
    }
}







