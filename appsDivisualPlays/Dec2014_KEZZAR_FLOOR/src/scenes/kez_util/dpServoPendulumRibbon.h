//
//  Ribbon.h
//  HakoVisTri
//
//  Created by kezzardrix2 on 2015/01/06.
//
//

#ifndef dpPendulum_Ribbon_h
#define dpPendulum_Ribbon_h

#include "dpRibbon.h"

class dpServoPendulumRibbon{
public:

    bool hasFinishedAddPt = false;
    float mTailBackStartTime = 0.5;
    float mElapsedTime = 0.0;
    
    dpRibbon<120> mRibbon;
    
    dpServoPendulumRibbon(){
     
    }
    
    void setup(){
        mRibbon.setup();
    }
    
    void setHead(ofPoint _pos){
        
        if(hasFinishedAddPt){
            mRibbon.setAll(_pos);
        }
        
        mRibbon.setHead(_pos);
        hasFinishedAddPt = false;
        
    }
    
    void update(){
        
        if(hasFinishedAddPt == false){
           
            mRibbon.setMode(dpRibbon<120>::NORMAL);
            mElapsedTime = 0.0;
            
        }else{
            
            if(mElapsedTime > mTailBackStartTime){
                mRibbon.setMode(dpRibbon<120>::BACK);
            }else{
                mRibbon.setMode(dpRibbon<120>::STOP);
            }
            
            mElapsedTime += ofGetLastFrameTime();
            
        }
        
         mRibbon.update();
    }

    void draw(){
        mRibbon.draw();
    }
    
    void finishedAddPt(){
        hasFinishedAddPt = true;
    }
    
};

#endif

