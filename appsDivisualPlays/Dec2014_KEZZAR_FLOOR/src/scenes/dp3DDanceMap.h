//
//  dp3DDanceMap.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_dp3DDanceMap_h
#define RAMDanceToolkit_dp3DDanceMap_h

#include "dpIceMapCube.h"
#include "ramMotionExtractor.h"
#include "KezSlidePoint.h"
#include "dpConstants.h"
#include "ramFloorQuadWarper.h"

class dp3DDanceMap : public ramBaseScene{
public:
    string getName() const { return "dp3DDanceMap"; }
    void setupControlPanel(){
        
        ramGetGUI().addToggle("Osc", &isReceiveOsc);
        ramGetGUI().addIntSlider("div", 1, 6, &mDiv);
        ramGetGUI().addSlider("scale", 0.5, 20.0, &mScale);
        ramGetGUI().addSlider("rotThresh", 0.1, 20.0, &mRotThresh);
        ramGetGUI().addToggle("all",&isAllDraw);
        ramGetGUI().addButton("randomize");
        ramGetGUI().addButton("randomDiv");
        ramGetGUI().addToggle("extend", &isExtend);
        ramGetGUI().addIntSlider("lineWidth", 1, 10, &mLineWidth);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(300,200));
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dp3DDanceMap::onPanelChanged);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Ice/pixelate");
    }
    
    void setup(){
        mCube.setup();
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/Ice/pixelate"){
                
                int width = m.getArgAsInt32(0);
                int height = m.getArgAsInt32(1);
                
                int sum = width * height;
                int argNum = ceil(sum / 64) + 1;
                
                string str;
                
                for(int i = 2; i < argNum + 2; i++){
                    
                    string tmp = ofToBinary(m.getArgAsInt64(i));
                    
                    for (int j = tmp.size()-1; j>=0; j--) {
                        str += tmp.at(j);
                    }
                    
                }
                
                for(int i = 2; i < argNum + 2; i++){
                    str += ofToBinary(m.getArgAsInt64(i));
                }
                
                for(int j = 0; j < height; j++){
                    for(int i = 0; i < width; i++){
                    
                        int idx = j * width + i;
                        if(str[idx] == '1')mCube.extendEach(width,height,i,j);
                        else mCube.shrink();
                    }
                }
                
            }
        }
        
    }
    
    void update(){
        
        if(isReceiveOsc)receiveOsc();
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        mCube.update();
        
        if(isExtend)mCube.extendEase();
        else mCube.shrink();
        
        mMotionExtractor.update();
        setRot();
        mRot.update();
        mAngle.update();
        
       /* cout << mRot.x << endl;
        cout << mRot.y << endl;
        cout << mRot.z << endl;*/
    
    }
    
    void setRot(){
        
        ofPoint quatVel = mMotionExtractor.getRotateVelocityAt(0).getEuler();
        
        if(quatVel.x > mRotThresh)makeRotate(X,quatVel.x);
        
        if(quatVel.y > mRotThresh)makeRotate(Y,quatVel.y);
        
        if(quatVel.z > mRotThresh)makeRotate(Z,quatVel.z);

    }
    
    void makeRotate(int axis,float val){
        
        int dir = 1;
        if(val > 0.0)dir = -1;
        
        if(axis == X){
            
            ofQuaternion quat(90,  ofPoint(dir, 0, 0));
            mQuat *= quat;
            
        }else if(axis == Y){
            
            ofQuaternion quat(90,  ofPoint(0, dir, 0));
            mQuat *= quat;
            
        }else if(axis == Z){
            
            ofQuaternion quat(90,  ofPoint(0, 0, dir));
            mQuat *= quat;
            
        }
        
        float angle;
        ofPoint rot;
        mQuat.getRotate(angle,rot.x,rot.y,rot.z);
        
        mAngle.set(angle);
        mRot.set(rot);
    
    }
    
    int convertToGrad(float rot){
     
        float tmp = ceil(ofMap(rot, -180, 180, 0,4,true)) * 90 - 180;
        
       /* float tmp = 0.0;
        
        if(rot < 0.0)tmp = -90.0;
        else tmp = 90.0;*/
        
        return tmp;
        
    }
    
    void drawActor(const ramActor& actor){
        
    }
    
    void draw(){
        
        ofSetLineWidth(mLineWidth);
        
        ramFloorQuadWarper::instance().begin();
        ramBeginCamera();
        
        ofPushMatrix();
        ofRotate(mAngle.val,mRot.x,mRot.y,mRot.z);
        mCube.draw();
        ofPopMatrix();
        ramEndCamera();
        
        ramFloorQuadWarper::instance().end();
        ramFloorQuadWarper::instance().draw();
    }
    
    void onPanelChanged(ofxUIEventArgs &e){
        string name = e.widget->getName();
        
        if(name == "randomize"){
            mCube.randomizeMode();
        }
        
        if(name == "randomDiv"){
            mCube.randomDiv(mDiv);
        }
        
        if(name == "all"){
            mCube.setAllDraw(isAllDraw);
        }
        
        if(name == "scale"){
            mCube.setScale(mScale);
        }
        
        if(name == "div"){
            mCube.changeDiv(mDiv);
        }
    }
    
private:
    dpIceMapCube mCube;
    ramMotionExtractor mMotionExtractor;
    
    int mDiv = 1;
    
    bool isAllDraw = false;
    bool isExtend = true;
    bool isReceiveOsc = false;
    
    float mScale = 1.0;
    float mRotThresh = 0.0;
    KezSlidePoint mRot;
    KezSlide mAngle;
    
    ofPoint mRotTarget;
    
    ramOscReceiveTag mReceiver;
    
    int mLineWidth = 2;
    ofQuaternion mQuat;
    
    enum ROT_DIR{
        X,
        Y,
        Z
    };
    
};

#endif
