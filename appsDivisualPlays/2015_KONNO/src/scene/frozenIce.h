//
//  frozenIce.h
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/09.
//
//

#ifndef __example_ramMotionExtractor__frozenIce__
#define __example_ramMotionExtractor__frozenIce__

#include "ramMain.h"
#include "ramMotionExtractor.h"

class frozenIce : public ramBaseScene{
public:
    
    frozenIce();
    virtual ~frozenIce(){};
    
    void setup();
    void setupControlPanel();
    
    void update();
    void draw();
    
    inline string getName() const {
        return "frozenIce";
    }
    
    //--Ice---
    int hantei;
    bool iceStart;
    int count;
    
private:
    
    ramMotionExtractor	motionExtractor;
    ofxOscSender sender;
    
    bool mDrawLines;
    bool mDrawTriangle;
    bool mDrawPreview;
    bool mDrawDump;
    
    void example_drawLines();
    void example_drawTriangles();
    void example_drawDump();
    
};


#endif /* defined(__example_ramMotionExtractor__frozenIce__) */
