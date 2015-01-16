//
//  dpIceMapCube.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_dpIceMapCube_h
#define RAMDanceToolkit_dpIceMapCube_h

#include "dpIceMapGrid.h"

class dpIceMapCube{
public:
    
    void setup(float size){
        mEachFaceSize = size;
        setup();
    }
    
    void setup(){
        
        mGrids.clear();
        
        for(int i = 0; i < FACE_NUM; i++){
            mGrids.push_back(dpIceMapGrid());
        }
        
        mGrids[FRONT].setup(ofPoint(0,0,mEachFaceSize * 0.5),mEachFaceSize);
        mGrids[BACK].setup(ofPoint(0,0,-mEachFaceSize * 0.5),mEachFaceSize);
        
        mGrids[TOP].setup(ofPoint(0,mEachFaceSize * 0.5,0),mEachFaceSize);
        mGrids[BOTTOM].setup(ofPoint(0,-mEachFaceSize * 0.5,0),mEachFaceSize);
        
        mGrids[RIGHT].setup(ofPoint(mEachFaceSize * 0.5,0,0),mEachFaceSize);
        mGrids[LEFT].setup(ofPoint(-mEachFaceSize * 0.5,0,0),mEachFaceSize);
        
        mGrids[TOP].setRot(ofPoint(90,0,0));
        mGrids[BOTTOM].setRot(ofPoint(90,0,0));
        mGrids[RIGHT].setRot(ofPoint(0,90,0));
        mGrids[LEFT].setRot(ofPoint(0,90,0));
    }
    
    void extend(){
        for(auto &v:mGrids){
            v.extend();
        }
    }
    
    void extendEase(){
        for(auto &v:mGrids){
            v.extendEase();
        }
    }
    
    void shrink(){
        for(auto &v:mGrids){
            v.shrink();
        }
    }
    
    void randomizeMode(){
        for(auto &v:mGrids){
            v.randomizeMode();
        }
    }
    
    void togAllDraw(){
        for(auto &v:mGrids){
            v.togAllDraw();
        }
    }
    
    void setAllDraw(bool flag){
        for(auto &v:mGrids){
            v.setAllDraw(flag);
        }
    }
    
    void changeDiv(int div){
        for(auto &v:mGrids){
            v.changeDiv(div);
        }
    }
    
    void randomDiv(int div){
        for(auto &v:mGrids){
            v.changeDiv((int)ofRandom(1,div));
        }
    }
    
    void setScale(float scale){
        mScale = scale;
    }
    
    void update(){
        for(auto &v:mGrids){
            v.update();
        }
    }
    
    void draw(){
        for(int i = 0; i < FACE_NUM; i++){
            ofPushMatrix();
            ofScale(mScale,mScale,mScale);
            mGrids[i].draw();
            ofPopMatrix();
        }
    }
    
private:
    static const int FACE_NUM = 6;
    vector<dpIceMapGrid>mGrids;
    
    float mEachFaceSize = 200.0;
    
    enum FACE_NAME{
        FRONT,
        BACK,
        TOP,
        BOTTOM,
        RIGHT,
        LEFT
    };
    
    float mScale = 1.0;
};

#endif
