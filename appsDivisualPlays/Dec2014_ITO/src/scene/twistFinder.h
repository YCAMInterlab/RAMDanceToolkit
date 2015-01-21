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
    
    double findTwist(ramNode & node, float bendingLimit = 30.0f, float divisionForLimit = 10.0f);       // check rotation on a line between A & B
    
    void debugDraw(ramMotionExtractor & motionExtractor);
    void debugDraw2(ramMotionExtractor & motionExtractor);
    void debugDraw3(ramMotionExtractor & motionExtractor);
    void debugDraw4(ramMotionExtractor & motionExtractor, int nodeNum);
    
    void calculateVector(const ramNode & node0, const ramNode & node1, ofVec3f & redVec);
    
    void drawGraph(vector<float> & history, ofColor drawColor);

    vector <float> his1, his0, hisSub;
    
    // Oreteru no wo modosu. de, on-axis no kaiten wo keisan.
    // how about thinkng some nodes rotation on just one long line?

};

#endif /* defined(__example_ramMotionExtractor__twistFinder__) */
