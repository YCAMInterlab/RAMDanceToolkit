//
//  doHakoVisRecordGrid.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_doHakoVisRecordGrid_h
#define RAMDanceToolkit_doHakoVisRecordGrid_h

class dpRecordGridCircle{
public:
    
    static const int PT_MAX = 500;
    
    void setup(ofPoint origin){
        mOrigin = origin;
        
        for(int i = 0; i < PT_MAX; i++){
            mVecs.push_back(ofPoint(0,0,0));
        }
    }
    
    void record(ofPoint pt){
   
        mPos = pt + mOrigin;
        mVecs.push_back(pt);
    
    }
    
    void startRecord(){
        
        mVecs.clear();
        isRecord = true;
        
    }
    
    void startPlayback(){
        isRecord = false;
        mPlaybackCounter = 0;

    }
    
    void swapPts(ofPoint pt){
        mVecs[0] = pt;
        for(int i = 1; i < PT_MAX; i++){
            ofPoint tmp = mVecs[i-1];
            mVecs[i - 1] = mVecs[i];
            mVecs[i] = tmp;
        }
    }
    
    void update(){
        
        if(mVecs.empty() == false && isRecord == false){
            mPos = mOrigin;
            mPos += mVecs[mPlaybackCounter];
            mPlaybackCounter++;
            mPlaybackCounter %= mVecs.size();
        }
    }
    
    void draw(){
       
        ofPushStyle();
        if(isRecord)ofSetColor(dpColor::MAIN_COLOR);
        else ofSetColor(255,255,255);

        ofCircle(mPos,mRad);
        ofPopStyle();
        
    }
    
    ofPoint getPos(){
        return mPos;
    }
    
private:

    vector<ofPoint>mVecs;
    
    ofPoint mOrigin;
    ofPoint mPos;
    
    int mPlaybackCounter = 0;
    int mRecordCounter = 0;
    
    float mRad = 20.0;
    
    bool isRecord = false;
};

class dpHakoVisMagnetPendulum : public ramBaseScene{
public:
    string getName() const{return "dpVisMagnetPendulum";}
    void setupControlePanel(){}
    void setup(){
        
        float offsetX = 300;
        float offsetY = 400;
        
        for(int j = 0; j < mDivY; j++){
            for(int i = 0; i < mDivX; i++){
                mCircles.push_back(dpRecordGridCircle());
                mCircles.back().setup(ofPoint(ofMap(i,0,mDivX,offsetX,SINGLE_SCREEN_WIDTH - offsetX),
                                              ofMap(j,0,mDivY,offsetY,SINGLE_SCREEN_HEIGHT - offsetY)));
            }
        }
        
        mCircles[0].startRecord();
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/MagnetPendulum/contour/boundingRect");

    }
    
    void record(ofPoint pt){
        mCircles[mRecordTargetNum].record(pt);
    }
    
    void changeRecordTarget(){
        mCircles[mRecordTargetNum].startPlayback();
        mRecordTargetNum++;
        mRecordTargetNum %= mCircles.size();
        
        mCircles[mRecordTargetNum].startRecord();
    }
    
    void receiveOsc(){
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/MagnetPendulum/contour/boundingRect"){
                record(ofPoint(m.getArgAsFloat(2) * 100.0,
                       m.getArgAsFloat(3) * 100.0));
            }
        }
    }
    
    void update(){
        
        receiveOsc();
    
        if(ofGetFrameNum() % dpRecordGridCircle::PT_MAX == 0)changeRecordTarget();
        
     //   record(ofPoint(ofGetMouseX(),ofGetMouseY()));
        
        for(auto &v:mCircles){
            v.update();
        }
        
    }
    
    void draw(){
        for(auto &v:mCircles){
            v.draw();
        }
        
        for(int i = 1; i < mDivX; i++){
             //ofLine(mCircles[i - 1].getPos(),mCircles[i].getPos());
            for(int j = 0; j < mDivY; j++){
               ofLine(mCircles[j * mDivX + i - 1].getPos(),mCircles[j * mDivX + i].getPos());
            }
        }
        
        for(int i = 0; i < mDivX; i++){
            ofLine(mCircles[i].getPos(),mCircles[i + mDivX].getPos());
        }
        
        ofPushStyle();
        ofSetColor(dpColor::MAIN_COLOR);
        ofNoFill();
        ofCircle(mCircles[mRecordTargetNum].getPos(),50);
        ofPopStyle();
    }
    
private:
    static const int LINE_NUM = 10;
    vector<dpRecordGridCircle>mCircles;
    int mDivX = 3;
    int mDivY = 2;
    int mRecordTargetNum = 0;
    
    ramOscReceiveTag mReceiver;
};

#endif
