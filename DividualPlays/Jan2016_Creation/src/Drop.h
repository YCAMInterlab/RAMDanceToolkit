//
//  Drop.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/08/04.
//
//

#ifndef afterWorkshop_Drop_h
#define afterWorkshop_Drop_h

class Ripple{
private:
    slide rad;
    slide alpha;
    ofPoint mCenter;
public:
    
    Ripple(){
        rad.speed = 0.05;
        alpha.speed = 0.05;
    }
    
    void burst(ofPoint center){
        alpha.imSet(255);
        alpha.set(0);
        rad.imSet(0);
    
        rad.speed = 0.05;
        rad.set(100);
        
        mCenter = center;
    }
    
    void update(){
        rad.update();
        alpha.update();
    }
    void draw(){
        if(alpha.val >= 1.0){
            ofPushStyle();
            ofSetLineWidth(3);
            ofNoFill();
            ofSetColor(255,255,255,alpha.val);
            ofPushMatrix();
            ofTranslate(mCenter);
            ofRotateX(90);
            ofCircle(0,0,rad.val);
            ofPopMatrix();
            ofPopStyle();
        }
    }
};

class Drop{
private:
    ofPoint mVel;
    float a = 1.05;
    int mMode = 0;
    
    enum Mode{
        RAIN_RISE,
        RAIN_BROWN,
        RAIN_FALL,
        RAIN_RIPPLE
    };
    
    int mFallCounter = 0;
    static const int FALL_LIMIT = 120;
    
public:
    Ripple mRipple;
    ofPoint mCenter;
    
    void update(){
        mRipple.update();
        
        if(mMode == RAIN_FALL)fall();
        else if(mMode == RAIN_BROWN)brown();
        //else if(mMode == RAIN_RISE)rise();
        
    }
    
    void reset(){
        mFallCounter = 0;
    }
    
    void fired(ofPoint pos,ofPoint vel){
        mCenter = pos;
        mMode = RAIN_FALL;
        mVel = vel;
        //mVel.y = abs(mVel.y) * 4.0;
        reset();
    }
    
    /*void rise(){
        mCenter += mVel;
        if(mCenter.y >= CEIL_POS)mMode = RAIN_BROWN;
    }*/
    
    void brown(){
        mCenter.x += ofRandom(-1.0,1.0);
        mCenter.z += ofRandom(-1.0,1.0);
        
        mFallCounter++;
        if(mFallCounter >= FALL_LIMIT){
            mMode = RAIN_FALL;
            mVel.y = -2.0;
        }
    }
    
    void fall(){
        mVel *= a;
        mCenter.y += mVel.y;
        
        if(mCenter.y <= 0.0){
            mRipple.burst(mCenter);
            mMode = RAIN_RIPPLE;
        }
        
    }
    
    void draw(){
        
        ofPushStyle();
        
        if(ofDist(0,0,mCenter.x,mCenter.z) < 300.0){
            
            if(mMode == RAIN_RIPPLE)mRipple.draw();
            else {
                
                glEnable(GL_POINT_SMOOTH);
                glPointSize(3);
                glColor4f(1.0,1.0,1.0,1.0);
                glBegin(GL_POINTS);
                glVertex3f(mCenter.x,mCenter.y,mCenter.z);
                glEnd();
            }
            
        }
        ofPopStyle();
    }
};

#endif
