//
//  dpIceMapController.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_dpIceMapController_h
#define RAMDanceToolkit_dpIceMapController_h

#include "dpIceMapFace.h"
#include "dpConstants.h"
class dpIceMapGrid{
public:
    
    void setup(ofPoint offset,float size){
        mOffset = offset;
        mSize = size;
        reset();
    }
    
    void reset(){
        mMaps.clear();
        mMaps.assign(mDiv * mDiv, dpIceMapFace());
        
        int size = mSize / mDiv;
        
        int counter = 0;
        
        for(int i = 0; i < mDiv; i++){
            for(int j = 0; j < mDiv; j++){
                                
                mMaps[j * mDiv + i].setup(ofPoint(i * size + size * 0.5,j * size + size * 0.5), ofPoint(size,size));
                
                counter++;
                counter %= 2;
            }
        }

    }
    
    void extendEach(int idx){
        if(idx >= 0 && idx < mMaps.size()){
            mMaps[idx].extend();
        }
    }
    
    void extendEach(int width,int height, int i, int j){
        int idxX = ofMap(i,0,width,0,mDiv,true);
        int idxY = ofMap(j,0,height,0,mDiv,true);
        
        int idx = idxY * mDiv + idxX;
        extendEach(idx);
    }
    
    void update(){
        for(auto &v:mMaps){
            v.update();
        }
    }
    
    void extend(){
        for(auto &v:mMaps){
            v.extend();
        }
    }
    
    void extendEase(){
        for(auto &v:mMaps){
            v.extendEase();
        }
    }
    
    void shrink(){
        for(auto &v:mMaps){
            v.shrink();
        }
    }
    
    void randomizeMode(){
        for(auto &v:mMaps){
            v.randomizeMode();
        }
    }
    
    void draw(){
        
        ofPushMatrix();
        ofTranslate(mOffset);
        ofTranslate(-mSize * 0.5,-mSize * 0.5);
        
        ofTranslate(mSize * 0.5,mSize * 0.5);
        ofRotateX(mRot.x);
        ofRotateY(mRot.y);
        ofRotateZ(mRot.z);
        ofTranslate(-mSize * 0.5,-mSize * 0.5);
        
        for(auto &v:mMaps){
            v.draw();
        }
        
        ofPopMatrix();
    }
    
    void subdivision(){
        mDiv++;
        reset();
    }
    
    void reduction(){
        mDiv--;
        if(mDiv < 1)mDiv = 1;
        reset();
    }
    
    void setRot(ofPoint rot){
        mRot = rot;
    }
    
    ofPoint getOffset(){
        return mOffset;
    }
    
    void togAllDraw(){
        for(auto &v:mMaps){
            v.togAllDraw();
        }
    }
    
    void setAllDraw(bool flag){
        for(auto &v:mMaps){
            v.setAllDraw(flag);
        }
    }
    
    void changeDiv(int div){
        mDiv = div;
        if(mDiv < 1)mDiv = 1;
        reset();
    }
    
private:
    vector<dpIceMapFace>mMaps;
    int mDiv = 1;
    
    float mSize = 50.0;
    
    ofPoint mOffset;
    ofPoint mRot;
};

#endif
