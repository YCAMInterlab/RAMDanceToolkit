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
#define NUM_MSG_STRINGS 20

class magnetLooper : public ramBaseScene{
public:
    
    magnetLooper();
    virtual ~magnetLooper(){};
    
    void setup();
    void setupControlPanel();
    
    void update();
    void draw();
    
    void onEnabled();
    void onDisabled();
    
    inline string getName() const {
        return "dpHLooper";
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
    
    //oscによるスピードコントロール
    //---------------------------
    int oscValLooper1;
    int oscValLooper2;
    //---------------------------
    
    //スピードコントロール to processing
    //---------------------------
    bool looper1Speed;
    int looper1SpeedHantei;
    float looper1SpeedVal;
    
    bool looper2Speed;
    int looper2SpeedHantei;
    float looper2SpeedVal;
    //---------------------------
        
    //test
    int x,y;
    //==========================================
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
private:
    void refleshState();
    
    ramMotionExtractor	motionExtractor;
    ofxOscSender sender;
    ofxOscSender sender2;
    
    //evalaPCヘポジションとスピードの送信
    ofxOscSender sender_evala;
    
    //-----------------------------------
    //processingからポジション受信
    ofxOscReceiver receiver;
    
    //evalaPCからのスピード受信
    ramOscReceiveTag receiver_speed;
    //-----------------------------------
    
    bool mDrawLines;
    bool mDrawTriangle;
    bool mDrawPreview;
    bool mDrawDump;
    
    void example_drawLines();
    void example_drawTriangles();
    void example_drawDump();
    
};
#endif /* defined(__example_ramMotionExtractor__magnetLooper__) */
