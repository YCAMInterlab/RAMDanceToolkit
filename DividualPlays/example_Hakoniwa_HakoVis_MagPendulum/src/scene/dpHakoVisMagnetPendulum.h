//
//  doHakoVisRecordGrid.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_doHakoVisRecordGrid_h
#define RAMDanceToolkit_doHakoVisRecordGrid_h

#include "dpRecordBufferCircle.h"

class dpHakoVisMagnetPendulum : public ramBaseScene{
public:
    
    string getName() const{return "dpVisMagPendulum";}
    
    void setupControlPanel();
    void setup();
    void record(ofPoint pt);
    void changeRecordTarget();
    void receiveOsc();
    void update();
    void draw();
    void onEnabled();
    
private:
 
    vector<dpRecordBufferCircle>mCircles;
    
    static const int DIV_X = 3;
    static const int DIV_Y = 2;
    static const int SPACE = 250;
    
    int mRecordTargetNum = 0;
    int mBeginFrame = 0;
    
    float mScale = 800.0;
    
    ofVec2f mPos;
    
    ramOscReceiveTag mReceiver;
    
};

#endif
