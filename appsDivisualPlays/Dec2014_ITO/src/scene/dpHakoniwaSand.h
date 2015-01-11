#pragma once

#include "ramMotionExtractor.h"
#include "dpConstants.h"

#define STRINGIFY(A) #A

class dpHakoniwaSand : public ramBaseScene{
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
    
    float mVelocitySpeed = 0.0;
    
    ramMotionExtractor	mMotionExtractor;
	ofxOscSender mSender;
    
    float mVelocitySpeedScale = 10.0;
};

