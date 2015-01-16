//
//  magnetLooper.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/15.
//
//

#include "magnetLooper.h"

magnetLooper::magnetLooper(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;

    sender.setup("192.168.20.55", 12001);
    //sender.setup("192.168.20.25", 12001);
    //sender.setup("loacalhost", 8528);
    //sender.setup("192.168.20.99", 8528);
}


//====================================
void magnetLooper::setup(){
    manualControl = false;
    
    speedControl = false;
    speedThreshold = 1.0;
    hantei = 0;
}
//====================================

void magnetLooper::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    //====================================
    //マニュアルコントロール
    gui->addSpacer();
    gui->addToggle("Manual Control"		, &manualControl);
    gui->addToggle("stop"		, &looperStop);
    //====================================
    
    //====================================
    //スピードコントロール
    gui->addSpacer();
    gui->addToggle("Speed Control"		, &speedControl);
    gui->addSlider("Speed Threshold", 0.0, 5.0, &speedThreshold);
    //====================================
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void magnetLooper::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    //oscを送る数を制限
    if(ofGetFrameNum() % 10 == 0){
        

    //====================================
    //マニュアルコントロール
    if(manualControl == true){
        if(looperStop == true){
            hantei = 1;
        }else{
            hantei = 0;
        }
    }
    //====================================
    
    //====================================
    //スピードコントロール
    if(speedControl == true){
        //ダンサー0の特定ノードの早さがspeedThreshold以上だったら
        if(motionExtractor.getVelocitySpeedAt(0) > speedThreshold){
            //magnetLooperに1（stop）を送る
            hantei = 1;
        }else{
            //magnetLooperに0（keep run）を送る
            hantei = 0;
        }
    }
    //====================================
    
//    /*=== OSC Send Example ===*/
//    ofxOscMessage m;
//    m.setAddress("/dp/hakoniwa/magnetLooper");
//    m.addIntArg(hantei);
////    m.addFloatArg(motionExtractor.getVelocitySpeedAt(0)); //send node's speed
//    sender.sendMessage(m);
        
        ofxOscMessage m;
        m.setAddress( "/dp/hakoniwa/magnetLooper" );
        m.addIntArg(hantei);
        m.addIntArg(0);
        sender.sendMessage(m);
    
    }
}

void magnetLooper::draw(){
    
    ramBeginCamera();
    
    /*=== Preview selected nodes ===*/
    
    if (mDrawPreview)	motionExtractor.draw();
    
    
    /*=== Example drawing with motionExtractor ===*/
    
    if (mDrawLines)		example_drawLines();
    if (mDrawTriangle)	example_drawTriangles();
    
    
    ramEndCamera();
    
    if (mDrawDump)		example_drawDump();
    
}

void magnetLooper::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
    
}

void magnetLooper::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void magnetLooper::example_drawDump(){
    
    ofPushMatrix();
    ofTranslate(700, 10);
    
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        ofPushMatrix();
        ofTranslate(0, i*90);
        
        ofVec3f vec = motionExtractor.getVelocityAt(i);
        float speed = motionExtractor.getVelocitySpeedAt(i);
        
        ofNoFill();
        ofRect(0, 0, 200, 80);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + motionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + motionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(motionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, motionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}