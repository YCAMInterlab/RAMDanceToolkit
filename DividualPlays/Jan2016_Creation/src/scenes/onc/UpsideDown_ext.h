//
//  UpsideDown_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#ifndef UpsideDown_ext_h
#define UpsideDown_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class UpsideDown_ext : public ramBaseScene{
public:
    string getName() const {return "UpsideDown_ext";}
    
    void setupControlPanel();
    
    void setup();
    void loopAngle(float &a);
    void update();
    void draw();
    
    void onValueChanged(ofxUIEventArgs& e);
    
private:
    ramMotionExtractor mMex;
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    ramFilterEach<ramUpsideDown> mUpsideDown;
    
    float mOffset;
    
    ofVec3f mAutoRotateSpeed;
    struct { bool x, y, z; } mAutoRotate;
};

#endif /* UpsideDown_ext_h */
