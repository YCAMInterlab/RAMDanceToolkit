//
//  dpVisSceneChange.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/27/16.
//
//

#ifndef __RAMDanceToolkit__dpVisSceneChange__
#define __RAMDanceToolkit__dpVisSceneChange__

#include "ramMain.h"
#include "MemeConstants.h"
#include "dpConstants.h"

#define HOST "localhost"
#define PORT 23451


class SceneChange: public ramBaseScene{
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "SceneChange"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void BlinkSyncEffect();
    
private:
    
    ofxOscSender sender;
    
    ramOscReceiveTag mReceiver;
    string oscString;
    float sceneChange[MEME_NUM];
    
    
};
#endif /* defined(__RAMDanceToolkit__dpVisSceneChange__) */
