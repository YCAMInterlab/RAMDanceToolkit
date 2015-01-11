#include "dpHakoniwaSand.h"

void dpHakoniwaSand::setupControlPanel(){
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    ramGetGUI().addIntSlider("Val1_Test", 1000, 2000, &val1);
    ramGetGUI().addIntSlider("Val2_Test", 1000, 2000, &val2);
    ramGetGUI().addIntSlider("Val3_Test", 1000, 2000, &val3);
    ramGetGUI().addToggle("Test mode", &bTestMode);
    
//    bTestMode = false;
    
}

void dpHakoniwaSand::setup(){
 
    mSender.setup("192.168.20.54",8528);
}

void dpHakoniwaSand::sendOsc(){
    
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/sand");
    
    if (bTestMode) {
        m.addIntArg(val1); // pulse width(usec) for motor1
        m.addIntArg(val2); // pulse width(usec) for motor2
        m.addIntArg(val3); // pulse width(usec) for motor3
    } else {
        m.addIntArg((int)ofMap(mMotionExtractor.getPositionAt(0).y, 0, 200, 1000, 2000)); // pulse width(usec) for motor1
        m.addIntArg((int)ofMap(mMotionExtractor.getPositionAt(1).y, 0, 200, 1000, 2000)); // pulse width(usec) for motor1
        m.addIntArg((int)ofMap(mMotionExtractor.getPositionAt(2).y, 0, 200, 1000, 2000)); // pulse width(usec) for motor1
    }
    mSender.sendMessage(m);

//    cout << val1 <<", "<< val2 << ", " << val3 << endl;
    
}

void dpHakoniwaSand::update(){

    mMotionExtractor.update();
//    mVelocitySpeed = mMotionExtractor.getVelocitySpeedAt(0) * mVelocitySpeedScale;
    sendOsc();
    
}

void dpHakoniwaSand::drawActor(const ramActor &actor){
    ramDrawBasicActor(actor);
}

void dpHakoniwaSand::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    mMotionExtractor.draw();
    
    ofPoint nodePos = mMotionExtractor.getPositionAt(0);
    
    ofPoint boxPos(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));
//    boxPos *= mVelocitySpeed * 0.1;
   
    ofSetColor(255,255);
    ofLine(boxPos,nodePos);
    ofLine(boxPos,ofPoint(0,1000,0));
    ofDrawBox(boxPos,30,30,30);
    
    ramEndCamera();
}