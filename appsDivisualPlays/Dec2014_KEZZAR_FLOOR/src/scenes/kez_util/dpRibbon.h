//
//  Ribbon.h
//  HakoVisTri
//
//  Created by kezzardrix2 on 2015/01/06.
//
//

#ifndef TriParticle_Ribbon_h
#define TriParticle_Ribbon_h

template <int N = 120>
class dpRibbon{
public:
    vector<ofPoint>mPts;
    static const int MAX_NUM = N;
    
    float mRandVal = 0;
    float mWidthMin = 1.0;
    float mWidthMax = 10.0;
    
    bool enable = true;
    
    int mTailCounter = 0;
    
    enum DIR_MODE{
        NORMAL,
        BACK,
        STOP
    };
    
    int mDirMode = NORMAL;
        
    dpRibbon(){
        
    }
    
    const ofPoint & getHead(){
        return mPts[mPts.size() - 1];
    }
    
    void setHead(ofPoint _pos){
    
        mPts[0] = _pos;
        
    }
    
    void setMode(DIR_MODE mode){
        mDirMode = mode;
    }
    
    void setAll(ofPoint _pos){
        for(auto &v:mPts){
            v = _pos;
        }
    }
    
    void setWidth(float _min,float _max){
        mWidthMin = _min;
        mWidthMax = _max;
    }
    
     void setup(){
         
         mPts.clear();
         
        for(int i = 0; i < MAX_NUM; i++){
            mPts.push_back(ofPoint(0,0,0));
        }
         
        mDirMode = NORMAL;
        resetTailBack();
    }
    
     void update(){
         if(mDirMode == NORMAL){
             
             swapPts();
             resetTailBack();
             
         }else if(mDirMode == BACK)tailBack();
         
    }
    
    
     void draw(){
        
        if(enable){
            
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            for(unsigned int i = 1; i < mPts.size(); i++){
                
                ofVec3f thisPoint = mPts[i-1];
                ofVec3f nextPoint = mPts[i];
                
                ofVec3f direction = (nextPoint - thisPoint);
                
                float distance = direction.length();
                
                ofVec3f unitDirection = direction.normalized();
                
                ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
                ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
                
                float thickness = ofMap(distance, 0, 60, mWidthMin, mWidthMax, true);

                ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
                ofVec3f rightPoint = thisPoint+toTheRight*thickness;
                
                mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z) + ofPoint(ofRandom(-mRandVal,mRandVal),ofRandom(-mRandVal,mRandVal),ofRandom(-mRandVal,mRandVal)));
                mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z) + ofPoint(ofRandom(-mRandVal,mRandVal),ofRandom(-mRandVal,mRandVal),ofRandom(-mRandVal,mRandVal)));
            
            }
            
            ofPushStyle();
            mesh.draw();
            ofPopStyle();
            
        }
    }
    
protected:
    
    void swapPts(){
        for(int i = 1; i < MAX_NUM; i++){
            ofPoint tmp = mPts[i-1];
            mPts[i - 1] = mPts[i];
            mPts[i] = tmp;
        }
        
    }
    
    void tailBack(){
        if(mTailCounter < mPts.size()){
            
            for(int i = 0; i < mTailCounter; i++){
                mPts[i] = mPts[mTailCounter];
            }
            
            mTailCounter++;
            
        }
    }
    
    void resetTailBack(){
        mTailCounter = 0;
    }
 
};

#endif

