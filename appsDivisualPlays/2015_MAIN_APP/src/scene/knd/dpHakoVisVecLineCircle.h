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
        swapPts();
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
    static const int VECS_MAX = 256;
    vector<ofPoint>mVecs;
    float mRad;
    
    KezSlidePoint mHead;
};

class dpHakoVisVecLineCircle : public ramBaseScene{
public:
    string getName() const {return "dpVisSandStorm";};
    void setupControlPanel(){
        ramGetGUI().addSlider("scale",1.0,100.0,&mVecScale);
    }
    void setup(){
        
        for(int i = 0; i < CIRCLE_NUM; i++){
            mCircles.push_back(dpVecLineCircle());
            mCircles.back().setup(ofMap(i + 1,0,CIRCLE_NUM - 1,0,SINGLE_SCREEN_HEIGHT));
        }
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/SandStorm/vector");
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

    }
    void draw(){
        glPointSize(8);
        glEnable(GL_POINT_SMOOTH);
        ramSetViewPort(dpGetFirstScreenViewPort());
        ramBeginCamera();
        for(auto &v:mCircles){
            v.draw();
        }
        ramEndCamera();
    }
private:
    ramOscReceiveTag mReceiver;
    static const int CIRCLE_NUM = 5;
    vector<dpVecLineCircle>mCircles;
    float mVecScale = 1.0;
};

#endif
