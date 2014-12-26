//
//  HakovisLaser.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 12/26/14.
//
//

#ifndef __RAMDanceToolkit__HakovisLaser__
#define __RAMDanceToolkit__HakovisLaser__

#include "ramMain.h"

class HakovisLaser : public ramBaseScene {
public:
    HakovisLaser();
    virtual ~HakovisLaser();
    
    void setupControlPanel();
    
    void update();
    void draw();
    void exit();
    
    void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
    inline
    string getName() const { return "HakovisLaser"; }
    
private:
    ramOscReceiveTag* mOscReceiverTag;
    
    struct Blob {
        Blob(int index, int nVerts);
        void draw();
        
        int idx;
        ofPolyline polyLine;
    };
    
    vector<ofPtr<Blob> > mBlobs;
    
    float mBlobScale;
    
};

#endif /* defined(__RAMDanceToolkit__HakovisLaser__) */
