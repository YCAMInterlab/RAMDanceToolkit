//
//  MixMonsterScene.h
//  example-emptyScene
//
//  Created by itotaka on 1/8/16.
//
//

#pragma once

#include "ramMain.h"

class MixMonsterScene : public ramBaseScene
{
    
public:
	
	string getName() const { return "MixMonster"; }
    
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
