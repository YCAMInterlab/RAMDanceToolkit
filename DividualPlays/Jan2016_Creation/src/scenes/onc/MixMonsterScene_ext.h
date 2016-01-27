//
//  MixMonsterScene_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#ifndef MixMonsterScene_ext_h
#define MixMonsterScene_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class MixMonsterScene_ext : public ramBaseScene
{
    
public:
    
    string getName() const { return "MixMonsterScene_ext"; }
    
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
    void reset();
    
    bool isAncestor(int ancestor, int child);
    vector<int> getNonChildren(int ancestor);
    int getRandomNonChild(int i);
    void attach(int child, int parent);
    
    ramMotionExtractor mex;
    
    vector<int> treeBase, treeSwap;
    vector<float> lengthScale;
    vector<bool>  bVisible;
    //    vector<bool>  bRotate;
    ramNodeArray monsterArray;
    float minScale, maxScale, randomizationAmount; //, rotationAngle;
    //    ofPoint rotationVector;
    bool needToReset, randomLine, randomizeTopology, randomizeGeometry, sidebyside;
    
    ofxUISlider *cameraX;
    ofxUISlider *cameraY;
    ofxUISlider *cameraZ;
    ofxUISlider *interGap;
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    const float dim = 16.0f;
    
};


#endif /* MixMonsterScene_ext_h */
