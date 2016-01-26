//
//  middleScene.h
//  example-emptyScene
//
//  Created by itotaka on 12/31/15.
//
//

#pragma once

#define EXTEND_VAL 5

#include "ramMain.h"

class MiddleScene : public ramBaseScene
{
    
public:
	
	string getName() const { return "Middle"; }
    
    void setupControlPanel();
    void setup();
    void update();
    void draw();
    
    void drawActor(const ramActor& actor);
    void drawRigid(const ramRigidBody &rigid);
    void onActorSetup(const ramActor &actor);
    void onActorExit(const ramActor &actor);
    void onRigidSetup(const ramRigidBody &rigid);
    void onRigidExit(const ramRigidBody &rigid);
	
private:
	
    void onPanelChanged(ofxUIEventArgs &e);
    void changeBodyExtend();
    void changeLLegExtend();
    void changeRLegExtend();
    void changeLArmExtend();
    void changeRArmExtend();
    void changeExtended(ofxUIToggle* toggle, ofxUISlider* groupSlider, int from, int to);
    
    void setSlidersVal(ofxUIToggle* toggle, ofxUISlider* groupSlider, ofxUIToggle* extend, int from, int to, float val);
    void setSlidersVal(ofxUISlider* slider, int from, int to);
    void setSlidersValAll(ofxUIToggle* toggle, float val);
    
    void saveData(string filename, ofxUIToggle* toggle);
    void loadData(string filename, ofxUIToggle* toggle);

    ofxUISlider *mSliders[ramActor::NUM_JOINTS];
    ofxUIToggle *mHalfToggles[ramActor::NUM_JOINTS];
    bool    bExtended[ramActor::NUM_JOINTS];
    
    ofxUISlider *mRightArm;
    ofxUISlider *mLeftArm;
    ofxUISlider *mRightLeg;
    ofxUISlider *mLeftLeg;
    ofxUISlider *mBody;

    ofxUISlider *sliderBody;
    ofxUISlider *sliderLLeg;
    ofxUISlider *sliderRLeg;
    ofxUISlider *sliderLArm;
    ofxUISlider *sliderRArm;
    
    ofxUIToggle *bHalfAll;
    ofxUIToggle *bDancer1All;
    ofxUIToggle *bDancer2All;
    ofxUIToggle *bExtendAll;
    ofxUIToggle *bUnExtendAll;
    
    ofxUIToggle *bHalfRightArm;
    ofxUIToggle *bExtendRightArm;

    ofxUIToggle *bHalfLeftArm;
    ofxUIToggle *bExtendLeftArm;

    ofxUIToggle *bHalfRightLeg;
    ofxUIToggle *bExtendRightLeg;

    ofxUIToggle *bHalfLeftLeg;
    ofxUIToggle *bExtendLeftLeg;

    ofxUIToggle *bHalfBody;
    ofxUIToggle *bExtendBody;
    
    ofxUISlider *cameraX;
    ofxUISlider *cameraY;
    ofxUISlider *cameraZ;
    ofxUISlider *interGap;
    
    ofxUIToggle *save[5];
    ofxUIToggle *load[5];
    
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    const float dim = 16.0f;


//	float mySlider;
//	bool myToggle;
};

