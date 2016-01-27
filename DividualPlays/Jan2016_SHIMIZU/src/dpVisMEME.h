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

class MEME: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "MEME"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void radiusCaculate();
        
private:
  
    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    string oscString;
    
    static const int NUM = 3;
    //position
    ofVec2f position[NUM];
    
    float blinkSpeed[NUM];
    float blinkStrength[NUM];
    float blinkInterval[NUM];
    
    float radius[NUM];
    
};

#endif /* defined(__RAMDanceToolkit__dpVisMEME__) */
