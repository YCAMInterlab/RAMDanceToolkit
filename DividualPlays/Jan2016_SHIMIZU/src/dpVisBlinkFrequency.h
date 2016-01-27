//
//  dpVisBlinkFrequency.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#ifndef __RAMDanceToolkit__dpVisBlinkFrequency__
#define __RAMDanceToolkit__dpVisBlinkFrequency__

#include "ramMain.h"
#include "MemeConstants.h"
#include "dpConstants.h"

class BlinkFrequency: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "dpVisBlinkFrequency"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void caculateFrequency();
    
private:
    
    
    ramOscReceiveTag mReceiver;
    string oscString;
    
    float blinkSpeed[MEME_NUM];
    float blinkInterval[MEME_NUM];
    float frequency;

    
    //position
    ofVec2f position[11];
    //speed
    ofVec2f velocity[MEME_NUM];
    
    float radius[11];
    float frequncy_array[11];
    int red[11];
    int green[11];
    int blue[11];
    
};
#endif /* defined(__RAMDanceToolkit__dpVisBlinkFrequency__) */
