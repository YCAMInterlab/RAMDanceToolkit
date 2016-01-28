//
//  Donuts_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#ifndef Donuts_ext_h
#define Donuts_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class Donuts_ext : public ramBaseScene
{
    ramMotionExtractor mMex;
    ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
    bool mNodeVisibility[ramActor::NUM_JOINTS];
    
    float mNumDuplicate;
    float mRadius;
    float mScale;
    float mBoxSize;
    ofVec3f mTranslate;
    bool mShowActor;
    bool mResetAll;
    bool mToggleAll;
    
    int mNumDuplicates;
    float mRadian;
    
public:
    void setupControlPanel();
    void setup();
    void update();
    void drawDonuts(const ramNodeArray &nodeArray);
    void draw();
    void drawActor(const ramActor& actor);
    void drawRigid(const ramRigidBody &rigid);
    void onValueChanged(ofxUIEventArgs& e);
    void clear();
    string getName() const;
    void setAllVisiblity(bool b);
};


#endif /* Donuts_ext_h */
