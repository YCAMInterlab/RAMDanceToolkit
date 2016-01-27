//
//  dpVisFrequency2.cpp
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/26/16.
//
//

#include "dpVisFrequency2.h"


void Frequency2::setup()
{
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/toVis/meme");
    
    ofSetFrameRate(60);
    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableAlphaBlending();
    
    for (int i = 0; i < 11; i++) {
        //color
        red[i] = ofRandom(0,255);
        green[i] = ofRandom(0,255);
        blue[i] = ofRandom(0,255);
        radius[i] = 10;
    }

    position[0].x = 140;
    position[0].y = ofGetHeight() / 3.0;
    position[1].x = 240;
    position[1].y = ofGetHeight() / 3.0;
    position[2].x = 340;
    position[2].y = ofGetHeight() / 3.0;
    position[3].x = 440;
    position[3].y = ofGetHeight() / 3.0;
    position[4].x = 540;
    position[4].y = ofGetHeight() / 3.0;
    position[5].x = 640;
    position[5].y = ofGetHeight() / 3.0;
    position[6].x = 740;
    position[6].y = ofGetHeight() / 3.0;
    position[7].x = 840;
    position[7].y = ofGetHeight() / 3.0;
    position[8].x = 940;
    position[8].y = ofGetHeight() / 3.0;
    position[9].x = 1040;
    position[9].y = ofGetHeight() / 3.0;
    position[10].x = 1140;
    position[10].y = ofGetHeight() / 3.0;
}

void Frequency2::update()
{
    receiveOSC();
    caculateFrequency();
    radiusCaculate();
    
}

void Frequency2::draw()
{
    for (int i = 0;i < 11; i++) {
        ofSetColor(red[i], green[i], blue[i]);
        ofCircle(position[i],frequncy_array[i] + radius[i]);
    }
}

void Frequency2::receiveOSC(){
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


void Frequency2::caculateFrequency(){
    
    for (int j = 0; j < 3; j++) {
        frequency = 1 / blinkInterval[j];
        if (blinkSpeed[j]> 0) {
            AnlyizeFrequency();
        }
    }
}
void Frequency2::AnlyizeFrequency(){
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

void Frequency2::radiusCaculate(){

//    for (int i = 0;i < 90; i++) {
//        if (i == 0) {
//            for (int j = 0; j < 11; j++) {
//                radius[j] = 10;
//            }
//        }else{
//            radius[0] = radius[0] + 0.5;
//            radius[1] = radius[1] + 1;
//        }
//    }
    
    if (j[0] > 300) {
        radius[0] = 2;
        j[0] = 0;
    }else{
        radius[0] = radius[0] + 0.5;
        j[0] = j[0] + 10;
    }
    
    if (j[1] > 260) {
        radius[1] = 2;
        j[1] = 0;
    }else{
        radius[1] = radius[1] + 0.5;
        j[1] = j[1] + 10;
    }
   
    if (j[2] > 230) {
        radius[2] = 2;
        j[2] = 0;
    }else{
        radius[2] = radius[2] + 0.5;
        j[2] = j[2] + 10;
    }
    
    if (j[3] > 200) {
        radius[3] = 2;
        j[3] = 0;
    }else{
        radius[3] = radius[3] + 0.5;
        j[3] = j[3] + 10;
    }
    
    if (j[4] > 170) {
        radius[4] = 2;
        j[4] = 0;
    }else{
        radius[4] = radius[4] + 0.5;
        j[4] = j[4] + 10;
    }
    
    if (j[5] > 140) {
        radius[5] = 2;
        j[5] = 0;
    }else{
        radius[5] = radius[5] + 0.5;
        j[5] = j[5] + 10;
    }
    
    if (j[6] > 110) {
        radius[6] = 2;
        j[6] = 0;
    }else{
        radius[6] = radius[6] + 0.5;
        j[6] = j[6] + 10;
    }
    
    if (j[7] > 80) {
        radius[7] = 2;
        j[7] = 0;
    }else{
        radius[7] = radius[7] + 0.5;
        j[7] = j[7] + 10;
    }
    
    if (j[8] > 50) {
        radius[8] = 2;
        j[8] = 0;
    }else{
        radius[8] = radius[8] + 0.5;
        j[8] = j[8] + 10;
    }
    
    if (j[9] > 40) {
        radius[9] = 2;
        j[9] = 0;
    }else{
        radius[9] = radius[9] + 0.5;
        j[9] = j[9] + 10;
    }
    
    if (j[10] > 30) {
        radius[10] = 2;
        j[10] = 0;
    }else{
        radius[10] = radius[10] + 0.5;
        j[10] = j[10] + 10;
    }
}





