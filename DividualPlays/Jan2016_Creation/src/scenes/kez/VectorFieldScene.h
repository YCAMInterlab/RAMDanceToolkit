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

class VectorFieldScene : public ramBaseScene{
public:
    
    string getName() const { return "VectorFieldScene"; }
    
    void setup(){
        
        mFloorSize = ramGetGUI().getPreferencesTab().getFloorSize();
        
        for (int i = 0; i < NUM; i++){
            VFParticle tmp;
            tmp.setInitialCondition(ofRandom(0,mFloorSize),ofRandom(0,mFloorSize),0,0);
            mParticles.push_back(tmp);
        
            mColors[i].set(1,1,1,1);
        }
        
        mVbo.setVertexData(mVerts, NUM, GL_DYNAMIC_DRAW);
        mVbo.setColorData(mColors, NUM, GL_DYNAMIC_DRAW);
        
        mVF.setupField(102, 76, mFloorSize,mFloorSize);
        
        isDrawVF = false;
    }
    void setupControlPanel(){}
    
    void addVectorCircle(float x,float y){
        mVF.addInwardCircle(x + mFloorSize * 0.5,y + mFloorSize * 0.5,100,0.2);
    }
    
    void update(){
        
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &array = getNodeArray(i);
            
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
                
                if(j == ramActor::JOINT_LEFT_HAND || j == ramActor::JOINT_RIGHT_HAND){
                    addVectorCircle(node.getGlobalPosition().x,node.getGlobalPosition().z);
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
        
        
        
    }
    void draw(){
        ramBeginCamera();
        ofSetColor(255,255,255);
        
        ofPushMatrix();
        ofTranslate(-mFloorSize * 0.5, -mFloorSize * 0.5);
        
        ofTranslate(mFloorSize * 0.25, mFloorSize * 0.25);
        ofRotateX(ofGetMouseX());
        ofTranslate(-mFloorSize * 0.25, -mFloorSize * 0.25);
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        if(isDrawVF)mVF.draw();
        
        mVbo.draw(GL_POINTS, 0, NUM);

        ofPopMatrix();
        
        ramEndCamera();
        
        //ofDisableDepthTest();
        //ofRect(0,0,1000,1000);
    }
    void drawActor(const ramActor& actor){
        
    }
    void onPanelChanged(ofxUIEventArgs& e){}
    
private:
    static const int NUM = 30000;
    vector<VFParticle>mParticles;
    ofVbo mVbo;
    ofVec2f mVerts[NUM];
    ofFloatColor mColors[NUM];

    VectorField mVF;
    
    bool isDrawVF = true;
    
    float mFloorSize;
};

#endif
