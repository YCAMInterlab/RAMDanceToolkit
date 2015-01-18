//
//  dpHakoVistornado.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/18.
//
//

#ifndef RAMDanceToolkit_dpHakoVistornado_h
#define RAMDanceToolkit_dpHakoVistornado_h

#include "KezSlidePoint.h"

class dptornadoCircle{
public:
    
    dptornadoCircle(){
        mDiv.speed = 0.333;
        mRad.speed = 0.333;
    };
    
    void update(){
        mDiv.update();
        mRad.update();
        mTheta.update();
    }
    
    void setRad(float rad){
        mRad.set(rad);
    }
    
    void setDiv(float div){
        mDiv.set(div);
    }
    
    void setTheta(float theta){
        mTheta.set(theta);
    }
    
    void draw(){
        
        vector<ofVec2f>mPts;
        
      
        for(int i = 0; i < mDiv.val; i++){
            float theta = mTheta.val + ofMap(i,0,mDiv.val,0,TWO_PI);
            float x = cos(theta) * mRad.val;
            float y = sin(theta) * mRad.val;
            
            ofSetColor(dpColor::MAIN_COLOR);
            ofCircle(x,y,ofMap(i,0,mDiv.val,2,mRad.val * 0.0625,true));
            
            mPts.push_back(ofVec2f(x,y));
        }
        
        ofSetColor(255,255,255);
        ofSetLineWidth(2);
        glBegin(GL_LINE_LOOP);
        for(auto &v:mPts){
            glVertex2f(v.x, v.y);
        }
        glEnd();
    
    }
    
private:
   
    float mThetaSpeed;
    
    KezSlide mDiv;
    KezSlide mRad;
    KezSlide mTheta;
};

class dpHakoVisTornado : public ramBaseScene{

public:
    
    string getName() const {return "dpVisTornado";};
    
    void setupControlPanel(){
        ramGetGUI().addSlider("radScale",1.0,100.0,&mRadScale);
        ramGetGUI().addSlider("divScale",1.0,20.0,&mDivScale);
        ramGetGUI().addSlider("thetaScale",1.0,1000.0,&mThetaScale);
    };
    void setup(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Tornado/mean");
        mReceiver.addAddress("/dp/cameraUnit/Tornado/vector");
        
        for(int i = 0; i < CIRCLE_NUM; i++){
            mCircles.push_back(dptornadoCircle());
        }
    }
    
    void receiveOsc(){
        
        while (mReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/Tornado/mean"){
                for(int i = 0; i < CIRCLE_NUM; i++){
                    mCircles[i].setRad((i + 1) * m.getArgAsInt32(3) * mRadScale);
                }
            }
            
            if(m.getAddress() == "/dp/cameraUnit/Tornado/vector"){
                for(int i = 0; i < CIRCLE_NUM; i++){
                    mCircles[i].setDiv(abs(m.getArgAsFloat(i * 2)) * mDivScale);
                    mCircles[i].setTheta(abs(m.getArgAsFloat(i * 2 + 1)) * mThetaScale);
                }
            }
            
        }
    }
    
    void update(){
        
        receiveOsc();
        
        for(auto &v:mCircles){
            v.update();
        }
    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(SINGLE_SCREEN_WIDTH * 0.5, SINGLE_SCREEN_HEIGHT * 0.5);
        for(auto &v:mCircles){
            v.draw();
        }
        ofPopMatrix();
    }
    
private:
    ramOscReceiveTag mReceiver;
    
    static const int CIRCLE_NUM = 10;
    vector<dptornadoCircle>mCircles;
    
    float mRadScale = 10.0;
    float mDivScale = 7.0;
    float mThetaScale = 4.0;
};

#endif
