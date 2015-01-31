//
//  HakoniwaServoPendulum.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_HakoniwaServoPendulum_h
#define RAMDanceToolkit_HakoniwaServoPendulum_h

#include "ramMotionExtractor.h"

class dpHakoniwaServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpHServoPendulum"; }
    
	void setupControlPanel(){
        
        ramGetGUI().addSlider("speed", 0.1, 4.0, &mSpeed);
        ramGetGUI().addSlider("lengthMin", 0.0, 100.0, &mLengthMin);
        ramGetGUI().addSlider("lengthMax", 5.0, 200.0, &mLengthMax);
        
        ramGetGUI().addSlider("range", 0.0, 90, &mRange);
        ramGetGUI().addSlider("angle", -RANGE_MAX, RANGE_MAX, &mAngle);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(350,32));
        mMotionExtractor.load("motionExt_HakoniwaServoPendulum.xml");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaServoPendulum::onPanelChanged);
        
    }
    
    void setup(){
    
        mSender.setup("192.168.20.67",8528);
        
    }
    
    void updateRange(){
        
        ofPoint pos1 = mMotionExtractor.getPositionAt(0);
        ofPoint pos2 = mMotionExtractor.getPositionAt(1);

        mRange = ofMap((pos1 - pos2).length(),mLengthMin,mLengthMax,0,RANGE_MAX,true);
        mRange = fmaxf(0, mRange);
        mRange = fminf(RANGE_MAX,mRange);
        
    }
    
    void updateAngle(){
        
        mAngle += (ofGetLastFrameTime() / mSpeed) * mDir * mRange * 4;
        
        if(mAngle > mRange * 0.5){
         
            mDir = -1;
            mAngle = mRange * 0.5;
         
        }
        
        if(mAngle < -mRange * 0.5){
            
            mDir = 1;
            mAngle = -mRange * 0.5;
           
        }
        
    }
    
    void sendOsc(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/servoPendulum");
        m.addIntArg(90 + mAngle);
        mSender.sendMessage(m);
    }
    
    void update(){
    
        updateRange();
        updateAngle();
        sendOsc();
        mMotionExtractor.update();

    }
    
    void draw(){
        
        ramBeginCamera();
        
        ofPoint pos1 = mMotionExtractor.getPositionAt(0);
        ofPoint pos2 = mMotionExtractor.getPositionAt(1);
        ofLine(pos1,pos2);
        
        ofPushMatrix();
        ofRotateZ(mAngle);
        ofLine(0,0,0,mDebugLineLength);
        ofCircle(0,mDebugLineLength,mDebugCircleRad);
        ofPopMatrix();

        ramEndCamera();
        
    }
    
    void drawActor(const ramActor &actor){
        
    }
    
    void onEnabled(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/servoPendulum");
        m.addIntArg(0);
        mSender.sendMessage(m);
    }
    
    void onDisabled(){
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
      
    }
    
private:
    
    static const int RANGE_MAX = 90;
    
    float mLengthMin = 20.0;
    float mLengthMax = 120.0;
    float mThreshMin = 6.0;
    float mSpeed = 1.0;
    float mRange = 0.0;
    float mAngle = 0.0;
    
    float mDebugLineLength = 150;
    float mDebugCircleRad = 30;
    
    ramMotionExtractor mMotionExtractor;
    
    ofxOscSender mSender;
    
    int mDir = 1;
    float mElapsed = 0.0;

};

#endif
