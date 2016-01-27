//
//  dpHakoVisRecordRibbon.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_dpHakoVisRecordRibbon_h
#define RAMDanceToolkit_dpHakoVisRecordRibbon_h

class dpSPBox{
public:
    
    dpSPBox(){
        mColor = dpColor::MAIN_COLOR;
    }
    
    void start(ofPoint size,float angle){
        mSize = size;
        mAngle = angle;
        mSat.imSet(255);
        mSat.set(0);
        mSat.speed = 0.03;
    }
    
    void update(){
        mPos.z += mSpeed;
        mSat.update();
        
        if(mSat.val > 1){
            mColor.setSaturation(mSat.val);
            mColor.setBrightness(ofMap(mSat.val,255,0,255,90));
        }
    }
    
    void draw(){
    
        ofSetColor(mColor.r,mColor.g,mColor.b,ofMap(mPos.z,230,300,255,0,true));
        
        ofPushMatrix();
        ofTranslate(0, 0, mPos.z);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawBox(0,0,0,mSize.x,mSize.y,mSize.z);
        ofPopMatrix();
    
        ofPushMatrix();
        ofTranslate(0, 0, -mPos.z);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawBox(0,0,0,mSize.x,mSize.y,mSize.z);
        ofPopMatrix();
       
    }
    
private:
    ofPoint mPos;
    ofPoint mSize;
    
    float mSpeed = 1.0;
    float mAngle = 0.0;

    ofColor mColor;
    KezSlide mSat;
};

class dpHakoVisServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpVisServoPendulum"; }
    
	void setupControlPanel(){
        
        mScale = 600;
        
        ramGetGUI().addSlider("scale", 100.0, 2000.0, &mScale);
        ramGetGUI().addButton("rndOrbit");

        mLong.speed = 0.005;
        mLat.speed = 0.005;
        mRad.speed = 0.01;
        
        rndOrbit();
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisServoPendulum::onPanelChanged);
    }
    void setup(){
    
     
        mCam.setVFlip(true);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/ServoPendulum/vector/total");
    
        mHead.speed = 0.2;

    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/ServoPendulum/vector/total"){
                mBoxes.push_back(dpSPBox());
                
                mHead.set(m.getArgAsFloat(0),
                          m.getArgAsFloat(1));
                
                float angle = atan2(mHead.y,mHead.x);
                
                mAngle = angle;
                
                mBoxes.back().start(ofPoint(1.0,mHead.length() * mScale,1.0),angle);
                if(mBoxes.size() > BOX_MAX)mBoxes.pop_front();
                
                mHead.update();
            }
        
        }
    }
    
    void update(){
        
        receiveOsc();
   
        for(auto &v:mBoxes){
            v.update();
        }
        
        mLong.update();
        mLat.update();
        mRad.update();
        
        mCam.lookAt(ofPoint(0,0,0));
        mCam.orbit(mLong.val, mLat.val, mRad.val);
        
        if(ofGetFrameNum() % RND_ROT_FRAME == 0)rndOrbit();
    }
    
    void rndOrbit(){
        mLong.set(ofRandom(-180,180));
        mLat.set(ofRandom(-180,180));
        mRad.set(ofRandom(50,300));
    }
    
    void draw(){

        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetLineWidth(2);
   
        mCam.begin(dpGetFirstScreenViewPort());
        
        ofSetColor(dpColor::MAIN_COLOR.r,
                   dpColor::MAIN_COLOR.g,
                   dpColor::MAIN_COLOR.b,
                   255);
        
        ofPushMatrix();
        ofTranslate(0, 0, 0.0);
        ofRotateZ(ofRadToDeg(mAngle));
        ofDrawBox(0,0,0,1.0,mHead.length() * mScale,1.0);
        ofPopMatrix();
        
        ofNoFill();
        
        for(auto &v:mBoxes){
            v.draw();
        }
        
        mCam.end();
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
        string name = e.widget->getName();
        
        if(name == "rndOrbit")rndOrbit();
        
    }
    
private:
    
    ofEasyCam mCam;

    ramOscReceiveTag mReceiver;
    
    float mScale;
    
    static const int BOX_MAX = 300;
    deque<ofPoint>mPts;
    deque<dpSPBox>mBoxes;
 
    KezSlidePoint mHead;
    
    float mAngle = 0.0;
    
    KezSlide mLong;
    KezSlide mLat;
    KezSlide mRad;
    
    static const int RND_ROT_FRAME = 1200;
};

#endif
