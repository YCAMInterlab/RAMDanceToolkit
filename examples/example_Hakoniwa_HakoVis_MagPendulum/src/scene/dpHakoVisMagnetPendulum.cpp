//
//  HakoVisMagnetPendulum.cpp
//  example_hakoniwa_hakoVis
//
//  Created by kezzardrix2 on 2015/01/30.
//
//

#include "dpHakoVisMagnetPendulum.h"


void dpHakoVisMagnetPendulum::setupControlPanel(){
    
    ramGetGUI().addSlider("scale",50,1000,&mScale);
    
}

void dpHakoVisMagnetPendulum::setup(){
	mRecordTargetNum = 0;
	mBeginFrame = 0;

	mScale = 800.0;

    for(int j = 0; j < DIV_Y; j++){
        for(int i = 0; i < DIV_X; i++){
            mCircles.push_back(dpRecordBufferCircle());
            mCircles.back().setup(ofPoint((i - 1) * SPACE * 2.0 + ofGetWidth() * 0.5,
                                          j * SPACE + ofGetHeight() * 0.5 - SPACE * 0.5));
        }
    }
    
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/cameraUnit/MagPendulum/contour/boundingRect");
    
}

void dpHakoVisMagnetPendulum::record(ofPoint pt){
    
    mCircles[mRecordTargetNum].record(pt);
    
}

void dpHakoVisMagnetPendulum::changeRecordTarget(){
    
    mCircles[mRecordTargetNum].startPlayback();
    mRecordTargetNum++;
    mRecordTargetNum %= mCircles.size();
    mCircles[mRecordTargetNum].startRecord();
    
}

void dpHakoVisMagnetPendulum::receiveOsc(){
    
    while(mReceiver.hasWaitingMessages()){
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/dp/cameraUnit/MagPendulum/contour/boundingRect"){
            
            if(m.getArgAsInt32(0) != 0){
                mPos.set(ofPoint((m.getArgAsFloat(2) + m.getArgAsFloat(4) * 0.5 - 0.5) * mScale,
                                 (m.getArgAsFloat(3) + m.getArgAsFloat(5) * 0.5 - 0.5) * mScale));
                
            }
        }
    }
    
}

void dpHakoVisMagnetPendulum::update(){
    
    receiveOsc();
    record(mPos);
    
    if((ofGetFrameNum() - mBeginFrame) % dpRecordBufferCircle::BUFFER_MAX == 0)changeRecordTarget();
    
    for(auto &v:mCircles){
        v.update();
    }
    
}

void dpHakoVisMagnetPendulum::draw(){
    
    ofDisableDepthTest();
    
    ofSetCircleResolution(60);
    ofSetLineWidth(4);
    
    for(int i = 1; i < DIV_X; i++){
        for(int j = 0; j < DIV_Y; j++){
            ofLine(mCircles[j * DIV_X + i - 1].getPos(),mCircles[j * DIV_X + i].getPos());
        }
    }
    
    for(int i = 0; i < DIV_X; i++){
        ofLine(mCircles[i].getPos(),mCircles[i + DIV_X].getPos());
    }
    
    for(auto &v:mCircles){
        v.draw();
    }
    
}

void dpHakoVisMagnetPendulum::onEnabled(){
    
    mBeginFrame = ofGetFrameNum();
    
}
