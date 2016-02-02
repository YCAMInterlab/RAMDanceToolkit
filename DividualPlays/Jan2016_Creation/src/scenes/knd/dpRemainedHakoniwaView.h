//
//  dpHakoniwaRemainView.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#ifndef __RAMDanceToolkit__dpHakoniwaRemainView__
#define __RAMDanceToolkit__dpHakoniwaRemainView__

#include "KezSlidePoint.h"
#include "ramMain.h"

class dpRemainedHakoniwaView{
public:
    
    void setup();
    void update();
    void receieveOsc();
    void draw();
    
    
private:
    int mCounter = 0;
    
    ramOscReceiveTag mReceiver;
};

#endif /* defined(__RAMDanceToolkit__dpHakoniwaRemainView__) */
