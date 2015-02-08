//
//  dpHakoniwaMagnetPendulum.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#include "dpHakoniwaMagnetPendulum.h"

const ofColor dpHakoniwaMagnetPendulum::MAIN_COLOR = ofColor(255,50,150);

void dpHakoniwaMagnetPendulum::setupControlPanel() {
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    mMotionExtractor.load("motionExt_dpHMagPendulum.xml");

    for (int i = 0; i<NMAGNETS; i++ ) {
        ramGetGUI().addToggle("ON/OFF_MAGNET"+ofToString(i+1), &bOn[i]);
    }

    ramGetGUI().addSlider("Distance Threshold", 2.0f, 200.0f, &mDistanceThreshold);

    for (int i = 0; i < NMAGNETS; i++){
        bOn[i] = false;
    }
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaMagnetPendulum::guiEvent);
    
}

void dpHakoniwaMagnetPendulum::setup() {
	mDistanceThreshold = 45;
    mSenderOnOff.setup("192.168.20.52",8528);

}

void dpHakoniwaMagnetPendulum::sendOsc() {
    
    {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/MagPendulum");
        
        for (int i = 0; i < NMAGNETS; i++) {
            m.addIntArg(bOn[i]);
        }
        
        mSenderOnOff.sendMessage(m);
    }
    
}

void dpHakoniwaMagnetPendulum::guiEvent(ofxUIEventArgs &e){

}

void dpHakoniwaMagnetPendulum::onEnabled(){

}

void dpHakoniwaMagnetPendulum::onDisabled(){

    for (int i = 0; i < NMAGNETS; i++) {
        bOn[i] = false;
    }
    
    sendOsc();
}

void dpHakoniwaMagnetPendulum::update() {
    
    mMotionExtractor.update();
    
    for(int i = 0; i < NMAGNETS; i++){
        bOn[i] = mMotionExtractor.getDistanceAt(i * 2, i * 2 + 1) < mDistanceThreshold;
    }
    
    sendOsc();
}

void dpHakoniwaMagnetPendulum::drawActor(const ramActor &actor){

}

void dpHakoniwaMagnetPendulum::draw(){
    
    ramBeginCamera();
    
    {
        ofSetLineWidth(3);
        
        for(int i = 0; i < NMAGNETS; i++){
            
            float dist = mMotionExtractor.getDistanceAt(i * 2, i * 2 + 1);
            float sat = ofMap(dist,mDistanceThreshold,mDistanceThreshold * 2,255,0,true);
            
            ofColor tmpColor = MAIN_COLOR;
            
            tmpColor.setSaturation(sat);
            
            ofSetColor(tmpColor);
            
            ofLine(mMotionExtractor.getPositionAt(i * 2),
                   mMotionExtractor.getPositionAt(i * 2 + 1));
            
        }
    }
    
    ramEndCamera();
    
}
