//
//  dpHakoVisRecordRibbon.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_dpHakoVisRecordRibbon_h
#define RAMDanceToolkit_dpHakoVisRecordRibbon_h

#include "dpRibbon.h"

class dpRibbonControler{
public:

    dpRibbon<2000> mRibbon;
    float mTargetPosZ = 0;
    KezSlidePoint mTarget;
    
    ofColor mColor;
    bool isFocus = false;
    
    void setup(){
        mRibbon.setup();
        mRibbon.setWidth(0.1, 0.5);
        mTarget.speed = 0.1;
    }
    
    void setColor(ofColor color){
        mColor = color;
    }
    
    void setTarget(ofPoint pt){
        mTarget.set(pt);
    }
    
    void setForcus(bool focus){
        isFocus = focus;
    }
    
    void update(){
        if(isFocus){
            mRibbon.setHead(mTarget);
            mTarget.update();
            mRibbon.update();
        }
    }
    
    void draw(){
        ofSetColor(mColor);
        mRibbon.draw();
        
        const ofPoint &head = mRibbon.getHead();
    
        ofDrawSphere(head, 0.5);
    }
};

class dpHakoVisServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpVisServoPendulum"; }
    
	void setupControlPanel(){
        
        mScale.set(200,100);
        
        ramGetGUI().addSlider("distance", 10.0, 800.0, &mCamDistance);
        ramGetGUI().addSlider("scaleX", 100.0, 500.0, &mScale.x);
        ramGetGUI().addSlider("scaleY", 100.0, 500.0, &mScale.y);
        ramGetGUI().addSlider("zSpeed",0.01,2.0,&mTargetPosZSpeed);
    }
    void setup(){
    
        for(int i = 0; i < MAX_RIBBONS; i++){
            mRibbons.push_back(dpRibbonControler());
            mRibbons.back().setup();
        }
        
        mCam.setVFlip(true);
        
        mRibbons[0].setForcus(true);
        
        mRibbons[0].setColor(dpColor::PALE_PINK_HEAVY);
        mRibbons[1].setColor(dpColor::DARK_PINK_LIGHT);
 
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/ServoPendulum/contour/boundingRect");

    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/ServoPendulum/contour/boundingRect"){
                
                int num = m.getArgAsInt32(0);
                
                if(num != 0){
                    
                    ofPoint tmp;
                
                    for(int i = 1; i < num * 5 + 1; i+=5){
                        tmp.x += m.getArgAsFloat(i + 1) + m.getArgAsFloat(i + 3) * 0.5 - 0.5;
                        tmp.y += m.getArgAsFloat(i + 2) + m.getArgAsFloat(i + 4) * 0.5 - 0.5;
                    }
                
                    tmp /= num;
                
                    tmp *= ofPoint(mScale.x,mScale.y);
                    tmp += ofPoint(SINGLE_SCREEN_WIDTH * 0.5,
                                   SINGLE_SCREEN_HEIGHT * 0.5);
                
                     mRibbons[mTargetNum].setTarget(ofPoint(tmp.x,tmp.y,mPosZ.val));
                }
            }
        }
    }
    
    void update(){
        
        mTargetPosZ += mTargetPosZSpeed;
        if(mTargetPosZ > 1000 || mTargetPosZ < 0){
            mTargetPosZSpeed *= -1;
            mRibbons[mTargetNum].setForcus(false);
            mTargetNum++;
            mTargetNum %= mRibbons.size();
            mRibbons[mTargetNum].setForcus(true);
        }
        mPosZ.imSet(mTargetPosZ);
        receiveOsc();
    
        for(auto &v:mRibbons){
            v.update();
        }
        
        mPosZ.update();

    }
    void draw(){
        ofDisableDepthTest();
        ofPushMatrix();
        ofScale(0.25,0.25,4.0);
        mCam.setPosition(SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5, mPosZ.val + mCamDistance);
        
        //mCam.setTarget(ofPoint(dpGetFirstScreenCenter().x,dpGetFirstScreenCenter().y,mZCounter));
        mCam.begin(dpGetFirstScreenViewPort());
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255,255,255,150);
        for(auto &v:mRibbons){
            v.draw();
        }
        mCam.end();
        ofPopMatrix();
    }
    
private:
    static const int MAX_RIBBONS = 2;
    vector<dpRibbonControler>mRibbons;
    KezSlide mPosZ;
    ofEasyCam mCam;
    float mTargetPosZ = 0;
    float mTargetPosZSpeed = 0.2;
    float mCamDistance;
    
    ramOscReceiveTag mReceiver;
    int mTargetNum = 0;
    
    ofPoint mScale;
};

#endif
