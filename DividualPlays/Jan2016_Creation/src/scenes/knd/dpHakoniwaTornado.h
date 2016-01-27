//
//  dpHakoniwaTornade.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/14.
//
//

#ifndef RAMDanceToolkit_dpHakoniwaTornade_h
#define RAMDanceToolkit_dpHakoniwaTornade_h

#include "dpTailSphere.h"

class dpHakoniwaTornado : public ramBaseScene{
public:
    
    string getName() const {return "dpHTornado";};
    
    void setupControlPanel(){
        
        ramGetGUI().addIntSlider("rad",0,255,&mRad);
        ramGetGUI().addSlider("radMin",0.0,300.0,&mRadMin);
        ramGetGUI().addSlider("radMax",0.0,300.0,&mRadMax);

        ramGetGUI().addSlider("drawRadMin",0.0,12.0,&mDrawRadMin);
        ramGetGUI().addSlider("drawRadMax",0.0,12.0,&mDrawRadMax);
        ramGetGUI().addSlider("drawRadThresh",0.0,255.0,&mDrawRadThresh);
        ramGetGUI().addSlider("mistThresh",10.0, 255, &mFunThresh);
        ramGetGUI().addSlider("fan",0,255,&mFan);
        ramGetGUI().addToggle("simpleCircle",&mIsDrawSimpleCircle);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaTornado::onPanelChanged);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(330,30));
        mMotionExtractor.load("motionExt_dpHakoniwaTornado.xml");
        
    }
    void setup(){

        mFanSender.setup("192.168.20.71",8528);
        
        mSender[0].setup("192.168.20.2", 10000);
        mSender[1].setup("192.168.20.3", 10000);
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
    }
    
    void fan(int val){
        
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/fan");
        m.addIntArg(val);
        mFanSender.sendMessage(m);
        
    }

    void update(){
        
        mMotionExtractor.update();
        fan(mFan);
        mSphere.setRad(mDrawRadMin, mDrawRadMax);

    }
    
    void rotateToNormal(ofVec3f normal) {
        normal.normalize();
        
        float rotationAmount;
        ofVec3f rotationAngle;
        ofQuaternion rotation;
        
        ofVec3f axis(0, 0, 1);
        rotation.makeRotate(axis, normal);
        rotation.getRotate(rotationAmount, rotationAngle);
        ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    }
    
    void draw(){
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        
        ofPoint a = mMotionExtractor.getPositionAt(0,false);
        ofPoint b = mMotionExtractor.getPositionAt(1,false);
        ofPoint c = mMotionExtractor.getPositionAt(2,false);
        
        sendOscToVis(a, b, c);
        
        ofPoint center;
        ofPoint normal;
        float radius;
        
        findCircle(a,b,c,center,normal,radius);
        
        mRad = ofMap(radius,mRadMin,mRadMax,0,255,true);
        
        if(mRad > mFunThresh){
            mFan = 255;
        }else{
            mFan = 0;
        }
        
        if(mRad < mDrawRadThresh)radius = 0.0;
        
        
        ramBeginCamera();
        ofPushMatrix();
        ofTranslate(center);
        rotateToNormal(normal);
        ofNoFill();
        if(mIsDrawSimpleCircle)ofCircle(0,0,0,radius);
        ofPopMatrix();
        
        ofSetColor(255,255,255);
        mSphere.draw(center,radius,normal);

        ramEndCamera();
    
    }
    
    void sendOscToVis(ofPoint a,ofPoint b, ofPoint c){
        ofxOscMessage m;
        m.setAddress("/dp/toVis/Tornado");
        
        m.addFloatArg(a.x);
        m.addFloatArg(a.y);
        m.addFloatArg(a.z);
        
        m.addFloatArg(b.x);
        m.addFloatArg(b.y);
        m.addFloatArg(b.z);
        
        m.addFloatArg(c.x);
        m.addFloatArg(c.y);
        m.addFloatArg(c.z);
        
        mSender[0].sendMessage(m);
        mSender[1].sendMessage(m);
    }
    
    void onDisabled(){
 
        mRad = 0;
        
        fan(0);

    }
    
private:
    
    ofxOscSender mFanSender;
    ofxOscSender mSender[2];
    
    int mRad = 0;
    
    float mRadMin = 7.6;
    float mRadMax = 95;
    float mFunThresh = 202.0;
    float mFan = 220;
    
    float mDrawRadThresh = 10.0;
    
    float mDrawRadMin = 1.0;
    float mDrawRadMax = 12.0;
    
    bool mIsDrawSimpleCircle = false;
    
    dpTailSphereController mSphere;
    ramMotionExtractor mMotionExtractor;
};

#endif
