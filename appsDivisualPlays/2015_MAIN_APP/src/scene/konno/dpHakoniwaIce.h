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

class dpHakoniwaIce : public ramBaseScene{
public:
    
    dpHakoniwaIce();
    virtual ~dpHakoniwaIce(){};
    
    void setup();
    void setupControlPanel();
    
    void update();
    void draw();
    
    //プログラム非選択時に、DMXを止める
    //------
    void exit();
    virtual void onDisabled();
    //------
    
    inline string getName() const {
        return "dpHIce";
    }
    
    //--Ice---
    //ペルチェ素子の状況識別用変数
    int hantei;
    
    //Manual Control
    //-------------------
    bool manualControl;
    bool frozing;
    bool melting;
    //-------------------
    
    //Dancer Control
    //-------------------
    bool dancerControl;
    bool iceFrozing;
    bool iceMelting;
    float fixationTime;
    float speedThreshold;
    //-------------------
    
    //Distance Control
    //-------------------
    bool distanceControl;
    float distanceThreshold;
    //-------------------
    
    //ファンの操作用
    //-------------------
    bool fanStart;
    int fanVal;
    //-------------------
    
    //時間のカウント
    //-------------------
    //冷やしている時間のカウント
    int frozingCount;
    //凍らせている時間のカウント
    int meltingCount;
    //-------------------
    
    
    
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
