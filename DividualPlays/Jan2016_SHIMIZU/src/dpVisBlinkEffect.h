//
//  dpVisBlinkEffect.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/21/16.
//
//

#ifndef __RAMDanceToolkit__dpVisBlinkEffect__
#define __RAMDanceToolkit__dpVisBlinkEffect__

#include "ramMain.h"

class BlinkEffect: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "BlinkEffect"; }
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    
    void blinkDetection();
    
    
private:
    
    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    
    string oscString;
    
    
    static const int NUM = 3;
    
    //position
    ofVec2f position[NUM];
    //speed
    ofVec2f velocity[NUM];
    
    float blinkSpeed[NUM];
    float blinkStrength[NUM];

    float blinkInterval[NUM];
    
    int red[NUM];
    int green[NUM];
    int blue[NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisBlinkEffect__) */
