//
//  dpHakoVisTornade.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/18.
//
//

#ifndef RAMDanceToolkit_dpHakoVisTornade_h
#define RAMDanceToolkit_dpHakoVisTornade_h

#include "KezSlidePoint.h"
#include "dpTailSphere.h"

class dpHakoVisTornado : public ramBaseScene{
public:
    
    string getName() const {return "dpVisTornado";};
    
    void setupControlPanel(){
        ramGetGUI().addSlider("drawRadMin",0.0,10.0,&mDrawRadMin);
        ramGetGUI().addSlider("drawRadMax",0.0,10.0,&mDrawRadMax);
        ramGetGUI().addToggle("simpleCircle",&isDrawSimpleCircle);
        ramGetGUI().addSlider("speed",0.01,2.0,&mSpeed);

        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/Tornado");
    }
    void setup(){
      
        for(auto &v:mPts){
            v.set(0,0,0);
        }
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
    }
    
    void receiveOsc(){
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/toVis/Tornado"){
                for(int i = 0; i < 3; i++){
                    mPts[i].set(m.getArgAsFloat(i * 3),
                                m.getArgAsFloat(i * 3 + 1),
                                m.getArgAsFloat(i * 3 + 2));
                }
            }
        }
    }
    
    void update(){

        receiveOsc();
        mSphere.setRad(mDrawRadMin, mDrawRadMax);
        mSphere.setRotateSpeed(mSpeed);
   
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
        
        ofPoint a = mPts[0];
        ofPoint b = mPts[1];
        ofPoint c = mPts[2];
        
        ofPoint center;
        ofPoint normal;
        float radius;
        
        findCircle(a,b,c,center,normal,radius);
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        
        ramBeginCamera();
        
        if(isDrawSimpleCircle){
            
            ofPushMatrix();
            ofTranslate(center);
            rotateToNormal(normal);
            ofNoFill();
            ofSetColor(255,255,255);
            ofCircle(0, 0, radius);
            ofPopMatrix();
        
        }else{
        
            mSphere.draw(center,radius,normal);
        
        }
        
        ramEndCamera();
        
    }

    void onDisabled(){
        
    }
    
    void onEnabled(){
        ramCameraManager::instance().getActiveCamera().setPosition(dpGetRDTKSceneCameraPosition());
        ramCameraManager::instance().getActiveCamera().lookAt(dpGetRDTKSceneCameraLookAt());
    }
    
private:
   
    int mRad = 0;
    
    dpTailSphereController mSphere;
    
    float mDrawRadMin = 0.1;
    float mDrawRadMax = 6.0;
    float mSpeed = 0.2;
    
    ramOscReceiveTag mReceiver;
    
    ofPoint mPts[3];
    
    bool isDrawSimpleCircle = false;

};

#endif
