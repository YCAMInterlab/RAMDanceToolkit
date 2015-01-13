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
    //共通
    int pointBoxSize;
    float defaultZ_val;
    
    //eyeCamera setup
    ofPoint camera1;
    ofPoint camera2;
    ofPoint camera3;
    ofPoint camera4;
    float camera1_x;
    float camera1_y;
    float camera1_z;
    int camera1_AngleX;
    int camera1_AngleY;
    
    //Manual Control
    bool manualControl;
    bool reset;
    float posX;
    float posY;
    float posZ;
    
    //Auto Turn
    bool autoTurn;
    ofPoint pos;
    float radius;
    float phase;
    float speed;
    
    //Node Control
    bool nodeControl;
    ofPoint nodeVal;
    
    //servo value
    bool sendServo;
    int servoX_val;
    int servoY_val;
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
