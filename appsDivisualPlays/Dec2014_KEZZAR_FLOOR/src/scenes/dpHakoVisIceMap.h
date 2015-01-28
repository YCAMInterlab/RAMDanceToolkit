//
//  dpHakoVisIceMap.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#ifndef RAMDanceToolkit_dpHakoVisIceMap_h
#define RAMDanceToolkit_dpHakoVisIceMap_h

#include "dpIceMapGrid.h"
#include "dpConstants.h"
#include "ramFloorQuadWarper.h"

class dpHakoVisIceMap : public ramBaseScene{
public:
    string getName() const { return "dpVisIce"; }
    void setupControlPanel(){
        
        ramGetGUI().addIntSlider("div", 1, 20, &mDiv);
        ramGetGUI().addToggle("all", &mTogAllDraw);
        ramGetGUI().addIntSlider("lineWidth", 1, 10, &mLineWidth);
        ramGetGUI().addIntSlider("thresh", 1, 255, &mExtendThreshNum);
        ramGetGUI().addButton("randomize");
        ramGetGUI().addToggle("mouse",&isMouseAssign);
        ramGetGUI().addIntSlider("ForceExtendNum", 0, 255, &mForceExtendNum);
        ramGetGUI().addToggle("osc",&isOsc);
        
        ramFloorQuadWarper::instance().setupContolPanel(this);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Ice/mean");
        mReceiver.addAddress("/dp/cameraUnit/sceneState/");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisIceMap::onPanelChanged);
        ofAddListener(ofEvents().keyPressed, this, &dpHakoVisIceMap::keyPressed);
        mGrid.setExtendThreshNum(mExtendThreshNum);
        
        mSceneNames[ICE] = "Ice";
        mSceneNames[TORNADO] = "Tornado";
        mSceneNames[FIVE] = "dpVisFive";
        
        five();
        
    }
    
    void keyPressed(ofKeyEventArgs &e){
        if(e.key == 'e'){
            mTogAllDraw = !mTogAllDraw;
            mGrid.setAllDraw(mTogAllDraw);
            if(mTogAllDraw)isMouseAssign = false;
        }
        
        if(e.key == 'r'){
            mDiv = 5;
            mGrid.changeDiv(mDiv);
        }
        
        if(e.key == 't'){
            mDiv = 13;
            mGrid.changeDiv(mDiv);
            mExtendThreshNum = 80;
            mGrid.setExtendThreshNum(mExtendThreshNum);
        }
        
        if(e.key == 'y'){
            isOsc = !isOsc;
        }
        
        if(e.key == 'w'){
            mTogAllDraw = false;
            mGrid.setAllDraw(mTogAllDraw);
            isMouseAssign = false;
            isOsc = false;
        }
        
    }
    
    void setup(){
        mGrid.setup(ofPoint(ramFloorQuadWarper::FBO_WIDTH * 0.5,
                            ramFloorQuadWarper::FBO_HEIGHT * 0.5,0),ramFloorQuadWarper::FLOOR_WIDTH);
        ramFloorQuadWarper::instance().setup();
        mGrid.changeDiv(mDiv);
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/Ice/mean"){
                if(isOsc)mGrid.extendByThresh(m.getArgAsInt32(3));
            }
            
            if (m.getAddress().substr(0,26) == "/dp/cameraUnit/sceneState/"){
                
                string sceneName = m.getAddress().substr(26);
        
                for (int i = 0;i < mSceneNames.size();i++){
                    if (sceneName == mSceneNames[i]){
                        
                        mSceneEnable[i] = m.getArgAsInt32(0);
                      
                    }
                }
                
                if(mSceneEnable[ICE] == true && mPreMode != ICE){
                    mPreMode = ICE;
                    
                    mDiv = 5;
                    mGrid.changeDiv(mDiv);
                    
                    mTogAllDraw = false;
                    mGrid.setAllDraw(mTogAllDraw);
                }
                
                if(mSceneEnable[ICE] == false && mPreMode == ICE){
                    mPreMode = FIVE;
                    five();
                }
                
                if(mSceneEnable[TORNADO] == true){
                    mPreMode = FIVE;
                    mTogAllDraw = false;
                    isMouseAssign = false;
                    mGrid.setAllDraw(mTogAllDraw);
                }
            }
        }
        
    }
    
    void five(){
       
        mDiv = 5;
        mGrid.changeDiv(5);
        
        mTogAllDraw = true;
        mGrid.setAllDraw(mTogAllDraw);
        
    }
    
    void update(){
        receiveOsc();
        ramSetViewPort(dpGetFirstScreenViewPort());
        mGrid.update();
        
        if(isMouseAssign)mGrid.extendByThresh(ofMap(ofGetMouseX(),0,SINGLE_SCREEN_WIDTH,0,255));
    }
    void draw(){
        
        ofSetLineWidth(mLineWidth);
        
        ramFloorQuadWarper::instance().begin();
        
        mGrid.draw();
  
        ramFloorQuadWarper::instance().end();
        
        ramFloorQuadWarper::instance().draw();
        
    //    ramEndCamera();
    }
    
    void onPanelChanged(ofxUIEventArgs &e){
        string name = e.widget->getName();
     
        if(name == "div"){
            mGrid.changeDiv(mDiv);
        }
        
        if(name == "all"){
            mGrid.setAllDraw(mTogAllDraw);
        }
        
        if(name == "thresh"){
            mGrid.setExtendThreshNum(mExtendThreshNum);
        }
        
        if(name == "ForceExtendNum"){
            mGrid.extendByThresh(mForceExtendNum);
        }
        
        if(name == "randomize"){
            mGrid.randomizeMode();
        }
        
        
    }
    
    
private:
    dpIceMapGrid mGrid;
    int mDiv = 4;
    bool mTogAllDraw = false;
    
    ramOscReceiveTag mReceiver;
    
    int mLineWidth = 2;
    
    int mExtendThreshNum = 160;
    
    map<int,string>mSceneNames;
    
    enum SCENE_NAME{
        ICE,
        TORNADO,
        FIVE,
    };
    
    bool isIce = false;
    bool isMouseAssign = false;
    int mPreMode = FIVE;
    bitset<3> mSceneEnable;
    
    int mForceExtendNum = 8;
    
    bool isOsc = false;
    
};

#endif
