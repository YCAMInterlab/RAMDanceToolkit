//
//  magnetLooper.h
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/15.
//
//

#ifndef __example_ramMotionExtractor__magnetLooper__
#define __example_ramMotionExtractor__magnetLooper__

#include "ramMain.h"
#include "ramMotionExtractor.h"

class magnetLooper : public ramBaseScene{
public:
    
    magnetLooper();
    virtual ~magnetLooper(){};
    
    void setup();
    void setupControlPanel();
    
    void update();
    void draw();
    
    inline string getName() const {
        return "Looper";
    }
    
    //==========================================
    bool manualControl;
    bool looperStop;
    bool speedControl;
    float speedThreshold;
    int hantei;
    
    //test
    int x,y;
    //==========================================
    
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
#endif /* defined(__example_ramMotionExtractor__magnetLooper__) */
