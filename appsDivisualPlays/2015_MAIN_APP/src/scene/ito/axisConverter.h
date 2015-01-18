//
//  axisConverter.h
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#ifndef __example_ramMotionExtractor__axisConverter__
#define __example_ramMotionExtractor__axisConverter__


#include "ramMain.h"

class axisConverter {
    
public:
    
    // change axis from world coodinate Axis system to Axis system attached to nodeOrigin - node vector.
    ofVec3f    convertAxis(ofNode& node, ofNode& nodeOrigin);
    void       draw();
    
    ofQuaternion    q;
    ofQuaternion    qSC;
    ofVec3f         euler;
    ofNode          node;
    
};


#endif /* defined(__example_ramMotionExtractor__axisConverter__) */
