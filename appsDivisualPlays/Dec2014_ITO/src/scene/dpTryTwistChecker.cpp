//
//  dpTryTwistChecker.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/18/15.
//
//

#include "dpTryTwistChecker.h"

void dpTryTwistChecker::setupControlPanel() {
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
//    mMotionExtractor.load("motionExt_dpHMagPendulum.xml");
    
}

void dpTryTwistChecker::setup() {
    
    //    mSenderOnOff.setup("192.168.20.52",8528);
    //    mSenderInverse.setup("192.168.20.72", 8528);
    
    bHideNodeView = true;
    
}

void dpTryTwistChecker::sendOsc() {
    
}

void dpTryTwistChecker::guiEvent(ofxUIEventArgs &e) {
    
}

void dpTryTwistChecker::update() {
    
    mMotionExtractor.update();
    
    
}

void dpTryTwistChecker::drawActor(const ramActor &actor){
    ramDrawBasicActor(actor);
}

void dpTryTwistChecker::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    
    mMotionExtractor.draw();
    ramEndCamera();
    
    ramNode n0 = mMotionExtractor.getNodeAt(0);
    float f0 = twFinder.findTwist(n0);
    ofDrawBitmapString(ofToString(f0), 500,500);
    ramNode n1 = mMotionExtractor.getNodeAt(1);
    float f1 = twFinder.findTwist(n1);
    ofDrawBitmapString(ofToString(f1), 800,500);
    
//    if (bHideNodeView) example_drawDump();
    
}




