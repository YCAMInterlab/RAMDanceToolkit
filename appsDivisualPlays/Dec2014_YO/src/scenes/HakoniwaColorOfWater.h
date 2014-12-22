//
//  HakoniwaColorOfWater.h
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#pragma once

#include "ramMain.h"

class HakoniwaColorOfWater : public ramBaseScene {
public:
    
	HakoniwaColorOfWater();
    virtual ~HakoniwaColorOfWater();
    
	void setupControlPanel();
	
    void update();
	void draw();
    void exit();
	
	void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
	inline
    string getName() const { return "HakoniwaColorOfWater"; }
    
private:
    ofxOscSender mOscSender;
    ramActor mActor;
    static const int kNumValves = 3;
    bool mEnableOsc;
    
    int mNode0a;
    int mNode0b;
    int mNode1a;
    int mNode1b;
    int mNode2a;
    int mNode2b;
    float mBlinkOpen;
    float mBlinkClose;
    
    class Valve {
    public:
        Valve();
        void update(const ramNode& n0, const ramNode& n1);
        void draw(int color, float x, float y);
        
        ofxOscSender* sender;
        int pin;
        bool on;
        float time;
        bool state;
        bool pState;
        float prevTime;
        float distance;
        float threshould;
        float blinkOpen, blinkClose;
        float openingDuration;
        int nOpen;
        bool enableOsc;
        
        ramNode nodeA;
        ramNode nodeB;
        
    } mValves[kNumValves];
    
};
