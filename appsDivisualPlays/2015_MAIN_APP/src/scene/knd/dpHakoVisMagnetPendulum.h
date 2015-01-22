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
        
        mPos.imSet(mOrigin);
    }
    
    void record(ofPoint pt){
   
        mPos.set(pt + mOrigin);
        mVecs.push_back(pt + mOrigin);
        isRecordOnce = true;
    
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
        
        if(mVecs.empty() == false && isRecord == false && isRecordOnce){
      
            mPos.set(mVecs[mPlaybackCounter]);
            mPlaybackCounter++;
            mPlaybackCounter %= mVecs.size();
            
        }
        
        mPos.update();
    }
    
    void draw(){
       
        ofPushStyle();
        if(isRecord)ofSetColor(dpColor::MAIN_COLOR);
        else ofSetColor(255,255,255);

      //  ofSetColor(255,255,255);
        ofFill();
        ofCircle(mPos.x,mPos.y,mRad);
        ofPopStyle();
      //  cout << mPos.x << endl;
    }
    
    ofPoint getPos(){
        return mPos;
    }
    
private:

    vector<ofPoint>mVecs;
    
    ofPoint mOrigin;
    KezSlidePoint mPos;
    
    int mPlaybackCounter = 0;
    int mRecordCounter = 0;
    
    float mRad = 40.0;
    
    bool isRecord = false;
    bool isRecordOnce = false;
};

class dpHakoVisMagnetPendulum : public ramBaseScene{
public:
    string getName() const{return "dpVisMagPendulum";}
    
    void setupControlPanel(){
        ramGetGUI().addSlider("scale",50,1000,&mScale);
    }
    void setup(){
        
        float offsetX = 530;
        float space = 250;
        
        for(int j = 0; j < mDivY; j++){
            for(int i = 0; i < mDivX; i++){
                mCircles.push_back(dpRecordGridCircle());
                mCircles.back().setup(ofPoint((i - 1) * 500 + SINGLE_SCREEN_WIDTH * 0.5,
                                              j * space + SINGLE_SCREEN_HEIGHT * 0.5 - space * 0.5));
            }
        }
        
        mCircles[0].startRecord();
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/MagPendulum/contour/boundingRect");

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
            
            if(m.getAddress() == "/dp/cameraUnit/MagPendulum/contour/boundingRect"){
                
                if(m.getArgAsInt32(0) != 0){
                mCurrentVec.set(ofPoint((m.getArgAsFloat(2) + m.getArgAsFloat(4) * 0.5 - 0.5) * mScale,
                                        (m.getArgAsFloat(3) + m.getArgAsFloat(5) * 0.5 - 0.5) * mScale));
                }
            }
        }
    }
    
    void update(){
        
        receiveOsc();
        record(mCurrentVec);
        if(ofGetFrameNum() % dpRecordGridCircle::PT_MAX == 0)changeRecordTarget();
        
     //   record(ofPoint(ofGetMouseX(),ofGetMouseY()));
        
        for(auto &v:mCircles){
            v.update();
        }
        
    }
    
    void draw(){
    
        ofDisableDepthTest();
        
        ofSetCircleResolution(60);
        ofSetLineWidth(4);
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
        ofCircle(mCircles[mRecordTargetNum].getPos(),100);
        ofPopStyle();

       for(auto &v:mCircles){
           v.draw();
        }
        
    }
    
private:
 
    vector<dpRecordGridCircle>mCircles;
    int mDivX = 3;
    int mDivY = 2;
    int mRecordTargetNum = 0;
    
    ramOscReceiveTag mReceiver;
    
    float mScale = 500.0;
    
    ofPoint mCurrentVec;
};

#endif
