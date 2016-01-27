//
//  MapTri.h
//  Ice
//
//  Created by kezzardrix2 on 2014/12/24.
//
//

#ifndef Ice_MapTri_h
#define Ice_MapTri_h

class dpIceMapLine{
public:
    
    dpIceMapLine(ofVec2f v1,ofVec2f v2){
        setup(v1,v2);
    }
    
    void setup(ofVec2f v1,ofVec2f v2){
        mOrigin[0].set(v1);
        mOrigin[1].set(v2);
        
        mVerts[0].imSet(v2);
        mVerts[1].imSet(v2);
        
        mAlpha.imSet(1.0);
        
        mUnique = ofRandom(0,10000);
        
        int rnd = (int)ofRandom(0,3);
        
        mTargetMode = rnd;
   
    }
    
    void setAlpha(float alpha){
        mAlpha.imSet(alpha);
    }
    
    void kill(){
        killAlpha();
        killMode();
        hasShrinked = true;
    }
    
    void gen(){
        if(hasShrinked){
            randomizeMode();
            hasShrinked = false;
        }
    }
    
    void killAlpha(){
        mAlpha.speed = 0.1;
        mAlpha.set(0);
    }
    
    void imSetMode(int mode){
        
        if(mode == 0){
            mVerts[0].imSet(mOrigin[0]);
            mVerts[1].imSet(mOrigin[1]);
        }else if(mode == 1){
            mVerts[0].imSet(mOrigin[0]);
            mVerts[1].imSet(mOrigin[0]);
        }else if(mode == 2){
            mVerts[0].imSet(mOrigin[1]);
            mVerts[1].imSet(mOrigin[1]);
        }
      
    }
    
    void setMode(int mode){
        
        if(mode == 0){
            mVerts[0].set(mOrigin[0]);
            mVerts[1].set(mOrigin[1]);
        }else if(mode == 1){
            mVerts[0].set(mOrigin[0]);
            mVerts[1].set(mOrigin[0]);
        }else if(mode == 2){
            mVerts[0].set(mOrigin[1]);
            mVerts[1].set(mOrigin[1]);
        }
        
    }
    
    void setTargetMode(){
        setMode(mTargetMode);
    }
    
    void imSetTargetMode(){
        imSetMode(mTargetMode);
    }
    
    void killMode(){
        mVerts[0].speed = 0.1;
        mVerts[0].set((mOrigin[0] + mOrigin[1]) * 0.5);
        mVerts[1].speed = 0.1;
        mVerts[1].set((mOrigin[0] + mOrigin[1]) * 0.5);
    }
    
    void randomizeMode(){
        mTargetMode = (int)ofRandom(0,3);
    }

    void update(){
        mAlpha.update();
        for(auto &v:mVerts){
            v.update();
        }
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(255,255,255,mAlpha.val * 255.0);
        ofLine(mVerts[0].x,mVerts[0].y,
               mVerts[1].x,mVerts[1].y);
        ofPopStyle();
    }
    
    void drawDebug(){
        ofSetColor(255,255,255,255);
        ofLine(mOrigin[0],mOrigin[1]);
    }
    
    float getAlpha(){
        return mAlpha.val;
    }
    
    ofVec2f getVert(int idx){
        if(idx >= 0 && idx < VERTEX_NUM)return mVerts[idx];
        else return ofVec2f(0,0);
    }
    
private:
    KezSlide mAlpha;
    static const int VERTEX_NUM = 2;
    ofPoint mOrigin[VERTEX_NUM];
    KezSlidePoint mVerts[VERTEX_NUM];
    
    float mUnique;
    
    int mTargetMode = 0;
    
    bool hasShrinked = false;
};

class dpIceMapTri{
public:
    dpIceMapTri(ofVec2f v1,ofVec2f v2,ofPoint v3){
        dpIceMapLine a(v1,v2);
        dpIceMapLine b(v1,v3);
        dpIceMapLine c(v2,v3);
        
        mLines.push_back(a);
        mLines.push_back(b);
        mLines.push_back(c);
        
        mLineVerts.clear();
        mLineAlpha.clear();
        
        mLineVerts.assign(2 * LINE_NUM, ofVec2f());
        mLineAlpha.assign(2 * LINE_NUM,float());
        
        mExtendThreshNum = ofRandom(1,20);
    }
    
    void setAlpha(float alpha){
        for(auto &v:mLines){
            v.setAlpha(alpha);
        }
    }
    
    void setMode(int mode){
        
        for(auto &v:mLines){
            v.setMode(mode);
        }
        
    }
    
    void gen(){
        if(hasShrinked){
            randomizeMode();
            hasShrinked = false;
        }
    }
    
    void randomizeMode(){
        
        for(auto &v:mLines){
            v.randomizeMode();
        }
        
    }
    
    void shrink(){
        for(auto &v:mLines){
            v.killAlpha();
            v.killMode();
        }
        
        hasShrinked = true;
    }
    
    void extendEase(){
        setTargetMode();
        setAlpha(1.0);
    }
    
    void extendByThresh(int num){
        if(num > mExtendThreshNum)extendEase();
        else shrink();
    }
    
    void setExtendThreshNum(int num){
        mExtendThreshNum = ofRandom(1,num);
    }
    
    void setTargetMode(){
        for(auto &v:mLines){
            v.setTargetMode();
        }
    }
    
    void imSetTargetMode(){
        for(auto &v:mLines){
            v.imSetTargetMode();
        }
    }
    
    void update(){
        for(int i = 0; i < mLines.size(); i++){
            
            mLineAlpha[i * 2] = mLines[i].getAlpha();
            mLineAlpha[i * 2 + 1] = mLines[i].getAlpha();
            mLineVerts[i * 2] = mLines[i].getVert(0);
            mLineVerts[i * 2 + 1] = mLines[i].getVert(1);
            mLines[i].update();
        }
    }
    
    vector<ofVec2f> & getVerts(){
        return mLineVerts;
    }
    
    vector<float> & getAlpha(){
        return mLineAlpha;
    }
    
    void draw(){
        /*for(auto &v:mLines){
            v.draw();
        }*/
    }
private:
    vector<dpIceMapLine>mLines;
    bool hasShrinked = false;
    
    static const int LINE_NUM = 3;
    
    vector<float> mLineAlpha;
    vector<ofVec2f> mLineVerts;
    
    int mExtendThreshNum;
};

#endif
