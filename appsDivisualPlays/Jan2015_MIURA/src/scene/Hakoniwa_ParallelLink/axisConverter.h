//
//  axisConverter.h
//  AxisChanger
//
//  Created by itotaka on 10/11/14.
//
//

#ifndef __AxisChanger__axisConverter__
#define __AxisChanger__axisConverter__

#include "ofMain.h"

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


#endif /* defined(__AxisChanger__axisConverter__) */
