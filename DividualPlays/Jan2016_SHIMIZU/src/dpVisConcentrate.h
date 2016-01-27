//
//  dpVisConcentrate.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#ifndef __RAMDanceToolkit__dpVisConcentrate__
#define __RAMDanceToolkit__dpVisConcentrate__

#include "ramMain.h"

class Concentrate: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "ConcentrateLevel"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void SceneChange();    
    
private:
    
    static const int NUM = 3;
    float ConcentrateLevel[NUM];
    
    //position
    ofVec2f position[NUM];

    
    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    
    string oscString;
    
};
#endif /* defined(__RAMDanceToolkit__dpVisConcentrate__) */
