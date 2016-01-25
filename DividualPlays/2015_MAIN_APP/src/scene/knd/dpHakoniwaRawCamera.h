//
//  dpHakoniwaRawCamera.h
//  2015_MAIN_APP
//
//  Created by kezzardrix2 on 2016/01/25.
//
//

#ifndef _015_MAIN_APP_dpHakoniwaRawCamera_h
#define _015_MAIN_APP_dpHakoniwaRawCamera_h

#include "dpSyphonClientManager.h"

class dpHakoniwaRawCamera : public ramBaseScene{
public:
    string getName() const { return "dpHakoniwaRawCamera"; }
    
    void setupControlPanel(){
    
    }
    void setup(){
     
    }
    
    void receiveOsc(){
        
    
    }
    
    void update(){
      
    }
    
    
    
    void draw(){
        
        ofSetColor(255);
        dpSyphonClientManager::instance().drawWithSideCrop();
        
    }
    
    
    
    void onEnabled(){
   
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
        string name = e.widget->getName();
        
        
    }
    
private:
    
    
};

#endif
