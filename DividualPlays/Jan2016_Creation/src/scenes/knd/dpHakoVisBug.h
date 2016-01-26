//
//  dpHakoVisBug.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/13.
//
//

#ifndef RAMDanceToolkit_dpHakoVisBug_h
#define RAMDanceToolkit_dpHakoVisBug_h

class dpBugPt{
private:
    
    ofPoint mPos;
    KezSlidePoint mPrePos;
    
    int mMoveDelay = 0;
    int mMoveDelayLimit = 300;
    
    float mBrownVal = 2.0;
    
    float mAlpha = 0.0;
    
    float mUnique;
    
    void speedChange(){
        mBrownVal = 30.0;
        mAlpha -= 0.02;
    }
    
    ofPoint mVec;
    
public:
    
    void appear(ofPoint pos){
        mPos = pos;
        mPrePos.imSet(mPos);
        mBrownVal = 2.0;
        mAlpha = 1.0;
        mMoveDelay = 0;
        
        mUnique = ofRandom(0.0,10000);
        
        mBrownVal = 1.0;
        mVec.set(0,0,0);

        mPrePos.speed = 0.1;
    }
    
    void brown(){
        mBrownVal = 2.0;
        mPos += ofPoint(ofRandom(-mBrownVal,mBrownVal),
                        ofRandom(-mBrownVal,mBrownVal),
                        ofRandom(-mBrownVal,mBrownVal));
    }
    
    void setDelayLimit(int limit){
        mMoveDelayLimit = limit;
    }
    
    void noise(){
        mPos += mVec;
        
        mVec += ofPoint(ofSignedNoise(mPos.y, mPos.x + ofGetLastFrameTime(), mUnique),
                        ofSignedNoise(mPos.x, mPos.y + ofGetLastFrameTime(), mUnique),
                        ofSignedNoise(mPos.y, mPos.x * mPos.y + ofGetLastFrameTime(), mUnique)) * 1.0;
    }
    
    void update(){
        
        mMoveDelay++;
        if(mMoveDelay > mMoveDelayLimit){
            noise();
            mAlpha -= 0.01;
            setPrePos();
        }else{
            brown();
            setPrePos();
        }
        
        mPrePos.update();
      //  brown();
    }
    
    void setPrePos(){
        mPrePos.set(mPos);
    }
    
    ofPoint getPos(){
        return mPos;
    }
    
    ofPoint getPrePos(){
        return mPrePos;
    }
    
    float getAlpha(){
        return mAlpha;
    }
};

class dpHakoVisBug : public ramBaseScene{
public:
    
    string getName() const {return "dpVisStage";};
    
    void setupControlPanel(){
        ramGetGUI().addIntSlider("limit", 10, 300, &mLimit);
    }
    void setup(){
        
        for(int i = 0; i < MAX_PT; i++){
            mVerts.push_back(ofPoint(0,0,0));
            mColors.push_back(ofFloatColor(1,1,1,1));
        }
        
        ofFloatColor color(dpFloatColor::MAIN_COLOR.r,
                           dpFloatColor::MAIN_COLOR.g,
                           dpFloatColor::MAIN_COLOR.b);
        
        for(int i = 0; i < MAX_PT * 0.5; i++){
            
            mPts.push_back(dpBugPt());
            
            mPointVerts.push_back(ofPoint(0,0,0));
            mPointColors.push_back(color);
        }
        
        mVbo.setVertexData(&mVerts[0],mVerts.size(),GL_DYNAMIC_DRAW);
        mVbo.setColorData(&mColors[0],mColors.size(),GL_DYNAMIC_DRAW);

        mPointVbo.setVertexData(&mPointVerts[0],mPointVerts.size(),GL_DYNAMIC_DRAW);
        mPointVbo.setColorData(&mPointColors[0],mPointColors.size(),GL_DYNAMIC_DRAW);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Stage/contour/boundingRect");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisBug::onPanelChanged);
    }
    
    void receiveOsc(){
        while (mReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
    
        if(m.getAddress() == "/dp/cameraUnit/Stage/contour/boundingRect"){
        
            int num = m.getArgAsInt32(0);
        
            for(int i = 1; i < num * 5 + 1; i+=5){
    
                mPts[mCurrentPt].appear(ofPoint((m.getArgAsFloat(i+1) + m.getArgAsFloat(i+3) * 0.5 - 0.5) * SINGLE_SCREEN_HEIGHT,
                                                (m.getArgAsFloat(i+2) + m.getArgAsFloat(i+4) * 0.5 - 0.5) * SINGLE_SCREEN_HEIGHT));
                mCurrentPt++;
                mCurrentPt %= (int)(MAX_PT * 0.5);
                
                }
            }
        }
    }
    
    void update(){
        
        receiveOsc();
        
        for(int i = 0; i < MAX_PT * 0.5; i++){
            mPts[i].update();
            mVerts[i * 2] = mPts[i].getPos();
            mVerts[i * 2 + 1] = mPts[i].getPrePos();
            mColors[i * 2] = mPts[i].getAlpha();
            mColors[i * 2 + 1] = 0.0;
          //  mPts[i].setPrePos();
            
            mPointVerts[i] = mPts[i].getPos();
            mPointColors[i].a = mPts[i].getAlpha();
           
        }
        
        
        mVbo.updateVertexData(&mVerts[0],mVerts.size());
        mVbo.updateColorData(&mColors[0],mColors.size());
        
        mPointVbo.updateVertexData(&mPointVerts[0],mPointVerts.size());
        mPointVbo.updateColorData(&mPointColors[0],mPointColors.size());
    
    }
    
    void draw(){
      
        ramSetViewPort(dpGetFirstScreenViewPort());
        ramBeginCamera();
        ofSetLineWidth(4);
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        mVbo.draw(GL_LINES,0,mVerts.size());
        glPointSize(8);
        glEnable(GL_POINT_SMOOTH);
        mPointVbo.draw(GL_POINTS,0,mPointVerts.size());
        ramEndCamera();
    }
    
    void onPanelChanged(ofxUIEventArgs &e){
        string name = e.widget->getName();
        
        if(name == "limit"){
            for(auto &v:mPts){
                v.setDelayLimit(mLimit);
            }
        }
            
    }
    
private:
    static const int MAX_PT = 10000;
    vector<ofPoint>mVerts;
    vector<ofFloatColor>mColors;
    ofVbo mVbo;
    
    vector<ofPoint>mPointVerts;
    vector<ofFloatColor>mPointColors;
    ofVbo mPointVbo;
    int mCurrentPt = 0;
    
    vector<dpBugPt>mPts;
    
    ramOscReceiveTag mReceiver;
    
    int mLimit = 300;
};

#endif
