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
        
        ramGetGUI().addIntSlider("div", 1, 14, &mDiv);
        ramGetGUI().addToggle("all", &mTogAllDraw);
        ramGetGUI().addIntSlider("lineWidth", 1, 10, &mLineWidth);
        
        ramFloorQuadWarper::instance().setupContolPanel(this);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/Ice/pixelate");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisIceMap::onPanelChanged);
    }
    void setup(){
        mGrid.setup(ofPoint(ramFloorQuadWarper::FBO_WIDTH * 0.5,
                            ramFloorQuadWarper::FBO_HEIGHT * 0.5,0),ramFloorQuadWarper::FLOOR_WIDTH);
        ramFloorQuadWarper::instance().setup();
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/Ice/pixelate"){
               
                int width = m.getArgAsInt32(0);
                int height = m.getArgAsInt32(1);
                
                int sum = width * height;
                int argNum = ceil(sum / 64) + 1;
                
                string str;
                
                for(int i = 2; i < argNum + 2; i++){
                 
                    string tmp = ofToBinary(m.getArgAsInt64(i));
                 
                    for (int j = tmp.size()-1; j>=0; j--) {
                        str += tmp.at(j);
                    }
                    
                }
                
                for(int i = 0; i < mDiv; i++){
                    for(int j = 0; j < mDiv; j++){
                            
                        int idxX = ofMap(i,0,mDiv,0,width,true);
                        int idxY = ofMap(j,0,mDiv,0,height,true);
                        
                        int idx = idxY * mDiv + idxX;
                        
                        if(idx < str.size() && str[idx] == '1'){
                            mGrid.extendEach(i * mDiv + j);
                        }
                        else mGrid.shrink();
                    }
                }
         
            }
        }
        
    }
    
    void update(){
        receiveOsc();
        ramSetViewPort(dpGetFirstScreenViewPort());
        mGrid.update();
    }
    void draw(){
        
        ofSetLineWidth(mLineWidth);
        
        ramFloorQuadWarper::instance().begin();
        
        ofPushMatrix();
      
        mGrid.draw();
  
        ofPopMatrix();
        
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
    }
    
    
private:
    dpIceMapGrid mGrid;
    int mDiv = 1;
    bool mTogAllDraw = false;
    
    ramOscReceiveTag mReceiver;
    
    int mLineWidth = 2;
    
};

#endif
