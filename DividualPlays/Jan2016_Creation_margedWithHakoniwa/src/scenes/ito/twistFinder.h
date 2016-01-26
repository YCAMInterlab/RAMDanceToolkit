//
//  twistFinder.h
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/17/15.
//
//

#ifndef __example_ramMotionExtractor__twistFinder__
#define __example_ramMotionExtractor__twistFinder__

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "dpConstants.h"

class twistFinder {
    
    public:
    
    double findTwist(ramNode & node, float bendingLimit = 30.0f, float divisionForLimit = 10.0f);
    void debugDraw(ramMotionExtractor & motionExtractor, int nodeNum);
    void drawGraph(vector<float> & history, ofColor drawColor);

    vector <float> his1, his0;
    
};

#endif /* defined(__example_ramMotionExtractor__twistFinder__) */
