//
//  VectorField.h
//  afterWorkshop
//
//  Created by kezzardrix2 on 2014/12/20.
//
//

#ifndef afterWorkshop_VectorField_h
#define afterWorkshop_VectorField_h

#include "VectorField.h"
#include "VFParticle.h"
#include "KezConstants.h"

#include "ramCenteredActor.h"

class VectorFieldScene : public ramBaseScene{
public:
    
    string getName() const { return "VectorFieldScene"; }
    
    void setup(){
        
        mFloorSize = SINGLE_SCREEN_HEIGHT;//ramGetGUI().getPreferencesTab().getFloorSize();
        
        for (int i = 0; i < NUM; i++){
            VFParticle tmp;
            tmp.setInitialCondition(ofRandom(0,mFloorSize),ofRandom(0,mFloorSize),0,0);
            mParticles.push_back(tmp);
        
            mColors[i].set(1,1,1,0.7);
        }
        
        mVbo.setVertexData(mVerts, NUM, GL_DYNAMIC_DRAW);
        mVbo.setColorData(mColors, NUM, GL_DYNAMIC_DRAW);
        
        mVF.setupField(76, 76, mFloorSize,mFloorSize);

    }
    void setupControlPanel(){
        ramGetGUI().addToggle("isDrawVF", &isDrawVF);
        ramGetGUI().addSlider("multHandPos", 1.0, 20.0, &mMult);
        ramGetGUI().addSlider("rad", 50.0, 500.0, &mRad);
    }
    
    void addVectorCircle(float x,float y){
        mVF.addInwardCircle(x * mMult + mFloorSize * 0.5,y * mMult + mFloorSize * 0.5,mRad,0.2);
    }
    
    void update(){
        
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &arr = getNodeArray(i);
            
            const ramNodeArray array = mCentered.update(arr);
            
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
                
                if(j == ramActor::JOINT_LEFT_HAND || j == ramActor::JOINT_RIGHT_HAND){
                    addVectorCircle(node.getGlobalPosition().x - array.getCentroid().x,
                                    node.getGlobalPosition().z - array.getCentroid().z);
                }
            }
        }
        
        for (int i = 0; i < mParticles.size(); i++){
            
            mParticles[i].resetForce();
            
            ofVec2f frc;
            frc = mVF.getForceFromPos(mParticles[i].pos.x, mParticles[i].pos.y);
            
            mParticles[i].addForce(frc.x, frc.y);
            mParticles[i].addDampingForce();
            mParticles[i].bounceOffWalls();
            mParticles[i].update();
            
            mVerts[i].set(mParticles[i].pos);
        }
        
        //ベクトル場の力の減衰
        mVF.fadeField(0.95f);
        
        mVbo.updateVertexData(mVerts, NUM);
        mVbo.updateColorData(mColors, NUM);
        
        ramSetViewPort(getFirstViewPort());
        
        
    }
    void draw(){
        
        //ramBeginCamera(getSecondViewPort());
        
        ofSetColor(255,255,255);
        float val = 0.5;
        
        ofPushMatrix();
        ofTranslate(SINGLE_SCREEN_WIDTH  - (mFloorSize - SINGLE_SCREEN_WIDTH) * 0.5,0.0);
        /*ofTranslate(mFloorSize * val, mFloorSize * val);
        ofRotateX(ofGetMouseX());
        ofTranslate(-mFloorSize * val, -mFloorSize * val);*/
        /*
         ofTranslate(mFloorSize * 0.25, mFloorSize * 0.25);
         ofRotateX(ofGetMouseX());
         ofTranslate(-mFloorSize * 0.25, -mFloorSize * 0.25);*/
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        if(isDrawVF)mVF.draw();
        
        glPointSize(4);
        mVbo.draw(GL_POINTS, 0, NUM);
        
        
        ofPopMatrix();
       // ramEndCamera();
        
        //ofDisableDepthTest();
        //ofRect(0,0,1000,1000);
    }
    void drawActor(const ramActor& actor){
       
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &arr = getNodeArray(i);
            
            const ramNodeArray array = mCentered.update(arr);
            
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
                
                if(j == ramActor::JOINT_LEFT_HAND || j == ramActor::JOINT_RIGHT_HAND){
                    ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                           node.getGlobalPosition().x * mMult,node.getGlobalPosition().y + 1000.0,node.getGlobalPosition().z * mMult);
                }
                
                float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
                ofDrawBox(node.getGlobalPosition(), jointSize);
                ramLine(node);
                
            }
            
        }
        
        
    }
    void onPanelChanged(ofxUIEventArgs& e){}
    
private:
    static const int NUM = 20000;
    vector<VFParticle>mParticles;
    ofVbo mVbo;
    ofVec2f mVerts[NUM];
    ofFloatColor mColors[NUM];

    VectorField mVF;
    
    bool isDrawVF = true;
    
    float mFloorSize;
    
    float mMult = 1.0;
    
    ramCenteredActor mCentered;
    
    float mRad = 100.0;
    
};

#endif
