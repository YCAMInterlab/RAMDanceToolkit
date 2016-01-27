//
//  dpVisFrequency2.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/26/16.
//
//

#ifndef __RAMDanceToolkit__dpVisFrequency2__
#define __RAMDanceToolkit__dpVisFrequency2__

#include "ramMain.h"
#include "MemeConstants.h"
#include "dpConstants.h"

class Frequency2: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "dpVisFrequencyTest"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void caculateFrequency();
    
    void radiusCaculate();
    
    void AnlyizeFrequency();
    
private:
    
    ramOscReceiveTag mReceiver;
    string oscString;
    
    float blinkInterval[MEME_NUM];
    float blinkSpeed[MEME_NUM];
    float frequency;
    //speed
    ofVec2f velocity[MEME_NUM];
    
    //position
    ofVec2f position[11];
    
    float radius[11];
    float frequncy_array[11];
    int red[11];
    int green[11];
    int blue[11];
    int j[11];

};

#endif
    /* defined(__RAMDanceToolkit__dpVisFrequency2__) */
