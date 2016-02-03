//
//  dpAfterBaseSceneDraw.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/03.
//
//

#ifndef __RAMDanceToolkit__dpAfterBaseSceneDraw__
#define __RAMDanceToolkit__dpAfterBaseSceneDraw__

#include "ramMain.h"

#include "dpDancerCue.h"
#include "dpRemainedHakoniwaView.h"
#include "DoSomething.h"
#include "dpBDisplayOutline.h"

class dpAfterBaseSceneDraw{
public:
    
    void setup();
    void update(ofEventArgs& args);
    void draw(ofEventArgs& args);
    
private:
    DoSomething doSomething;
    dpDancerCue dancerCue;
    dpRemainedHakoniwaView remainedView;
    dpBDisplayOutline mBDisplayOutline;
};

#endif /* defined(__RAMDanceToolkit__dpAfterBaseSceneDraw__) */
