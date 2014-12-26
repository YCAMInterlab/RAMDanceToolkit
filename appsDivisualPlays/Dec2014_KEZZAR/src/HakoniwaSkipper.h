//
//  HakoniwaSkipper.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/21.
//
//

#ifndef RAMDanceToolkit_HakoniwaSkipper_h
#define RAMDanceToolkit_HakoniwaSkipper_h

#include "SkipperWakeUpBeam.h"
#include "ofxDelaunay.h"

#define STRINGIFY(A) #A

class HakoniwaSkipper : public ramBaseScene{
public:
    string getName() const { return "HakoniwaSkipper"; }
    
	void setupControlPanel(){
        ramGetGUI().addToggle("dir", &mDir);
        ramGetGUI().addToggle("vibeAuto", &isVibeAuto);
        ramGetGUI().addToggle("vibeWhenMoveFast", &isVibeWhenActorMoveFast);
        ramGetGUI().addSlider("strength", 0.0, 255.0, &mStrength);
        ramGetGUI().addSlider("thresh", 0.0, 30.0, &mThresh);
        ramGetGUI().addButton("vibe");
        ramGetGUI().addSlider("timeSpan", 0.01, 3.0, &mVibeTimeSpan);
        ramGetGUI().addSlider("tempo",2,8.0,&mVibeTempo);
        ramGetGUI().addSlider("dur",0.1,2.0,&mVibeDur);
        ramGetGUI().addToggle("isDrawActor", &isDrawActor);
        ramGetGUI().addToggle("isDrawFloor", &isDrawFloor);
        ramGetGUI().addSlider("mulForBuffer",1.0,500.0,&mMultForDrawBuffer);
        ramGetGUI().addSlider("lineWidth",1.0,20.0,&mLineWidth);
        
        ofxUIRadio *radio = NULL;
        
        const float dim = 16.0f;
        
        ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
        
        vector<string> names;
        
        names.push_back("JOINT_ABDOMEN");
        names.push_back("LEFT_WRIST");
        names.push_back("RIGHT_WRIST");
        names.push_back("LEFT_ANKLE");
        names.push_back("RIGHT_ANKLE");
        
        radio = new ofxUIRadio("JOINT", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
        //radio->getToggles().at(ramActor::JOINT_LEFT_HAND)->setValue(true);
        panel->addWidgetDown(radio);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HakoniwaSkipper::onPanelChanged);
    }
    void setup(){
        mSender.setup("192.168.20.53",8528);
        ofAddListener(ReachAtSkipperEvent::events, this, &HakoniwaSkipper::reachEvent);
        
        for(int i = 0; i < BEAM_NUM; i++){
            mBeams.push_back(SkipperWakeUpBeam());
        }
        
        mVibe.speed = 0.666;
        
        mPlane.setResolution(14, 14);
        
        mVert = STRINGIFY(uniform  float   val;
                          uniform  float time;
                                   
                                   float rand(vec2 co){
                                       return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453) * 2.0 - 1.0;
                                   }
                          
                          void main(){
                              vec4 newVertex = gl_Vertex;
                              newVertex.x += rand(newVertex.xy + vec2(time)) * val;
                              newVertex.y += rand(newVertex.xx + vec2(time)) * val;
                              newVertex.z += rand(newVertex.yx + vec2(time)) * val;
                              gl_Position = gl_ModelViewProjectionMatrix * newVertex;
                              gl_FrontColor = gl_Color;
                          }
                          
                                   );
        
        mFrag = STRINGIFY(
                          void main(){
                              gl_FragColor = gl_Color;
                          }
        );
        
        mDisplace.setupShaderFromSource(GL_VERTEX_SHADER, mVert);
        mDisplace.setupShaderFromSource(GL_FRAGMENT_SHADER, mFrag);
        
        mDisplace.linkProgram();
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/struggle/vector/total");
        
        mVecFromCam.speed = 0.2;
        
        for(int i = 0; i < BUFFER_MAX; i++){
            mBufferFromCam.push_back(ofPoint(0,0,0));
        }
    }
    
    void reachEvent(ReachAtSkipperEvent &e){
        vibe();
    }
    
    void vibe(){
        //mVibe.speed = mVibeDur;
        mVibe.imSet((int)mStrength);
        
        
        //mDir = !mDir;
        //mDir %= 2;//(int)ofRandom(0,2);
    }
    
    void stopVibe(){
        mVibe.set(0);
    }
    
    void getOsc(){
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/struggle/vector/total"){
               mVecFromCam.imSet(m.getArgAsFloat(0),
                               m.getArgAsFloat(1));
            }
        }
    }
    
    void update(){
        ramSetViewPort(getFirstViewPort());
        
        getOsc();
        
        ofSetLineWidth(mLineWidth);
        
        if(isVibeWhenActorMoveFast == false && isVibeAuto == false)mVibeFromActor.imSet(mStrength);
        
        vibeAuto();
        
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &array = getNodeArray(i);
            
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
                
                
                if(j == mJointNum){
                    if(node.getVelocity().length() > mThresh){
                    
                        if(isVibeWhenActorMoveFast){
                            
                            mVibeFromActor.imSet(mStrength);
                            mVibeFromActor.set(0);
                            
                        }else{
                            mVibeFromActor.imSet(0);
                            mVibe.imSet(0);
                        }
                    }
                }
            }
        }
        
        float vibeVal = mVibe.val + mVibeFromActor.val;
        
        if(vibeVal > 255.0)vibeVal = 255.0;
        
        mSmoothedVibeVal.set(vibeVal);
        
        //if(vibeVal < 0.0)vibeVal = 0.0;
                
       // if(vibeVal > 1.0){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/skipper");
        if(mDir == false){
            m.addIntArg(0);
            m.addIntArg(1);
        }else{
            m.addIntArg(1);
            m.addIntArg(0);
        }
            
        m.addIntArg((int)vibeVal);
        
        mSender.sendMessage(m);
        
        //}
        
        for(auto &v:mBeams){
            v.update();
        }
        
        
        mVibe.update();
        
        mVibeFromActor.update();
        
        mVecFromCam.update();
        
        mBufferFromCam[mBufferCounter].set(mVecFromCam);
        mBufferCounter++;
        mBufferCounter %= BUFFER_MAX;
        
        mSmoothedVibeVal.update();
        
      //  mVecFromCam.set(ofGetMouseX(),ofGetMouseY());
    }
    
    void vibeAuto(){
        if(isVibeAuto){
            
            if(ofGetElapsedTimef() - mElapsedTime > mVibeTimeSpan){
                mVibeCounter++;
                
                mElapsedTime = ofGetElapsedTimef();
                
                if(mVibeCounter == (int)mVibeTempo)mVibeCounter = 0;
                else vibe();
            }
            
        }
        
        if(ofGetElapsedTimef() - mElapsedTimeFromVibe > mVibeDur){
            mElapsedTimeFromVibe = ofGetElapsedTimef();
            mVibe.set(0);
        }
    }
    
    void draw(){
        
        float vibeVal = mSmoothedVibeVal.val;
        //ofRect(SINGLE_SCREEN_WIDTH * 0.5 + rnd.x,SINGLE_SCREEN_HEIGHT * 0.5 + rnd.y,100,100);
        
        /*ramBeginCamera();
       
        for(auto &v:mBeams){
            v.draw();
        }

        ramEndCamera();*/
        /*
        for(auto &v:mBeams){
            v.draw();
        }
        
        ofPoint tmp(ofRandom(-1.0,1.0),
                    0.0,
                    ofRandom(-1.0,1.0));
        tmp *= mVibe.val * 0.2;
        ofDrawBox(getSecondScreenCenter() + tmp,40.0);
        ofLine(getSecondScreenCenter() + tmp,getSecondScreenCenter() + ofPoint(0,-1000,0));*/
        
        ramBeginCamera();
        
        if(isDrawFloor){
            ofPushMatrix();
       
            ofRotateX(90);
            ofScale(3.0,6.0);
            mDisplace.begin();
            mDisplace.setUniform1f("time", ofGetElapsedTimef());
            mDisplace.setUniform1f("val", vibeVal * 0.015);
            mPlane.drawWireframe();
            mDisplace.end();
            ofPopMatrix();
        }
        
        ramEndCamera();
        
        float mult = mMultForDrawBuffer;
        
        ofPolyline poly;
        
        for(auto &v:mBufferFromCam){
            poly.curveTo(v * mult);
        }
        
        poly.setClosed(true);
        
        ofPushMatrix();
    
        ofTranslate(SINGLE_SCREEN_WIDTH + SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5);
        ofSetColor(255,255,255);
       // poly.draw();
        
        ofLine(0,0,mVecFromCam.x * mult,mVecFromCam.y * mult);
        ofPushStyle();
        ofSetColor(255,0,0);
        ofCircle(mVecFromCam * mult,10);
        ofPopStyle();
        drawDelaunay();
        
        ofPopMatrix();
        
        /*for(int i = 1; i < mBufferFromCam.size(); i++){
        
            ofPushMatrix();
            ofTranslate(SINGLE_SCREEN_WIDTH + SINGLE_SCREEN_WIDTH * 0.5, SINGLE_SCREEN_HEIGHT * 0.5);
            ofSetColor(255,255,255);
            ofLine(mBufferFromCam[i] * mult,mBufferFromCam[i-1] * mult);
            ofCircle(mBufferFromCam[i] * mult,2);
            ofPopMatrix();
        }*/
      
    }
    
    
    
    void drawActor(const ramActor& actor){
        
        float vibeVal = mSmoothedVibeVal.val * 0.5;
        
        if(isDrawActor){
        
            for(int j = 0; j < actor.getNumNode(); j++){
                ramNode node = actor.getNode(j);
             
                float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
                
                node.setGlobalPosition(node.getGlobalPosition() + ofPoint(ofSignedNoise(node.getGlobalPosition().x,
                                                                                  node.getGlobalPosition().y,
                                                                                  node.getGlobalPosition().z),
                                                                          ofSignedNoise(node.getGlobalPosition().z,
                                                                                        node.getGlobalPosition().x,
                                                                                        node.getGlobalPosition().y),
                                                                          ofSignedNoise(node.getGlobalPosition().z,
                                                                                        node.getGlobalPosition().y,
                                                                                        node.getGlobalPosition().x)) * vibeVal);
                
                ofDrawBox(node.getGlobalPosition(), jointSize + ofSignedNoise(node.getGlobalPosition().x,
                                                                              node.getGlobalPosition().y,
                                                                              node.getGlobalPosition().z) * vibeVal * 0.333);
                ramLine(node);
                
            }
        }
    }
    void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
        
        if(name == "vibe"){
            vibe();
        }
        
        ofxUIToggle *t = static_cast<ofxUIToggle *>(e.widget);
        
        string togName = e.widget->getName();
        
        if(togName == "JOINT_ABDOMEN")mJointNum = ramActor::JOINT_ABDOMEN;
        if(togName == "LEFT_WRIST")mJointNum = ramActor::JOINT_LEFT_WRIST;
        if(togName == "RIGHT_WRIST")mJointNum = ramActor::JOINT_RIGHT_WRIST;
        if(togName == "LEFT_ANKLE")mJointNum = ramActor::JOINT_LEFT_ANKLE;
        if(togName == "RIGHT_ANKLE")mJointNum = ramActor::JOINT_RIGHT_ANKLE;
	}
    
    void drawDelaunay(){
        mDelaunay.reset();
        
        vector<ofPoint>tmp,tmp2;
        
        for(auto &v:mBufferFromCam){
            tmp.push_back(v * mMultForDrawBuffer);
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

           /* ofDisableDepthTest();
            ofPushStyle();
            ofNoFill();
            ofMesh &mesh = mDelaunay.triangleMesh;
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            for(int i = 0; i < mesh.getIndices().size(); i+=3){
                
                int idx1 = mesh.getIndices()[i];
                int idx2 = mesh.getIndices()[i+1];
                int idx3 = mesh.getIndices()[i+2];
                
                ofSetColor(255,255,255, ((int)mesh.getVertices()[idx1].x % 10 + 1) * 25);
                ofSetLineWidth((int)mesh.getVertices()[idx1].x % 10 + 1);
                ofTriangle(mesh.getVertices()[idx1].x, mesh.getVertices()[idx1].y,
                       mesh.getVertices()[idx2].x, mesh.getVertices()[idx2].y,
                           mesh.getVertices()[idx3].x, mesh.getVertices()[idx3].y);
            }*/
            
            
            ofPushStyle();
            ofNoFill();
            mDelaunay.draw();
            ofPopStyle();
        }
    }
    
private:
    float mStrength = 200.0;
    bool mDir = 0;
    ofxOscSender mSender;
    float mThresh = 100.0;
    static const int BEAM_NUM = 30;
    
    vector<SkipperWakeUpBeam>mBeams;
    int mBeamCounter = 0;
    Slide mVibe;
    Slide mVibeFromActor;
    
    float mVibeTimeSpan = 0.2;
    float mElapsedTime = 0.0;
    int mVibeCounter = 0;
    float mVibeTempo = 4;
    
    vector<string>mVibeTempoStr;
    
    float mVibeDur = 0.1;
    float mElapsedTimeFromVibe;
    bool isVibeAuto = false;
    bool isVibeWhenActorMoveFast = false;
    
    ofPlanePrimitive mPlane;
    
    string mFrag;
    string mVert;
    ofShader mDisplace;
    
    int mJointNum = ramActor::JOINT_ABDOMEN;
    
    bool isDrawActor = true;
    bool isDrawFloor = false;
    
    ramOscReceiveTag mReceiver;
    
    SlidePoint mVecFromCam;
    vector<ofVec2f>mBufferFromCam;
    static const int BUFFER_MAX = 60;
    int mBufferCounter = 0;
    
    Slide mSmoothedVibeVal;
    ofxDelaunay mDelaunay;
    
    float mMultForDrawBuffer = 30.0;
    float mLineWidth = 4.0;
};

#endif
