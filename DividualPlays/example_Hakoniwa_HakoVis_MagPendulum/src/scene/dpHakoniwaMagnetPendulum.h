//
//  dpHakoniwaMagnetPendulum.h
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#ifndef __example_ramMotionExtractor__dpHakoniwaMagnetPendulum__
#define __example_ramMotionExtractor__dpHakoniwaMagnetPendulum__

#define NMAGNETS 2

#include "ramMain.h"
#include "ramMotionExtractor.h"

class dpHakoniwaMagnetPendulum : public ramBaseScene {
    
public:
    
    string getName() const {return "dpHMagPendulum";};
    
    void setupControlPanel();
    void setup();
    void sendOsc();
    void update();
    void draw();
    void drawActor(const ramActor& actor);
    void guiEvent(ofxUIEventArgs &e);
    void onEnabled();
    void onDisabled();
    
private:
    
    float mDistanceThreshold = 45;
    bool bOn[NMAGNETS];
    
    ramMotionExtractor	mMotionExtractor;
    ofxOscSender mSenderOnOff;

    static const ofColor MAIN_COLOR;
    
};


#endif /* defined(__example_ramMotionExtractor__dpHakoniwaMagnetPendulum__) */
