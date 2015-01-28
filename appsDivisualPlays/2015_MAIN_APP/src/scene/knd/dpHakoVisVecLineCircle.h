//
//  dpHakoVisLineCircle.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/13.
//
//

#ifndef RAMDanceToolkit_dpHakoVisLineCircle_h
#define RAMDanceToolkit_dpHakoVisLineCircle_h

class dpVecLineCircle{
public:
    
    void setup(float rad){
        
        mRad = rad;
        
        mBuffers.clear();
        
        for(int i = 0; i < BUFFER_MAX; i++){
            mBuffers.push_back(ofPoint(0,0,0));
        }
        
        mVbo.clear();
        mLineVbo.clear();
        
        mPts.clear();
        mLines.clear();
        
        mPts.assign(BUFFER_MAX - 1, ofPoint()); //i do not use head pt
        mLines.assign((BUFFER_MAX - 1) * 2, ofPoint());
        
        mVbo.setVertexData(&mPts.at(0), mPts.size(), GL_DYNAMIC_DRAW);
        mLineVbo.setVertexData(&mLines.at(0), mLines.size(), GL_DYNAMIC_DRAW);
        
    }
    
    void setHead(ofPoint vec){
        mHead.set(vec.x,vec.y,vec.length());
    }
    
    void swapPts(){
        
        if(isForward){
            
            mBuffers[0] = mHead;
        
            for(int i = 1; i < BUFFER_MAX; i++){
                ofPoint tmp = mBuffers[i-1];
                mBuffers[i - 1] = mBuffers[i];
                mBuffers[i] = tmp;
            }
            
        }else{
        
            mBuffers[mBuffers.size() - 1] = mHead;
            
            for(int i = mBuffers.size() - 1; i > 0; i--){
                ofPoint tmp = mBuffers[i-1];
                mBuffers[i - 1] = mBuffers[i];
                mBuffers[i] = tmp;
            }
        }
    }
    
    void update(){
    
        mHead.update();
        if(isMove)swapPts();
    
        updateVbos();
        
    }
    
    void updateVbos(){
        
        for(int i = 0; i < mBuffers.size() - 1; i++){
            
            ofVec2f pt;
            alongCircle(pt, i);
            
            mPts[i].set(pt.x * mRad + mBuffers[i].x , pt.y * mRad + mBuffers[i].y, mBuffers[i].z);
            mLines[i * 2].set(pt.x * mRad, pt.y * mRad , 0.0);
            mLines[i * 2 + 1].set(pt.x * mRad + mBuffers[i].x , pt.y * mRad + mBuffers[i].y , mBuffers[i].z);
            
        }
        
        mVbo.updateVertexData(&mPts.at(0), mPts.size());
        mLineVbo.updateVertexData(&mLines.at(0), mLines.size());
    }
    
    void setMove(bool move){
        isMove = move;
    }
    
    void setForward(bool forward){
        isForward = forward;
    }
    
    void alongCircle(ofVec2f &pt,int idx){
        
        float theta = ofMap(idx,0,mBuffers.size(),0,TWO_PI) - HALF_PI;
        
        float x = cos(theta);
        float y = sin(theta);
        
        pt.set(x,y);
        
    }
    
    void draw(){
        
        ofSetColor(dpColor::MAIN_COLOR);
        mVbo.draw(GL_POINTS,0,mPts.size());
        
        ofSetLineWidth(2);
        ofSetColor(255,255,255);
        mLineVbo.draw(GL_LINES,0,mLines.size());
        
        drawHead();
    }
    
    void drawHead(){
        
        int idx = mBuffers.size() - 1;
        
        ofVec2f pt;
        alongCircle(pt, idx);
        
        ofSetColor(dpColor::MAIN_COLOR);
        ofDrawSphere(pt.x * mRad + mBuffers[idx].x , pt.y * mRad + mBuffers[idx].y, mBuffers[idx].z,20);
        
        ofSetLineWidth(4);
        ofLine(pt.x * mRad, pt.y * mRad , 0.0,
               pt.x * mRad + mBuffers[idx].x , pt.y * mRad + mBuffers[idx].y , mBuffers[idx].z);
        
    }
    
private:
    
    static const int BUFFER_MAX = 512;
    vector<ofPoint>mBuffers;
    
    float mRad;
    
    KezSlidePoint mHead;
    
    bool isMove = true;
    bool isForward = true;
    
    ofVbo mVbo;
    ofVbo mLineVbo;
    
    vector<ofPoint>mPts;
    vector<ofPoint>mLines;
    
};

class dpHakoVisVecLineCircle : public ramBaseScene{
public:
    string getName() const {return "dpVisSandStorm";};
    void setupControlPanel(){
        
        ramGetGUI().addSlider("scale",1.0,100.0,&mVecScale);
        ramGetGUI().addSlider("rot:x",0.0,1.0,&mRotSpeed.x);
        ramGetGUI().addSlider("rot:y",0.0,1.0,&mRotSpeed.y);
        ramGetGUI().addSlider("rot:z",0.0,1.0,&mRotSpeed.z);
        ramGetGUI().addToggle("zoomOut", &isZoomOut);
        ramGetGUI().addButton("rndRot");
        ramGetGUI().addButton("stop");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisVecLineCircle::onPanelChanged);
        
    }
    void setup(){
        
        for(int i = 0; i < CIRCLE_NUM; i++){
            mCircles.push_back(dpVecLineCircle());
            mCircles.back().setup(ofMap(i + 1,0,CIRCLE_NUM - 1,0,SINGLE_SCREEN_HEIGHT));
        }
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/SandStorm/vector");

        mZoom.speed = 0.001;
        mRot.speed = 0.005;
        mRotSpeed.x = -0.05;
    }
    
    void receiveOsc(){
        
        while (mReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/SandStorm/vector"){
                for(int i = 0; i < CIRCLE_NUM; i++){
                    mCircles[i].setHead(ofPoint(m.getArgAsFloat(i * 2),
                                       m.getArgAsFloat(i * 2 + 1)) * mVecScale);
                }
            }
        }
        
    }
    
    void update(){
        receiveOsc();
        
        for(auto &v:mCircles){
            v.update();
        }
        
        mRot += mRotSpeed;

        mZoom.update();
        
        mZoom.set(isZoomOut * -1000);
        
        if((ofGetFrameNum() - mBeginFrame) % RANDOMIZE_HAPPEN == RANDOMIZE_HAPPEN - 1){
            
            int rnd = ofRandom(0,4);
            
            if(rnd == 0)isZoomOut = !isZoomOut;
            else if(rnd == 1)rndStop();
            else if(rnd == 2)rndRot();
            else if(rnd == 3)rndDirection();
            
        }
        
        mRot.update();
    }
    
    void rndStop(){
        
        for(auto &v:mCircles){
            v.setMove(true);
        }
        
        if(mStopCounter < mCircles.size()){
            
            mCircles.at(mStopCounter).setMove(false);
            mStopCounter++;
            
        }else{
            
            mStopCounter %= mCircles.size();
            
        }
    }
    
    void rndDirection(){
        
        for(auto &v:mCircles){
            v.setForward(true);
        }
        
        int rnd = ofRandom(0,4);
        
        if(rnd == 0){
            mCircles[(int)ofRandom(0,mCircles.size())].setForward(false);
        }
    }
    
    void rndRot(){
        
        int rnd = ofRandom(0,5);
        
        if(rnd == 0){
            
            mRotSpeed.set(0,0,0);
            
        }else if(rnd == 1){
            
            mRotSpeed.set(0,0,0);
         
            mRot.imSet(fmodf(mRot.x,360.0),
                       fmodf(mRot.y,360.0),
                       fmodf(mRot.z,360.0));
            
            mRot.set(0,0,0);
            
        }else{
            
            mRotSpeed.set(ofRandom(-0.1,0.1),
                          ofRandom(-0.1,0.1),
                          0.0);
            
        }
    }
    
    void draw(){
        
        glPointSize(8);
        glEnable(GL_POINT_SMOOTH);
        
        mCam.begin(dpGetFirstScreenViewPort());
        
        ofPushMatrix();
        ofTranslate(0, 0, mZoom.val);
        ofRotateX(mRot.x);
        ofRotateY(mRot.y);
        ofRotateZ(mRot.z);
        for(auto &v:mCircles){
            v.draw();
        }
        ofPopMatrix();
        
        mCam.end();
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
        string name = e.widget->getName();
    
        if(name == "rndRot")rndRot();
        
        if(name == "stop")rndStop();
        
    }
    
    void onEnabled(){
        mRotSpeed.set(-0.05,0.0,0.0);
        mRot.imSet(0,0,0);
        mBeginFrame = ofGetFrameNum();
    }
    
private:
    ramOscReceiveTag mReceiver;
    
    static const int RANDOMIZE_HAPPEN = 600;
    
    static const int CIRCLE_NUM = 5;
    vector<dpVecLineCircle>mCircles;
    
    float mVecScale = 30.0;
    
    ofEasyCam mCam;
    
    KezSlidePoint mRot;
    KezSlide mZoom;
    ofPoint mRotSpeed;
    
    bool isZoomOut = false;
    
    int mStopCounter = 0;
    int mBeginFrame = 0;
};

#endif
