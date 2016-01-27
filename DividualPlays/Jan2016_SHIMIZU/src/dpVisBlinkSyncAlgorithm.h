//
//  dpVisBlinkSyncAlgorithm.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/27/16.
//
//

#ifndef __RAMDanceToolkit__dpVisBlinkSyncAlgorithm__
#define __RAMDanceToolkit__dpVisBlinkSyncAlgorithm__

#include "ramMain.h"
#include "MemeConstants.h"
#include "dpConstants.h"

#define HOST "localhost"
#define PORT 23451


class BlinkSyncAlgorithm: public ramBaseScene{
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "dpVisSyncAlgorithm"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();    
    void BlinkSyncEffect();
    
private:
    
    ofxOscSender sender;
    
    ramOscReceiveTag mReceiver;
    string oscString;
    //position
    ofVec2f position[MEME_NUM];
    float blinkInterval[MEME_NUM];
    float radius[MEME_NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisBlinkSyncAlgorithm__) */




