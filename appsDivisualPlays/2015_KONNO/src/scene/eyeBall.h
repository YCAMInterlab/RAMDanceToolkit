//
//  eyeBall.h
//  example-ramMotionExtractor
//
//  Created by ycam on 2015/01/11.
//
//

#ifndef __RAMDanceToolkit__eyeBall__
#define __RAMDanceToolkit__eyeBall__

#include "ramMain.h"
#include "ramMotionExtractor.h"

class eyeBall : public ramBaseScene{
public:
    
    eyeBall();
    virtual ~eyeBall(){};
    
    void setup();
    void setupControlPanel();
    
    void update();
    void draw();
    
    inline string getName() const {
        return "eyeBall";
    }
    
    //----------
    ofBoxPrimitive point;
    float radiusX, radiusY;
    float phase;
    float speed;
    float roll, pitch, yaw;
    //----------
    
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
#endif /* defined(__example_ramMotionExtractor__eyeBall__) */
