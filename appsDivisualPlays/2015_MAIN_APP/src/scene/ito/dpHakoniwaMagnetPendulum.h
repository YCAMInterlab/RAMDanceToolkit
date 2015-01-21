//
//  dpHakoniwaMagnetPendulum.h
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#ifndef __example_ramMotionExtractor__dpHakoniwaMagnetPendulum__
#define __example_ramMotionExtractor__dpHakoniwaMagnetPendulum__

#define NMAGNETS 6

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "dpConstants.h"
#include "twistFinder.h"

class dpHakoniwaMagnetPendulum : public ramBaseScene {
public:
    
    string getName() const {return "dpHMagPendulum";};
    
    void setupControlPanel();
    void setup();
    void sendOsc();
    void update();
    void draw();
    void drawActor(const ramActor& actor);
    void example_drawDump();
    void guiEvent(ofxUIEventArgs &e);
        
    virtual void onEnabled();
    virtual void onDisabled();
    
private:
    
    float distanceThreshold;
    bool bOn[NMAGNETS];
    bool bInversed[NMAGNETS];
    bool bTestMode = true;
    bool bEachMode = false;
    bool bModeTwist = false;
    bool bHideNodeView;
    
    float startTime;
    bool bFirstInverseTimeDone;
    
    ramMotionExtractor	mMotionExtractor;
    ofxOscSender mSenderOnOff;
    ofxOscSender mSenderInverse;
    
    float d1, d2, d3;
    
    twistFinder twFinder;
    
};


#endif /* defined(__example_ramMotionExtractor__dpHakoniwaMagnetPendulum__) */
