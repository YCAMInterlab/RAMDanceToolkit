//
//  dpScoreVec2Plotter.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreVec2Plotter__
#define __dpScore__dpScoreVec2Plotter__

#include "dpScoreBase.h"

class dpScoreVec2Plotter final : public dpScoreBase {
public:
    dpScoreVec2Plotter() {}
    virtual ~dpScoreVec2Plotter() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mCircleBuffer;
    vector<ofVec3f> mCircleVertices;
    vector<ofFloatColor> mCircleColors;
    ofVbo mCircleVbo;
    
    float mSensorScale;
    ofVec2f mVec;
};


#endif /* defined(__dpScore__dpScoreVec2Plotter__) */
