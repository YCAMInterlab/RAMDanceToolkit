//
//  dpTailSphere.h
//  2015_MAIN_APP
//
//  Created by kezzardrix2 on 2015/01/22.
//
//

#ifndef _015_MAIN_APP_dpdpTailSphere_h
#define _015_MAIN_APP_dpdpTailSphere_h

class dpTailSphere{
public:
    static const int NUM = 12;
    
    KezSlidePoint mPos;
    ofPoint mTarget;
    int mTargetCounter = 0;
    vector<ofPoint>mTail;
    
    float mTheta = ofRandom(0.0,TWO_PI);
    
    float mRad = ofRandom(0.3,1.0);
    
    float mRotateSpeed = 0.1;
    
    dpTailSphere(float theta,float rad){
        for(int i = 0; i < NUM; i++){
            mTail.push_back(ofPoint());
        }
        
        mTheta = theta;
        mRad = rad;
    }
    
    void setRotateSpeed(float speed){
        mRotateSpeed = speed;
    }
    
    void setRad(float rad){
        mRad = rad;
    }
    
    void set(ofPoint pos){
        
        mPos.set(pos);
        mTarget = pos;
    }
    
    int getTarget(){
        return mTargetCounter;
    }
    
    void update(ofPoint pos){
        
        mPos.set(pos);
        mTail[0] = mPos;
        
        for(int i = 1; i < NUM; i++){
            ofPoint tmp = mTail[i - 1];
            mTail[i-1] = mTail[i];
            mTail[i] = tmp;
            
        }
        
        /*if((mTarget - mTarget).length() < 1.0){
         mTargetCounter++;
         mTargetCounter %= 2;
         }*/
        
        mTheta += mRotateSpeed;
        
        mPos.update();
    }
    void draw(){
        ofSetColor(dpColor::MAIN_COLOR);
        ofDrawSphere(mTail[0],2);
        ofSetColor(255,255,255);
       /* for(int i = 1; i < NUM; i++){
            ofLine(mTail[i-1],mTail[i]);
        }*/
        
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        for(int i = 0; i < NUM; i++){
            ofCurveVertex(mTail[i].x, mTail[i].y);
        }
        ofEndShape();
        ofPopStyle();
    }
};

class dpTailSphereController{
private:
  
    static const int NUM = 12;
    vector<dpTailSphere>mTail;
    bool isEnable = true;
    
public:
    
    bool isInner = false;
    
    
    dpTailSphereController(){
        for(int i = 0; i < NUM; i++){
            mTail.push_back(dpTailSphere(ofMap(i,0,NUM,0,TWO_PI),
                                       ofMap(i,0,NUM,0.1,1.0)));
        }
     
        setRotateSpeed(0.2);
    }
    
    void setEnable(bool enable){
        isEnable = enable;
    }
    
    void setRotateSpeed(float speed){
        for(auto &v:mTail){
            v.setRotateSpeed(speed);
        }
    }
    
    void setRad(float min,float max){
        for(int i = 0; i < NUM; i++){
            mTail[i].setRad(ofMap(i,0,NUM,min,max));
        }
    }
    
    void draw(ofPoint center,float radius,ofPoint normal){
        
        if(isEnable){
            
            for(auto &v:mTail){
                
                float x = cos(v.mTheta) * radius * v.mRad;
                float y = sin(v.mTheta) * radius * v.mRad;
                
                v.update(ofPoint(x,y));
                
                ofPushMatrix();
                ofTranslate(center);
                rotateToNormal(normal);
                v.draw();
                ofPopMatrix();
            }
            
        }
    }
};

#endif
