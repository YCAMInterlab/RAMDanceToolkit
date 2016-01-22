//
//  HakoniwaOscTest.h
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#pragma once

#include "ramMain.h"

class HakoniwaOscTest : public ramBaseScene {
public:
    
	HakoniwaOscTest();
    virtual ~HakoniwaOscTest();
    
    void update();
	void draw();
    
    void setupControlPanel();
    void onPanelChanged(ofxUIEventArgs& e);
    
	inline
    string getName() const { return "HakoniwaOscTest"; }
    
private:
    ofxOscSender mOscSender;
    // HakoniwaOscTest
    bool mLed;
    // HakoniwaColorOfWater
    bool m6, m7, m8, m9;
};
