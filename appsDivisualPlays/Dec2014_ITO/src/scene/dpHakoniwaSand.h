#pragma once

#include "ramMotionExtractor.h"
#include "dpConstants.h"

#define STRINGIFY(A) #A

class dpHakoniwaSand : public ramBaseScene {
public:
    
    string getName() const {return "dpHSand";};
    
    void setupControlPanel();
    void setup();
    void sendOsc();
    void update();
    void draw();
    void drawActor(const ramActor& actor);
    
private:
    
    ofPlanePrimitive mPlane;
    string mFrag;
    string mVert;
    ofShader mDisplace;
    
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    bool bTestMode = true;
    
    ramMotionExtractor	mMotionExtractor;
	ofxOscSender mSender;
    
    float mVelocitySpeedScale = 10.0;
};

