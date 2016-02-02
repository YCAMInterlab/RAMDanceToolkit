#include "dpHakoniwaSand.h"

void dpHakoniwaSand::setupControlPanel(){
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
   // mMotionExtractor.load("motionExt_dpHSandStorm.xml");
    ramGetGUI().addIntSlider("Val1_Test", 750, 2170, &val[0]);
    ramGetGUI().addIntSlider("Val2_Test", 750, 2170, &val[1]);
    ramGetGUI().addIntSlider("Val3_Test", 750, 2170, &val[2]);
    ramGetGUI().addIntSlider("Val1_Min", 0, 200, &minVal[0]);
    ramGetGUI().addIntSlider("Val1_Max", 0, 200, &maxVal[0]);
    ramGetGUI().addIntSlider("Val2_Min", 0, 200, &minVal[1]);
    ramGetGUI().addIntSlider("Val2_Max", 0, 200, &maxVal[1]);
    ramGetGUI().addIntSlider("Val3_Min", 0, 200, &minVal[2]);
    ramGetGUI().addIntSlider("Val3_Max", 0, 200, &maxVal[2]);
    
    ramGetGUI().addToggle("Test mode", &bTestMode);
    bTestMode = false;
    
    val[0] = 750;
    val[1] = 750;
    val[2] = 750;
    
    for (int i = 0; i < 3; i++) {
        minVal[i] = 30;
        maxVal[i] = 180;
    }
}

void dpHakoniwaSand::setup(){
 
    mSender.setup(SAND_STORM_IP,8528);
    
}

void dpHakoniwaSand::sendOsc(){
    
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/sand");
    
    m.addIntArg(val[0]); // pulse width(usec) for motor1
    m.addIntArg(val[1]); // pulse width(usec) for motor2
    m.addIntArg(val[2]); // pulse width(usec) for motor3
    
    mSender.sendMessage(m);
}

void dpHakoniwaSand::update(){

    mMotionExtractor.update();
    
    if (!bTestMode) {
        val[0] = ofClamp((int)ofMap(mMotionExtractor.getPositionAt(0).y, minVal[0], maxVal[0], 750, 2170), 750, 2170);
        val[1] = ofClamp((int)ofMap(mMotionExtractor.getPositionAt(1).y, minVal[1], maxVal[1], 750, 2170), 750, 2170);
        val[2] = ofClamp((int)ofMap(mMotionExtractor.getPositionAt(2).y, minVal[2], maxVal[2], 750, 2170), 750, 2170);
    }

    sendOsc();

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
    
    val[0] = 750;
    val[1] = 750;
    val[2] = 750;
    
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
