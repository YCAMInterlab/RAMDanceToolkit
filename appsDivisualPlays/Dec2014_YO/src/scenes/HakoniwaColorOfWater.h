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
	
	void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
	inline
    string getName() const { return "HakoniwaColorOfWater"; }
    
private:
    ofxOscSender mOscSender;
    ramActor mActor;
    static const int kNumValves = 3;
    
    int node0a;
    int node0b;
    int node1a;
    int node1b;
    int node2a;
    int node2b;
    
    class Valve {
    public:
        void update(const ramNode& n0, const ramNode& n1);
        void draw(int color, float x, float y);
        
        bool stateChanged() const { return state != pState; }
        
        bool state;
        float threshould;
        float distance;
        bool pState;
        
        ramNode nodeA;
        ramNode nodeB;
        
    } mValves[kNumValves];
    
};
