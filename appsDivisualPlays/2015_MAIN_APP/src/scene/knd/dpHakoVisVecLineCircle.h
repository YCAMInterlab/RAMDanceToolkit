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
        
        mVecs.clear();
        for(int i = 0; i < VECS_MAX; i++){
            mVecs.push_back(ofPoint(0,0,0));
        }
    }
    
    void setVec(ofPoint vec){
        mHead.set(vec.x,vec.y,vec.length());
    }
    
    void swapPts(){
        mVecs[0] = mHead;
        
        for(int i = 1; i < VECS_MAX; i++){
            ofPoint tmp = mVecs[i-1];
            mVecs[i - 1] = mVecs[i];
            mVecs[i] = tmp;
        }
        
    }
    void update(){
        mHead.update();
        if(isMove)swapPts();
    }
    
    void setMove(bool move){
        isMove = move;
    }

    
    void draw(){
        for(int i = 0; i < mVecs.size(); i++){
            float theta = ofMap(i,0,mVecs.size(),0,TWO_PI) - HALF_PI;
            
            float x = cos(theta);
            float y = sin(theta);
            
            if(i == mVecs.size() - 1){
                
                ofSetColor(dpColor::MAIN_COLOR);
                ofDrawSphere(x * mRad + mVecs[i].x , y * mRad + mVecs[i].y, mVecs[i].z,20);
                
                ofSetLineWidth(4);
                ofLine(x * mRad, y * mRad , 0.0,
                       x * mRad + mVecs[i].x , y * mRad + mVecs[i].y , mVecs[i].z);

                
            }else {
                
                ofSetColor(dpColor::MAIN_COLOR);
                glBegin(GL_POINTS);
                glVertex3d(x * mRad + mVecs[i].x , y * mRad + mVecs[i].y, mVecs[i].z);
                glEnd();
                
                ofSetLineWidth(2);
                ofSetColor(255,255,255);
                ofLine(x * mRad, y * mRad , 0.0,
                       x * mRad + mVecs[i].x , y * mRad + mVecs[i].y , mVecs[i].z);
    
                }
            
            }
        }
private:
    static const int VECS_MAX = 512;
    vector<ofPoint>mVecs;
    float mRad;
    
    KezSlidePoint mHead;
    
    bool isMove = true;
};

class dpHakoVisVecLineCircle : public ramBaseScene{
public:
    string getName() const {return "dpVisSandStorm";};
    void setupControlPanel(){
        ramGetGUI().addSlider("scale",1.0,100.0,&mVecScale);
        ramGetGUI().addSlider("rot:x",0.0,1.0,&mRotSpeed.x);
        ramGetGUI().addSlider("rot:y",0.0,1.0,&mRotSpeed.y);
        ramGetGUI().addSlider("rot:z",0.0,1.0,&mRotSpeed.z);
        ramGetGUI().addToggle("far", &isFar);
        ramGetGUI().addButton("rndRot");
        ramGetGUI().addButton("stop");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisVecLineCircle::onPanelChanged);
        mRotSpeed.x = -0.05;
    }
    void setup(){
        
        for(int i = 0; i < CIRCLE_NUM; i++){
            mCircles.push_back(dpVecLineCircle());
            mCircles.back().setup(ofMap(i + 1,0,CIRCLE_NUM - 1,0,SINGLE_SCREEN_HEIGHT));
        }
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/SandStorm/vector");

        mZoom.speed = 0.001;
        mRot.speed = 0.01;
        
    }
    
    void receiveOsc(){
        while (mReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/SandStorm/vector"){
                for(int i = 0; i < CIRCLE_NUM; i++){
                    mCircles[i].setVec(ofPoint(m.getArgAsFloat(i * 2),
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
        
        mZoom.set(isFar * -1000);
        
        if(ofGetFrameNum() % 600 == 0){
            int rnd = ofRandom(0,3);
            
            if(rnd == 0)isFar = !isFar;
            else if(rnd == 1)checkStop();
            else if(rnd == 2)rndRot();
        }
        
        mRot.update();
    }
    
    void checkStop(){
        
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
    
    void rndRot(){
        int rnd = ofRandom(0,5);
        
        if(rnd != 0){
            mRotSpeed.set(0,0,0);
        }else if(rnd == 1){
            mRotSpeed.set(0,0,0);
            mRot.set(0,0,0);
        }
        else{
            mRotSpeed.set(ofRandom(-0.1,0.1),
                          ofRandom(-0.1,0.1),
                          ofRandom(-0.1,0.1));
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
        
        if(name == "stop")checkStop();
    }
    
private:
    ramOscReceiveTag mReceiver;
    static const int CIRCLE_NUM = 5;
    vector<dpVecLineCircle>mCircles;
    float mVecScale = 50.0;
    
    ofEasyCam mCam;
    
    KezSlidePoint mRot;
    ofPoint mRotSpeed;
    
    int mStopCounter = 0;
    KezSlide mZoom;
    bool isFar = false;
};

#endif
