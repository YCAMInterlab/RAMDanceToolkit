//
//  KezDelaunayMesh.h
//  HakoVisTri
//
//  Created by kezzardrix2 on 2015/01/06.
//
//

#ifndef HakoVisTri_KezDelaunayMesh_h
#define HakoVisTri_KezDelaunayMesh_h

#include "ofxDelaunay.h"
#include "KezBlinker.h"

class KezDelaunayMesh{
public:
    
    KezDelaunayMesh(){
        
    }
    
    void setup(){
        mPrePt.set(0,0,0);
        mPts.clear();
        for(int i = 0; i < PTS_MAX; i++){
            mPts.push_back(ofPoint(0,0,0));
        }
        setBitset();
    }
    
    void clear(){
        
        for(auto &v:mPts){
            v.set(0,0,0);
        }
        
        mBlink.setup(3, 3);
        
        setBitset();
    }
    
    void addPoint(ofPoint pt){
        
        ofPoint tmp(pt.x,pt.y,(pt - mPrePt).length() * 0.2);
        
        mPts[mPtsCounter].set(tmp);
        mPtsCounter++;
        mPtsCounter %= PTS_MAX;
        
        mAlpha = 255;
        mPrePt.set(pt);
        mHead = tmp;
        
        mHasFinishedAddPt = false;
        
        mRotSpeed.set(0,0,0);
        mRot.set(0,0,0);
        mBlink.stop();
    }
    
    void update(){
        mAlpha -= mAlphaReduc;
        mRot += mRotSpeed;
    }
    
    void finishAddPt(){
        mHasFinishedAddPt = true;
        
        if(isRotAfterFinish){
            changeRot();
            mBlink.start();
        }
    }
    
    void changeRot(){
        int rnd = ofRandom(0,3);
        
        if(rnd == 0){
            mRotSpeed.set(ofRandom(-1.0,1.0),0,0);
        }else if(rnd == 1){
            mRotSpeed.set(0,ofRandom(-1.0,1.0),0);
        }else if(rnd == 2){
            mRotSpeed.set(0,0,ofRandom(-1.0,1.0));
        }else if(rnd == 3){
            mRotSpeed.set(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0));
        }
    }
    
    void draw(){
        
        if(mAlpha > 0 && mPts.empty() == false){
        
            mDelaunay.reset();
            
            vector<ofPoint>tmp,tmp2;
            
            for(auto &v:mPts){
                tmp.push_back(v * mScale);
            }
            
            for(int i = 0; i < tmp.size() - 1; i++){
                ofPoint a = tmp[i];
                bool isNear = false;
                for(int j = i + 1; j < tmp.size(); j++){
                    ofPoint b = tmp[j];
                    
                    if(a.distanceSquared(b) < 1.0){
                        isNear = true;
                    }
                }
                
                if(isNear == false)tmp2.push_back(a);
            }
            
            if(tmp2.size() >= 3){
                
                for(auto &v:tmp2){
                    mDelaunay.addPoint(v);
                }
                
                mDelaunay.triangulate();
                
                ofPushStyle();
                ofNoFill();
                ofSetColor(255,mAlpha);
                ofPushMatrix();

                ofRotateX(mRot.x);
                ofRotateY(mRot.y);
                ofRotateZ(mRot.z);
                
                if(mBlink.update()){
                    mDelaunay.draw();
                    mDelaunay.triangleMesh.drawVertices();
                
                    if(isShowFaces)drawEachTriangle(mDelaunay.triangleMesh);
                
                }
                
                if(mHasFinishedAddPt == false){
                    ofSetColor(255,0,0,mAlpha);
                    ofDrawSphere(mHead,mRad);
                    
                    ofSetColor(255,255,255,mAlpha);
                    ofLine(0,0,mHead.x,mHead.y);
                 
                }
                
                ofPopMatrix();
                ofPopStyle();
                
            }
        }
    }
    
    void drawEachTriangle(ofMesh &mesh){
        vector<ofPoint>pts;
        
        int num = mesh.getNumIndices();
        
        ofMesh tmp;
        
        int bitsetCounter = 0;
        
        for(int i = 0; i < num; i+=3){
            
            if(i < mBitset.size() && mBitset[bitsetCounter]){
            
                int idx1 = mesh.getIndex(i);
                int idx2 = mesh.getIndex(i + 1);
                int idx3 = mesh.getIndex(i + 2);
                
                tmp.addVertex(mesh.getVertex(idx1));
                tmp.addVertex(mesh.getVertex(idx2));
                tmp.addVertex(mesh.getVertex(idx3));
                
            }
            
             bitsetCounter++;
        }
        
        ofSetColor(200, mAlpha);
        tmp.drawFaces();
    }
    
    void setScale(float scale){
        mScale = scale;
    }
    
    void setAlphaReducSpeed(float speed){
        mAlphaReduc = speed;
    }
    
    void showFaces(bool flag){
        isShowFaces = flag;
    }
    
    void rotAfterFinish(bool flag){
        isRotAfterFinish = flag;
    }
    
private:
    ofxDelaunay mDelaunay;
    
    static const int PTS_MAX = 300;
    int mPtsCounter = 0;
    vector<ofPoint>mPts;
    
    float mScale = 1.0;
    float mAlphaReduc = 1.0;
    float mAlpha = 0;
    float mRad = 10.0;
    
    ofPoint mPrePt;
    ofPoint mHead;
    
    ofPoint mRot;
    ofPoint mRotSpeed;
    
    bool mHasFinishedAddPt = false;
    
    KezBlinker mBlink;
    
    bitset<PTS_MAX * 6>mBitset;
    
    void setBitset(){
        for(int i = 0; i < mBitset.size(); i++){
            
            if((int)ofRandom(0,4) == 0)mBitset[i] = true;
            else mBitset[i] = false;
        }
    }
    
    bool isRotAfterFinish = false;
    bool isShowFaces = false;
};

class KezDelaunayMeshController{
public:
    
    static const int MAX_MESH = 10;
    vector<KezDelaunayMesh>mDelaunays;
    
    int mCurrentMesh = 0;
    int mThresh = 4.0;
    
    bool mIsMeshChanged = false;

    void setup(){
        for(int i = 0; i < MAX_MESH; i++){
            mDelaunays.push_back(KezDelaunayMesh());
            mDelaunays.back().setup();
        }
    }
    
    void update(){
        for(auto &v:mDelaunays){
            v.update();
        }
    }
    
    void setScale(float scale){
        for(auto &v:mDelaunays){
            v.setScale(scale);
        }
    }
    
    void draw(){
    
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        glEnable(GL_POINT_SMOOTH);
        
        enablePointAtt();
        
        for(auto &v:mDelaunays){
            v.draw();
        }
        
        disablePointAtt();
        
    }
    
    void enablePointAtt(){
        float att[3];
        att[0] = 0.0;
        att[1] = 0.0;
        att[2] = 1.0f;
        glPointSize(2048 * 2);
        glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, att);
    }
    
    void disablePointAtt(){
        float att[3];
        att[0] = 1.0;
        att[1] = 0.0;
        att[2] = 0.0f;
        glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, att);
        glPointSize(1);
    }
    
    void addPoint(ofPoint pt){
        
        if(pt.length() > mThresh){
            
            mDelaunays[mCurrentMesh].addPoint(pt);
            mIsMeshChanged = false;
            
        }else{
            
            if(mIsMeshChanged == false){
                mIsMeshChanged = true;
                changeMesh();
            }
            
        }
    }
    
    void rotAfterFinish(bool flag){
        for(auto &v:mDelaunays){
            v.rotAfterFinish(flag);
        }
    }
    
    void showFaces(bool flag){
        for(auto &v:mDelaunays){
            v.showFaces(flag);
        }
    }
    
    void setAlphaReducSpeed(float speed){
        for(auto &v:mDelaunays){
            v.setAlphaReducSpeed(speed);
        }
    }
    
    void changeMesh(){
        mDelaunays[mCurrentMesh].finishAddPt();
        mCurrentMesh++;
        mCurrentMesh %= mDelaunays.size();
        mDelaunays[mCurrentMesh].clear();
    }
    
};

#endif
