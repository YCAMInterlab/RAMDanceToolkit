//
//  dpVisBlinkSync.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#ifndef __RAMDanceToolkit__dpVisBlinkSync__
#define __RAMDanceToolkit__dpVisBlinkSync__

#include "ramMain.h"

class BlinkSync: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "BlinkSync"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void radiusCaculate();
    
    void BlinkSyncEffect();
    
private:
    
    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    string oscString;
    
    static const int NUM = 3;
    //position
    ofVec2f position[NUM];
    float blinkInterval[NUM];
    float radius[NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisBlinkSync__) */
