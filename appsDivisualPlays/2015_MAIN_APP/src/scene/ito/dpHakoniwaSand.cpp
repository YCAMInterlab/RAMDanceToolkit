#include "dpHakoniwaSand.h"

void dpHakoniwaSand::setupControlPanel(){
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    mMotionExtractor.load("motionExt_dpHSandStorm.xml");
    ramGetGUI().addIntSlider("Val1_Test", 800, 2200, &val1);
    ramGetGUI().addIntSlider("Val2_Test", 800, 2200, &val2);
    ramGetGUI().addIntSlider("Val3_Test", 800, 2200, &val3);
    ramGetGUI().addToggle("Test mode", &bTestMode);
    
//    vector<string> contents;
//    contents.push_back("Hight");
//    contents.push_back("Rot");
//    
    //ramGetGUI().addRadioGroup("mode__", contents, &mode);
    
    bTestMode = false;
    
}

void dpHakoniwaSand::setup(){
 
    mSender.setup("192.168.20.54",8528);
    
}

void dpHakoniwaSand::sendOsc(){
    
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/sand");
    
    m.addIntArg(val1); // pulse width(usec) for motor1
    m.addIntArg(val2); // pulse width(usec) for motor2
    m.addIntArg(val3); // pulse width(usec) for motor3
    
    mSender.sendMessage(m);
}

void dpHakoniwaSand::update(){

    mMotionExtractor.update();
    
    if (!bTestMode) {
//        if (mode == 0) {
            val1 = (int)ofMap(mMotionExtractor.getPositionAt(0).y, 0, 200, 800, 2200);
            val2 = (int)ofMap(mMotionExtractor.getPositionAt(1).y, 0, 200, 800, 2200);
            val3 = (int)ofMap(mMotionExtractor.getPositionAt(2).y, 0, 200, 1000, 2000);
//        } else if (mode ==1) {
//            
//                //not yet
//        
//        }
    }

    sendOsc();
    
//    cout << mode << endl;
    
}

void dpHakoniwaSand::drawActor(const ramActor &actor){
    ramDrawBasicActor(actor);
}

void dpHakoniwaSand::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    mMotionExtractor.draw();
    ramEndCamera();
    
    example_drawDump();
}

void dpHakoniwaSand::onEnabled(){
    
    
}

void dpHakoniwaSand::onDisabled(){
    
    val1 = 2200;
    val2 = 2200;
    val3 = 2000;
    
    sendOsc();
    
}


void dpHakoniwaSand::example_drawDump(){
    
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
        ofRect(10, 45, mMotionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}
