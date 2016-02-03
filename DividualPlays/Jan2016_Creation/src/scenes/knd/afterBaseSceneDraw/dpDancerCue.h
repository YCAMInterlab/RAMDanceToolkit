//
//  dpDancerCue.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#ifndef RAMDanceToolkit_dpDancerCue_h
#define RAMDanceToolkit_dpDancerCue_h

#include "ramMain.h"
#include "dpConstants.h"
#include "KezSlidePoint.h"

class dpDancerCue{
public:
    
    void setup();
    void update();
    void receiveOsc();
    void draw();

private:
    
    ramOscReceiveTag mReceiver;
    
    const float mHeight = 300;
    static const int DANCER_NUM = 4;

    map<string,int>mDancerNum;
    
    class CueRect{
        public:
            
            CueRect(){};
            void setup(ofPoint pos, ofPoint size, ofColor color);
            void update(float time);
            void draw();
            void setEnable(bool enable, float time);
        
        private:
            
            ofPoint mPos;
            ofPoint mSize;
            ofColor mColor;
            
            float mAlphaBlinkSpeed = 8.0;
            float mAlpha = 0.0;
            
            bool mIsEnable = false;
        
            float mElapsed = 0.0;
        
            KezSlide mAlphaScale;

    };
    
    vector<CueRect>mRects;    
    
    enum ARRANGEMENT_MODE{
        TOP,
        CENTER,
        BOTTOM,
        CORNER
    };
    
    ARRANGEMENT_MODE mMode;
    
};
#endif
