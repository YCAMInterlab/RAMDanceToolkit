//
//  dpVisMEME.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/25/16.
//
//

#ifndef __RAMDanceToolkit__dpVisMEME__
#define __RAMDanceToolkit__dpVisMEME__

#include "ramMain.h"
#include "MemeConstants.h"
#include "dpConstants.h"


class MEME: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "dpVisMemeTest"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void radiusCaculate();
        
private:
  
    ramOscReceiveTag mReceiver;
    string oscString;
    //position
    ofVec2f position[MEME_NUM];
    
    float blinkSpeed[MEME_NUM];
    float blinkStrength[MEME_NUM];
    float blinkInterval[MEME_NUM];
    float radius[MEME_NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisMEME__) */
