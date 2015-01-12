//
//  dpHakoVisStruggle.h
//  example-ramMotionExtractor
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#ifndef example_ramMotionExtractor_dpHakoVisStruggle_h
#define example_ramMotionExtractor_dpHakoVisStruggle_h

#include "ramMain.h"
#include "dpConstants.h"

class dpHakoVisStruggle : public ramBaseScene{
public:
    string getName() const {return "dpVStruggle";};
    
    void setupControlPanel();
    void setup();
    void receiveOsc();
    void update();
    void draw();
private:
    ramOscReceiveTag mReceiver; //oscの受信はramOscReceiveTag
    ofPoint mTotalVec;
    float mScale = 1.0;
};

#endif
