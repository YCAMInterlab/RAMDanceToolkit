//
//  frozenIce.cpp
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/09.
//
//

#include "frozenIce.h"

frozenIce::frozenIce(){
    
    mDrawLines		= false;
    mDrawTriangle	= false;
    mDrawPreview	= true;
    mDrawDump		= true;
    
    sender.setup("192.168.20.55", 8528);
    
}

void frozenIce::setup(){
    hantei  = 0;
}

void frozenIce::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    
    gui->addToggle("Preview"	, &mDrawPreview);
    gui->addToggle("Dump"		, &mDrawDump);
    
    //--------
    gui->addToggle("Start"		, &iceStart);
    //--------
    
    gui->addSpacer();
    gui->addToggle("Lines"		, &mDrawLines);
    gui->addToggle("Triangle"	, &mDrawTriangle);
    
    /*=== register ===*/
    motionExtractor.setupControlPanel(this);
    
}

void frozenIce::update(){
    
    /*=== update ===*/
    motionExtractor.update();
    
    
    if(motionExtractor.getVelocitySpeedAt(0) > 2){
        hantei = 0;
    }else{
        hantei = 1;
    }
    
    
    /*=== OSC Send Example ===*/
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/frozenIce");
    //m.addFloatArg(hantei);
    m.addIntArg(hantei);
    sender.sendMessage(m);
}

void frozenIce::draw(){
    
    ramBeginCamera();
    
    /*=== Preview selected nodes ===*/
    
    if (mDrawPreview)	motionExtractor.draw();
    
    
    /*=== Example drawing with motionExtractor ===*/
    
    if (mDrawLines)		example_drawLines();
    if (mDrawTriangle)	example_drawTriangles();
    
    
    ramEndCamera();
    
    if (mDrawDump)		example_drawDump();
    
    //----------
    if(iceStart == true){
        hantei  = 0;
        count++;
        
        if(count > 100){
            hantei = 1;
        }
    }
    
    cout << hantei << endl;
    //----------
    
}

void frozenIce::example_drawLines(){
    
    ofSetColor(255);
    for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){
        
        ofVec3f vec_a = motionExtractor.getPositionAt(i);
        ofVec3f vec_b = motionExtractor.getPositionAt(i+1);
        
        ofLine(vec_a, vec_b);
    }
    
}

void frozenIce::example_drawTriangles(){
    
    ofVec3f vec_a = motionExtractor.getPositionAt(0);
    ofVec3f vec_b = motionExtractor.getPositionAt(1);
    ofVec3f vec_c = motionExtractor.getPositionAt(2);
    
    ofSetLineWidth(3.0);
    ofNoFill();
    ofTriangle(vec_a, vec_b, vec_c);
    ofFill();
    ofSetLineWidth(1.0);
    
}

void frozenIce::example_drawDump(){
    
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
