//
//  dpServoPendulumThread.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_dpServoPendulumThread_h
#define RAMDanceToolkit_dpServoPendulumThread_h

#include "ofMain.h"
#include "ofThread.h"


class dpServoPendulumThread : public ofThread
{
    
private:
    bool isSetupOsc = false;
    ofxOscSender mSender;
    float mAngle;
    float mOscInterval = 0.0333;
    float mShake = 0.0;
    float mRange = 40.0;
    
    float mEllapsedTime = 0.0;
    float mSpeed = 2.0;
    float mPTime = 0.0;
    float mLastFrameTime = 0.0;
    
    int mDir = 1;
public:

    dpServoPendulumThread(){}
    
    void setup(){
        mSender.setup("192.168.20.67",8528);
        isSetupOsc = true;
    }
    
    void sendOsc(float val){
        if(isSetupOsc){
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/servoPendulum");
            m.addIntArg(mAngle + 90 + val);
            mSender.sendMessage(m);
        }
    }
    
    float setSpeed(float speed){
        lock();
        mSpeed = speed;
        unlock();
    }
    
    float setShake(float shake){
        lock();
        mShake = shake;
        unlock();
    }
    
    void setRange(float range){
        lock();
        mRange = range;
        unlock();
    }
    
    float getAngle(){
        ofScopedLock lock(mutex);
        return mAngle + 90;
    }
    
    void start()
    {
        startThread();
        mLastFrameTime = ofGetElapsedTimef();
    }
    
    void stop()
    {
        stopThread();
    }
    
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            if(lock())
            {
                
                float elapsed = ofGetElapsedTimef();
                
                mEllapsedTime += ((elapsed - mLastFrameTime) / mSpeed) * mDir;
                
                mAngle = mEllapsedTime * mRange * 4.0;
                
                if(mAngle < -mRange){
                    mDir *= -1;
                    while(mAngle < -mRange){
                        mAngle += 1.0;
                        mEllapsedTime = 0.0;
                    }
                }
                else if(mAngle > mRange){
                    mDir *= -1;
                    while(mAngle > mRange){
                        mAngle += -1.0;
                        mEllapsedTime = 0.0;
                    }
                }
                
                if((elapsed - mPTime) > mOscInterval){
                
                    mPTime = elapsed;
                    sendOsc(mShake);
                    
                }
                
                mLastFrameTime = elapsed;
                
                unlock();
                
                usleep(1);
            }
        }
    }
    
protected:
  
    
};

#endif
