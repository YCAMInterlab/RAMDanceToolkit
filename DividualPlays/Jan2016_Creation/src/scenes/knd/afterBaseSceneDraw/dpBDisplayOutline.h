//
//  dpBDisplayOutline.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/03.
//
//

#ifndef __RAMDanceToolkit__dpBDisplayOutline__
#define __RAMDanceToolkit__dpBDisplayOutline__

#include "ramMain.h"
#include "dpConstants.h"

class dpBDisplayOutline{
public:
    void setup();
    void update();
    void receiveOsc();
    void draw();
protected:
    ramOscReceiveTag mReceiver;
    bool mIsEnable = false;
    float mLineWidth = 1.0;
};

#endif /* defined(__RAMDanceToolkit__dpBDisplayOutline__) */
