//
//  WeiredSpace.h
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#pragma once

#include "ramMain.h"

class WeiredSpace : public ramBaseScene {
public:
    
	WeiredSpace();
    virtual ~WeiredSpace();
    
	void setupControlPanel();
	
    void update();
	void draw();
	
	void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
	inline
    string getName() const { return "WeiredSpace"; }
    
private:
    struct Rect {
        ofVec3f center;
        ofVec3f movement;
        float size;
        ofColor color;

        ofVec3f spd;
        ofVec3f displacement;
    };
    
    void drawChecker();
    
    vector<Rect> checker;
    
    bool displaceZ;
    bool displaceXY;
    
    
};
