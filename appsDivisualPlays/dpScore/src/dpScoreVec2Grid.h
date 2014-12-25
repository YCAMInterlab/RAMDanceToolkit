//
//  dpScoreVec2Grid.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreVec2Grid__
#define __dpScore__dpScoreVec2Grid__

#include "dpScoreBase.h"

class dpScoreVec2Grid final : public dpScoreBase {
public:
    dpScoreVec2Grid() {}
    virtual ~dpScoreVec2Grid() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mGridBuffer;
    vector<ofVec3f> mGridVertices;
    ofVbo mGridVbo;
    
    ofEasyCam mCam;
    
    float mSensorScale;
    ofVec2f mVec;
    
    int mGridStep;
    int mGridW;
    int mGridH;
};

#endif /* defined(__dpScore__dpScoreVec2Grid__) */
