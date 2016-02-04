//
//  dpHakoniwaRemainView.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#ifndef __RAMDanceToolkit__dpHakoniwaRemainView__
#define __RAMDanceToolkit__dpHakoniwaRemainView__

#include "KezSlidePoint.h"
#include "KezDelay.h"
#include "ramMain.h"
#include "dpConstants.h"

class dpRemainedHakoniwaView{
public:
    
    void setup();
    void update();
    void receieveOsc();
    void showEndNum();
    void draw();
    
private:
    
    int mNumRemain = 0;
    
    int mEndNum = 0;
    bool mHasEnd = false;
    
    class EndBar{
    public:
        
        void setup(){
            mHeightTarget = SINGLE_SCREEN_HEIGHT * 0.75;
            mWidth = 20;
            mMargin = 200;
        }
        
        void update(){
            mHeight.update();
            mAlpha.update();
            
            if(mFadeStartDelay.update()){
                mAlpha.set(0);
            }
        }
        
        void show(){
            mHeight.imSet(0);
            mHeight.set(mHeightTarget);
            mAlpha.imSet(255);
            mFadeStartDelay.start(120);
        }
        
        void draw(int num){
            
            if(mAlpha.val > 1.0){
            
                drawBar(num);
                
            }
        
        }
        
        void drawBar(int num){
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            
            for(int i = 0; i < num; i++){
                ofColor color = dpColor::MAIN_COLOR;
                ofSetColor(color.r,color.g,color.b,mAlpha.val);
                
                float x = SINGLE_SCREEN_WIDTH * 0.5;
                
                ofRect(x - num * 0.5 * mMargin + mMargin * i + mMargin * 0.75,
                       SINGLE_SCREEN_HEIGHT * 0.5,
                       mWidth,
                       mHeight.val);
            }
            
            ofPopStyle();
        }
        
    private:
        
        KezSlide mHeight;
        KezSlide mAlpha;
        KezDelay mFadeStartDelay;
        float mHeightTarget;
        
        float mWidth;
        float mMargin;
    };
    
    ramOscReceiveTag mReceiver;
    
    EndBar mEndBar;
        
};

#endif /* defined(__RAMDanceToolkit__dpHakoniwaRemainView__) */
