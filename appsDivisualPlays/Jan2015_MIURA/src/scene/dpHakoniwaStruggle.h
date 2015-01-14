//
//  HakoniwaStruggle.h
//  example-ramMotionExtractor
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#ifndef example_ramMotionExtractor_HakoniwaStruggle_h
#define example_ramMotionExtractor_HakoniwaStruggle_h

#include "ramMotionExtractor.h"
#include "dpConstants.h"

#define STRINGIFY(A) #A

class dpHakoniwaStruggle : public ramBaseScene{
public:
    
    string getName() const {return "dpHStruggle";};
    
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

#endif
