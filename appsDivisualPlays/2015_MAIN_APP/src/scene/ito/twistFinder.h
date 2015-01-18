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
    
    void findTwist(ramNode & node, ramNode & nodeOrigin);       // check rotation on a line between A & B
    void debugDraw(ramMotionExtractor & motionExtractor);
    


    
    // Oreteru no wo modosu. de, on-axis no kaiten wo keisan.
    
    // how about thinkng some nodes rotation on just one long line?

};

#endif /* defined(__example_ramMotionExtractor__twistFinder__) */
