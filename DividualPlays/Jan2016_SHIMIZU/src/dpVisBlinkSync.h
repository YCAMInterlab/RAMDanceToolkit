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
#include "MemeConstants.h"
#include "dpConstants.h"

class BlinkSync: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "dpVisBlinkSync"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void radiusCaculate();
    
    void BlinkSyncEffect();
    
private:
    
    ramOscReceiveTag mReceiver;
    string oscString;
    
    //position
    ofVec2f position[MEME_NUM];
    float blinkInterval[MEME_NUM];
    float radius[MEME_NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisBlinkSync__) */
