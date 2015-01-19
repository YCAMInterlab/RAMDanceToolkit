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
    
    //マニュアルコントロールでストップ
    //---------------------------
    //oldしゃくとり
    bool looperStop;
    //newしゃくとり
    bool looperStop2;
    //---------------------------
    
    
    bool speedControl;
    //スピードコントロールで判定
    //---------------------------
    //oldしゃくとり
    int hantei;
    float speedThreshold;
    //newしゃくとり
    int hantei2;
    float speedThreshold2;
    //---------------------------
    
    bool looper1Speed;
    int looper1SpeedHantei;
    float looper1SpeedVal;
    
    bool looper2Speed;
    int looper2SpeedHantei;
    float looper2SpeedVal;
    
    //test
    int x,y;
    //==========================================
    
private:
    
    ramMotionExtractor	motionExtractor;
    ofxOscSender sender;
    ofxOscSender sender2;
    
    bool mDrawLines;
    bool mDrawTriangle;
    bool mDrawPreview;
    bool mDrawDump;
    
    void example_drawLines();
    void example_drawTriangles();
    void example_drawDump();
    
};
#endif /* defined(__example_ramMotionExtractor__magnetLooper__) */
