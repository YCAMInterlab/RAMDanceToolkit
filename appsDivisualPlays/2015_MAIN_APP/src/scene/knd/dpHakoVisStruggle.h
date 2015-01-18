//
//  HakoVisStruggle.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/21.
//
//

#ifndef RAMDanceToolkit_HakoVisStruggle_h
#define RAMDanceToolkit_HakoVisStruggle_h

#include "KezDelaunayMesh.h"
#include "KezSlidePoint.h"

#define STRINGIFY(A) #A

class dpHakoVisStruggle : public ramBaseScene{
public:
    string getName() const { return "dpVisStruggle"; }
    
	void setupControlPanel(){

        ramGetGUI().addSlider("scaleForDrawBuffer",0.1,500.0,&mScaleDrawDelaunay);
        ramGetGUI().addSlider("lineWidth",1.0,20.0,&mLineWidth);
        
        ramGetGUI().addSlider("alphaReducSpeed", 0.01, 5.0, &mAlphaReducSpeed);
        ramGetGUI().addToggle("isShowFaces", &isShowFaces);
        ramGetGUI().addToggle("isRotAfterFinish", &isRotAfterFinish);
        ramGetGUI().addSlider("thresh",0.001,0.5, &mThresh);
        
        ramGetGUI().addIntSlider("omit", 1, 20, &mOmitNum);
        
        ramGetGUI().addSlider("rot:x",0,360,&mRot.x);
        ramGetGUI().addSlider("rot:y",0,360,&mRot.y);
        ramGetGUI().addSlider("rot:z",0,360,&mRot.z);
                
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisStruggle::onPanelChanged);
    }
    void setup(){
 
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Struggle/vector/total");
        
        mVecFromCam.speed = 0.2;
        
        mDelaunayMesh.setup();
    }
  

    void receiveOsc(){
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/Struggle/vector/total"){
              //  cout << m.getArgAsFloat(0) << endl;
               mVecFromCam.set( m.getArgAsFloat(0), m.getArgAsFloat(1));
            }
        }
    }
    
    void update(){
//        ramSetViewPort(dpGetFirstScreenViewPort());
        
        receiveOsc();
        
        ofSetLineWidth(mLineWidth);
        
     //   mVecFromCam.set(ofRandom(-300,300),ofRandom(-300,300));
        
        mVecFromCam.update();
        mDelaunayMesh.addPoint(mVecFromCam,mScaleDrawDelaunay);
        mDelaunayMesh.update();
        
    }
    
    void draw(){

        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofPushMatrix();
        ofTranslate(SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5);
        ofRotateX(mRot.x);
        ofRotateY(mRot.y);
        ofRotateZ(mRot.z);
        drawDelaunay();
        ofPopMatrix();

    }
    
    
    
    void drawActor(const ramActor& actor){
        
    }
    void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
        

        if(name == "isRotAfterFinish"){
            mDelaunayMesh.rotAfterFinish(isRotAfterFinish);
        }
        
        if(name == "isShowFaces"){
            mDelaunayMesh.showFaces(isShowFaces);
        }

        if(name == "alphaReducSpeed"){
            mDelaunayMesh.setAlphaReducSpeed(mAlphaReducSpeed);
        }
        
        if(name == "omit"){
            mDelaunayMesh.setOmitNum(mOmitNum);
        }
        
        if(name == "thresh"){
            mDelaunayMesh.setThresh(mThresh);
        }
        
     
	}
    
    void drawDelaunay(){
        mDelaunayMesh.draw();
    }
    
private:
  
    ramOscReceiveTag mReceiver;
    
    KezSlidePoint mVecFromCam;
    
    float mScaleDrawDelaunay = 400.0;
    float mLineWidth = 4.0;
    
    ofPlanePrimitive mPlane;
    
    KezDelaunayMeshController mDelaunayMesh;
    
    bool isShowFaces = true;
    bool isRotAfterFinish = true;
    float mAlphaReducSpeed = 0.5;
    
    int mOmitNum = 6;
    float mThresh = 0.02;
    
    ofPoint mRot;
};

#endif
