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
    static const int kNumValvePins{6};
    static const int kNumPumpPins{2};
    
    const int kValvePins[kNumValvePins]{
        2, 3, 4, 5, 6, 7,
    };
    
    const int kPumpPins[kNumPumpPins]{
        8, 9,
    };
    
    const int kPumpPinForward{kPumpPins[0]};
    const int kPumpPinBack{kPumpPins[1]};
    
    void initialize();
    void shutdown();
    
    void setupUI(ofxUITabBar* tabbar);
    void update();
    void draw();
    
    void turnOnValve(int index);
    void turnOffAllPins();
    bool getIsOpeningValve(int index);
    
    void sendScene(const string& name,
                   bool enabled,
                   bool scr0,
                   bool scr1,
                   bool scr2,
                   bool scr3);
    
    static MasterHakoniwa& instance();
    
    static const int kNumScenes;
    static const string kSceneNames[];
    
private:
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void sendPin(int pin, bool open);
    
    struct Valve {
        bool doOpen{false};
        bool opening{false};
        float openedTime{0.f};
        int pin{0};
    };
    
    struct Pump {
        bool opening{false};
        float duration{8.f};
        float idle{2.f};
        float openedTime{0.f};
        int pin{0};
    };
    
    const int kUpdateFrames{10};
    
    ofxOscSender mMasterHakoniwaOscServer;
    ofxOscSender mCameraUnitOscServer;
    
    vector<Valve> mValves;
    vector<Pump> mPumps;
    
    float mOpenDuration{0.5f};
    bool mEnable{false};
    bool mOpenPumpForward{false};
    bool mOpenPumpBack{false};
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH() { return MasterHakoniwa::instance(); }

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
