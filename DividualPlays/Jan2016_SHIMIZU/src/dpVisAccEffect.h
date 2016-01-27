//
//  dpVisAccEffect.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/21/16.
//
//

#ifndef __RAMDanceToolkit__dpVisAccEffect__
#define __RAMDanceToolkit__dpVisAccEffect__


#include "ramMain.h"

class AccEffect: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "AccEffect"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
        
    
private:

    static const int NUM = 3;
    float blinkSpeed[NUM];

    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    
    string oscString;
    
};


#endif /* defined(__RAMDanceToolkit__dpVisAccEffect__) */
