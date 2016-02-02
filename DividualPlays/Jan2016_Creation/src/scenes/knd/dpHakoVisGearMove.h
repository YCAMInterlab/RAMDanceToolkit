//
//  dpHakoVisGearMove.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#ifndef __RAMDanceToolkit__dpHakoVisGearMove__
#define __RAMDanceToolkit__dpHakoVisGearMove__

#include "ramMain.h"

class dpHakoVisGearMove : public ramBaseScene {

public:
    
     string getName() const { return "dpVisGearMove"; }
     void setupControlPanel();
     void setup();
     void update();
     void draw();
     void onPanelChanged(ofxUIEventArgs& e);

    
private:
    
};

#endif /* defined(__RAMDanceToolkit__dpHakoVisGearMove__) */
