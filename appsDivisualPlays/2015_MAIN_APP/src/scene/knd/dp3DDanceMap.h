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

class dp3DDanceMap : public ramBaseScene{
public:
    string getName() const { return "dp3DDanceMap"; }
    void setupControlPanel(){
        
        ramGetGUI().addIntSlider("div", 1, 6, &mDiv);
        ramGetGUI().addSlider("scale", 0.5, 20.0, &mScale);
        ramGetGUI().addSlider("rotThresh", 0.1, 20.0, &mRotThresh);
        ramGetGUI().addToggle("all",&isAllDraw);
        ramGetGUI().addButton("randomize");
        ramGetGUI().addButton("randomDiv");
        ramGetGUI().addToggle("extend", &isExtend);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(300,200));
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dp3DDanceMap::onPanelChanged);
    }
    void setup(){
        mCube.setup();
    }
    void update(){
        ramSetViewPort(dpGetFirstScreenViewPort());
        mCube.update();
        
        if(isExtend)mCube.extendEase();
        else mCube.shrink();
        
        mMotionExtractor.update();
        setRot();
        mRot.update();
        
       /* cout << mRot.x << endl;
        cout << mRot.y << endl;
        cout << mRot.z << endl;*/
    
    }
    
    void setRot(){
        
        ofPoint euler = mMotionExtractor.getRotationAt(0).getEuler();
        ofVec3f quatVel = mMotionExtractor.getVelocityAt(0);
        
        if(quatVel.x > mRotThresh)mRotTarget.x = (convertToGrad(euler.x));
        
        if(quatVel.y > mRotThresh)mRotTarget.y = (convertToGrad(euler.y));
        
        if(quatVel.z > mRotThresh)mRotTarget.z = (convertToGrad(euler.z));

        mRot.set(mRotTarget);
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
        ramBeginCamera();
        
        ofPushMatrix();
        ofRotateX(mRot.x);
        ofRotateY(mRot.y);
        ofRotateZ(mRot.z);
        mCube.draw();
        ofPopMatrix();
        ramEndCamera();
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
    
    float mScale = 1.0;
    float mRotThresh = 0.0;
    KezSlidePoint mRot;
    ofPoint mRotTarget;
};

#endif
