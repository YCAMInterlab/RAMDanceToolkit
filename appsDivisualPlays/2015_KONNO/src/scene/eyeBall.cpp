//
//  eyeBall.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/11.
//
//

#include "eyeBall.h"
eyeBall::eyeBall(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;
    
    sender.setup("192.168.20.71", 8528);
}

void eyeBall::setup(){
    //-------------
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    point.set(20);
    
    roll = 0.0;
    pitch = 0.0;
    yaw = 0.0;
    phase = 0.0;
    
    radiusX = 0.0;
    radiusY = 0.0;
    //-------------
}

void eyeBall::setupControlPanel(){
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    gui->addSpacer();
    gui->addSlider("speed", 0.0, 1, &speed);
    gui->addSlider("radiusX", 0.0, 800.0, &radiusX);
    gui->addSlider("radiusY", 0.0, 800.0, &radiusY);
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void eyeBall::update(){
    
    //-------------
    //  x軸を回転軸とする場合
    //  pitch += 1;
    
    //  y軸を回転軸とする場合
    //  yaw += 1.0;
    
    //  z軸を回転軸とする場合
    //    roll += 1.0;
    
    phase += speed;
    //-------------
    
    /*=== update ===*/
    motionExtractor.update();
    
    /*=== OSC Send Example ===*/
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/testHakoniwa");
    m.addFloatArg(motionExtractor.getVelocitySpeedAt(0)); //send node's speed
    sender.sendMessage(m);
    
}

void eyeBall::draw(){
    
    //----------------
//    ofVec3f Znormal(0,0,1);
//    ofVec3f Xnormal(1,0,0);
//    ofVec3f Ynormal(0,1,0);
//    
//    ofQuaternion qr (roll, Znormal);
//    ofQuaternion qp (pitch, Xnormal);
//    ofQuaternion qy (yaw, Ynormal);
//    ofQuaternion qt;    //  total quaternion
//    
//    qt = qr * qp * qy;
    
    ofSetColor(180);
    ofNoFill();
    
    float radius = 200.0;
    
    ofPushMatrix();
    {
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofNoFill();
        
//        ofVec3f qAxsis;
//        float   angle;
//        qt.getRotate(angle, qAxsis);
//        ofRotate(angle, qAxsis.x, qAxsis.y, qAxsis.z);
        
        ofRotateX(sin(phase) * radius);
        ofRotateY(cos(phase) * radius);
        
        point.setPosition(radiusY, radiusX, 0);
        //point.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
        point.drawAxes(40);
        point.draw();
        ofPopMatrix();
    }
    ofPopMatrix();
    //----------------
    
    ramBeginCamera();
    
    /*=== Preview selected nodes ===*/
    
    if (mDrawPreview)	motionExtractor.draw();
    
    
    /*=== Example drawing with motionExtractor ===*/
    
    if (mDrawLines)		example_drawLines();
    if (mDrawTriangle)	example_drawTriangles();
    
    ramEndCamera();
    
    if (mDrawDump)		example_drawDump();
}

void eyeBall::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
    
}

void eyeBall::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void eyeBall::example_drawDump(){
    
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