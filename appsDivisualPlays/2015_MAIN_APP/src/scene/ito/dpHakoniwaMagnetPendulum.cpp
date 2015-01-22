//
//  dpHakoniwaMagnetPendulum.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#include "dpHakoniwaMagnetPendulum.h"

void dpHakoniwaMagnetPendulum::setupControlPanel() {
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    mMotionExtractor.load("motionExt_dpHMagPendulum.xml");
    ramGetGUI().addToggle("Test mode", &bTestMode);
    
    ramGetGUI().addSeparator();
    ramGetGUI().addButton("ALL MAGNETS ON");
    ramGetGUI().addButton("ALL MAGNETS OFF");
    
    for (int i = 0; i<NMAGNETS; i++ ) {
        ramGetGUI().addToggle("ON/OFF_MAGNET"+ofToString(i+1), &bOn[i]);
    }

    ramGetGUI().addSeparator();
    ramGetGUI().addButton("INVERSE ALL");
    ramGetGUI().addButton("RESTORE ALL");

    for (int i = 0; i<NMAGNETS; i++ ) {
        ramGetGUI().addToggle("INVERSE_MAGNET" + ofToString(i+1), &bInversed[i]);
    }
    
    ramGetGUI().addSlider("Distance Threshold", 2.0f, 200.0f, &distanceThreshold);
    ramGetGUI().addToggle("2EACH MODE", &bEachMode);

    ramGetGUI().addSeparator();
    ramGetGUI().addSlider("LIMIT TIME", 0, 20, &limitDuration);
    
    ramGetGUI().addToggle("TWIST MODE", &bModeTwist);
    
    for (int i = 0; i < NMAGNETS; i++){
        bInversed[i] = true;
        bOn[i] = false;
        twistVal[i] = 0;
    }
    
    for (int i = 0; i < 3; i++){
        bD[i] = false;
        bDprev[i] = false;
    }
    
    ramGetGUI().addSlider("TWIST THRESHOLD_Positive", 0, 100, &twistThresholdPositive);
    ramGetGUI().addSlider("TWIST THRESHOLD_Negative", 0, 100, &twistThresholdNegative);
    ramGetGUI().addLabel("twist mode assumes 2 nodes on each actors elbowLR");
    
    bTestMode = false;
    bEachMode = true;
    bModeTwist = false;
    
    twistThresholdPositive = 50;
    twistThresholdNegative = 50;
    limitDuration = 3.0f;
    distanceThreshold = 65;
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaMagnetPendulum::guiEvent);
    
}

void dpHakoniwaMagnetPendulum::setup() {
    
    mSenderOnOff.setup("192.168.20.52",8528);
    mSenderInverse.setup("192.168.20.72", 8528);

    bHideNodeView = true;
}

void dpHakoniwaMagnetPendulum::sendOsc() {
    
    {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/magpen");
        
        for (int i = 0; i < NMAGNETS; i++) {
            m.addIntArg(bOn[i]); // Magnets ON/OFF (1:ON, 0:OFF)
        }
        
        mSenderOnOff.sendMessage(m);
    }
    {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/magpen");
        
        for (int i = 0; i < NMAGNETS; i++) {
            m.addIntArg(bInversed[i]); // Magnets Inversed (1: Inversed, 0: Normal)
        }
        mSenderInverse.sendMessage(m);
    }
}

void dpHakoniwaMagnetPendulum::guiEvent(ofxUIEventArgs &e) {

    string name = e.widget->getName();
    
    if (name == "ALL MAGNETS ON") {
        for (int i = 0; i < NMAGNETS; i++) {
            bOn[i] = true;
        }
    } else if (name == "ALL MAGNETS OFF") {
        for (int i = 0; i < NMAGNETS; i++) {
            bOn[i] = false;
        }
    } else if (name == "INVERSE ALL") {
        for (int i = 0; i < NMAGNETS; i++) {
            bInversed[i] = true;
        }
    } else if (name == "RESTORE ALL") {
        for (int i = 0; i < NMAGNETS; i++) {
            bInversed[i] = false;
        }
    }
}

void dpHakoniwaMagnetPendulum::onEnabled(){
    startTime = ofGetElapsedTimef();
    bFirstInverseTimeDone = false;
}

void dpHakoniwaMagnetPendulum::onDisabled(){

    for (int i = 0; i < NMAGNETS; i++) {
        bOn[i] = false;
    }
    sendOsc();
}

void dpHakoniwaMagnetPendulum::update() {
    
    mMotionExtractor.update();
    
    if (!bTestMode) {
        if (!bModeTwist) {
            
            d[0] = mMotionExtractor.getDistanceAt(0, 1);
            d[1] = mMotionExtractor.getDistanceAt(2, 3);
            d[2] = mMotionExtractor.getDistanceAt(4, 5);
            
            for (int i = 0; i < 3; i++){
                if (d[i] < distanceThreshold && d[i] > 0.0f) bD[i] = true;
                else bD[i] = false;

                if (bD[i] && !bDprev[i]) {
                    startTimeForDistanceCondition[i] = ofGetElapsedTimef();
                }
            }
            
            bool bCondition[3];
            for (int i = 0; i < 3; i++){
                bCondition[i] = (bD[i] && (startTimeForDistanceCondition[i] + limitDuration > ofGetElapsedTimef()));
            }
            
            if (bEachMode) {

                if (bCondition[0]) {
                    bOn[2] = true;
                } else {
                    bOn[2] = false;
                }
                if (bCondition[1]) {
                    bOn[3] = true;
                } else {
                    bOn[3] = false;
                }
                if (bCondition[2]) {
                    bOn[4] = true;
                    bOn[5] = true;
                } else  {
                    bOn[4] = false;
                    bOn[5] = false;
                }
                
            } else {
                
                if (bCondition[0] || bCondition[1] || bCondition[2]) {
                    for (int i = 0; i < 6; i++) bOn[i] = true;
                } else {
                    for (int i = 0; i < 6; i++) bOn[i] = false;
                }
            }
            
            for (int i = 0; i < 3; i++){
                bDprev[i] = bD[i];
            }
            
        } else {   // twist mode

            ramNode r[NMAGNETS];
            float twistValOnActor[3];
            bool bTwisted[NMAGNETS];
            
            for (int i = 0; i < NMAGNETS; i++) {
                r[i] = mMotionExtractor.getNodeAt(i);
                twistVal[i] = twFinder.findTwist(r[i]);

                if ((twistVal[i] > twistThresholdPositive) || (twistVal[i] < -twistThresholdNegative)) bTwisted[i] = true;
                else bTwisted[i] = false;
            }
            
            if ((bTwisted[0] || bTwisted[1]) && (bOn[2] == false)) {
                bOn[2] = true;
            } else {
                bOn[2] = false;
            }

            if (bTwisted[2] || bTwisted[3]) {
                bOn[3] = true;
            } else {
                bOn[3] = false;
            }

            if (bTwisted[4] || bTwisted[5]) {
                bOn[4] = true;
            } else {
                bOn[4] = false;
            }
        }
        
        if (!bFirstInverseTimeDone){
            if (startTime + 1.0f > ofGetElapsedTimef()){
                for (int i = 0; i < NMAGNETS; i++){
                    bOn[i] = true;
                    bInversed[i] = false;
                }
            } else {
                bFirstInverseTimeDone = true;
                for (int i = 0; i < NMAGNETS; i++){
                    bInversed[i] = true;
                    bOn[i] = false;
                }
            }
        }
    }
    
    if (ofGetKeyPressed(49)){
        bHideNodeView = !bHideNodeView;
    }
    
    sendOsc();
}

void dpHakoniwaMagnetPendulum::drawActor(const ramActor &actor){
//    ramDrawBasicActor(actor);
}

void dpHakoniwaMagnetPendulum::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    mMotionExtractor.draw();
    ramEndCamera();
    
    if (bModeTwist) {
        
        ofPushMatrix();
        ofTranslate(1200, 0);
        for (int i = 0; i < NMAGNETS; i++){
            ofTranslate(0, 150);
            ofPushMatrix();
            drawTwistGraph(i, ofColor::red, 60);
            ofPopMatrix();
        }
        ofPopMatrix();
    } else {
        for (int i = 0; i < 3; i++){
            ofSetColor(ofColor::white);
            ofDrawBitmapString(ofToString(ofGetElapsedTimef() - startTimeForDistanceCondition[i]), 1000, 300+20*i);
        }
    }
    
    if (!bHideNodeView) example_drawDump();
    
}

void dpHakoniwaMagnetPendulum::example_drawDump(){
    
    ofPushMatrix();
    ofTranslate(800, 10);
    
    for (int i = 0;i < mMotionExtractor.getNumPort();i++){
        ofPushMatrix();
        ofTranslate(0, i*90);
        
        ofVec3f vec = mMotionExtractor.getVelocityAt(i);
        float speed = mMotionExtractor.getVelocitySpeedAt(i);
        
        ofNoFill();
        ofRect(0, 0, 200, 80);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + mMotionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + mMotionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(mMotionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, mMotionExtractor.getVelocitySpeedAt(i) * 10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

void dpHakoniwaMagnetPendulum::drawTwistGraph(int nodeID, ofColor color, float size){

    ofSetColor(255);
    ofLine(0,0, size,0);
    ofNoFill();
    ofCircle(0, 0, size);
    
    ofPushMatrix();
    ofRotate(twistThresholdNegative, 0, 0, 1);
    ofSetColor(ofColor::blue);
    ofLine(0,0,size,0);
    ofPopMatrix();

    ofPushMatrix();
    ofRotate(-twistThresholdPositive, 0, 0, 1);
    ofSetColor(ofColor::yellow);
    ofLine(0,0,size,0);
    ofPopMatrix();

    ofPushMatrix();
    ofRotate(-twistVal[nodeID], 0, 0, 1);
    ofSetColor(color);
    ofLine(0,0,size,0);
    ofPopMatrix();
    
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(twistVal[nodeID]), size*0.9,size*0.9);


}

