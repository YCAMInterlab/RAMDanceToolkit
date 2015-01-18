//
//  dpScoreMasterHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#ifndef __dpScore__dpScoreMasterHakoniwa__
#define __dpScore__dpScoreMasterHakoniwa__

#include "dpScoreCommon.h"
#include "ofxOsc.h"

class ofxUITabBar;

class MasterHakoniwa final {
public:
    void initialize();
    void shutdown();
    
    void setupUI(ofxUITabBar* tabbar);
    void update();
    void draw();
    
    void turnOffAllPins();
    
    static MasterHakoniwa& instance();
    
    static const int kNumScenes;
    static const string kSceneNames[];
    
    enum ValvePins {
        VALVE_PIN_0 = 2,
        VALVE_PIN_1 = 3,
        VALV_PINE_2 = 4,
        VALVE_PIN_3 = 3,
        VALVE_PIN_4 = 4,
        VALV_PINE_5 = 5,
        NUM_VALVE_PINS,
    };
    enum PumpPins {
        PUMP_PIN_FORWARD = 8,
        PUMP_PIN_BACK = 9,
        NUM_PUMP_PINS,
    };
    
private:
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void sendPin(int pin, bool open);
    void sendScene(const string& name,
                   bool enabled,
                   bool scr0,
                   bool scr1,
                   bool scr2,
                   bool scr3);
    
    struct Valve {
        bool doOpen{false};
        bool opening{false};
        float openedTime{0.f};
        int pin{0};
    };
    
    const int kUpdateFrames{10};
    
    ofxOscSender mMasterHakoniwaOscServer;
    ofxOscSender mCameraUnitOscServer;
    
    vector<Valve> mValves;
    
    float mOpenDuration{0.5f};
    bool mEnable{false};
    bool mOpenPumpForward{false};
    bool mOpenPumpBack{false};
    bool mPumpOn{false};
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH() { return MasterHakoniwa::instance(); }

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
