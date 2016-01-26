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
        ramGetGUI().addIntSlider("extendThresh", 1, 255, &mExtendThresh);
        ramGetGUI().addIntSlider("extendNum", 0, 255, &mExtendNum);
        ramGetGUI().addButton("randomize");
        ramGetGUI().addToggle("externalOsc",&isExternalOsc);
        
        ramFloorQuadWarper::instance().setupContolPanel(this);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Ice/mean");
        mReceiver.addAddress("/dp/cameraUnit/sceneState/");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisIceMap::onPanelChanged);
        ofAddListener(ofEvents().keyPressed, this, &dpHakoVisIceMap::keyPressed);
        mGrid.setExtendThreshNum(mExtendThresh);
        
        mSceneNames[ICE] = "Ice";
        mSceneNames[TORNADO] = "Tornado";
        mSceneNames[FIVE] = "dpVisFive";
        
        five();
        
    }
    
    void keyPressed(ofKeyEventArgs &e){
        if(e.key == 'e'){
            mTogAllDraw = !mTogAllDraw;
            mGrid.setAllDraw(mTogAllDraw);
        }
        
        if(e.key == 'r'){
            mDiv = 5;
            mGrid.changeDiv(mDiv);
        }
        
        if(e.key == 't'){
            mDiv = 13;
            mGrid.changeDiv(mDiv);
            mExtendThresh = 80;
            mGrid.setExtendThreshNum(mExtendThresh);
        }
        
        if(e.key == 'y'){
            isExternalOsc = !isExternalOsc;
        }
        
        if(e.key == 'w'){
            mTogAllDraw = false;
            mGrid.setAllDraw(mTogAllDraw);
            isExternalOsc = false;
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
                if(isExternalOsc)mGrid.extendByThresh(m.getArgAsInt32(3));
            }
            
            if (m.getAddress().substr(0,26) == "/dp/cameraUnit/sceneState/"){
                
                string sceneName = m.getAddress().substr(26);
        
                for (int i = 0;i < mSceneNames.size();i++){
                    if (sceneName == mSceneNames[i]){
                        
                        mSceneEnable[i] = m.getArgAsInt32(0);
                      
                    }
                }
                
                // TODO: divがシーン毎に自動的に変わる場合は実装を検討
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
    }
    void draw(){
        
        ofSetLineWidth(mLineWidth);
        
        ramFloorQuadWarper::instance().begin();
        
        mGrid.draw();
  
        ramFloorQuadWarper::instance().end();
        
        ramFloorQuadWarper::instance().draw();
        
    }
    
    void onPanelChanged(ofxUIEventArgs &e){
        string name = e.widget->getName();
     
        if(name == "div"){
            mGrid.changeDiv(mDiv);
        }
        
        if(name == "all"){
            mGrid.setAllDraw(mTogAllDraw);
        }
        
        if(name == "extendThresh"){
            mGrid.setExtendThreshNum(mExtendThresh);
        }
        
        if(name == "extendNum"){
            mGrid.extendByThresh(mExtendNum);
        }
        
        if(name == "randomize"){
            mGrid.changeDiv(mDiv);
            mGrid.extendByThresh(mExtendNum);
        }
        
    }
    
    
private:
    dpIceMapGrid mGrid;
    
    ramOscReceiveTag mReceiver;
    
    int mDiv = 4;
    int mLineWidth = 2;
    int mExtendThresh = 160;
    int mPreMode = FIVE;
    int mExtendNum = 8;
    
    map<int,string>mSceneNames;
    
    enum SCENE_NAME{
        ICE,
        TORNADO,
        FIVE,
    };
    
    bool isExternalOsc = false;
    bool mTogAllDraw = false;
    
    bitset<3> mSceneEnable;
    
};

#endif
