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
    void onEnabled();
    void onDisabled();
    
    inline string getName() const {
        return "dpHEyeBall";
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
    
    //Eye Camera
    //--------------
    //camera1
    float camera1_x;
    float camera1_y;
    float camera1_z;
    int camera1_AngleX;
    int camera1_AngleY;
    
    //camera2
    float camera2_x;
    float camera2_y;
    float camera2_z;
    int camera2_AngleX;
    int camera2_AngleY;
    
    //camera3
    float camera3_x;
    float camera3_y;
    float camera3_z;
    int camera3_AngleX;
    int camera3_AngleY;
    
    //camera4
    float camera4_x;
    float camera4_y;
    float camera4_z;
    int camera4_AngleX;
    int camera4_AngleY;
    //--------------
    
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
    
    //pendulum Contorl
    bool pendulumContorl;
    float pendulumPosX;
    float pendulumPosZ;
    
    //servo value
    //---------------
    bool sendServo;
    
    //Eye1 servo
    int servoX_val;
    int servoY_val;
    
    //Eye1 servo
    int servoX2_val;
    int servoY2_val;
    
    //Eye1 servo
    int servoX3_val;
    int servoY3_val;
    
    //Eye1 servo
    int servoX4_val;
    int servoY4_val;
    //---------------
    
    //led
    int ledVal;
    //----------
    
private:
    void refleshState();
    
    ramMotionExtractor	motionExtractor;
    ofxOscSender sender;
    ofxOscSender sender2;
    
    ramOscReceiveTag receiver;
    
    bool mDrawLines;
    bool mDrawTriangle;
    bool mDrawPreview;
    bool mDrawDump;
    
    void example_drawLines();
    void example_drawTriangles();
    void example_drawDump();
    
};
#endif /* defined(__example_ramMotionExtractor__eyeBall__) */
