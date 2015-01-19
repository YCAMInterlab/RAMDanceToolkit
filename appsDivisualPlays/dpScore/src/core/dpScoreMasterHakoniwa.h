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
#include "ofxMotioner.h"
#include "dpScoreAnalyzeMean.h"

class ofxUITabBar;

DP_SCORE_NAMESPACE_BEGIN

class MasterHakoniwa final {
public:
    static MasterHakoniwa& instance();
    
    void initialize();
    void shutdown();
    
    void setupUI(ofxUITabBar* tabbar);
    void update();
    void draw();
    
    void turnOnValve(int index);
    void turnOffAllPins();
    bool getIsOpeningValve(int index);
    
    void sendScene(const string& name, bool enabled, bool scr0, bool scr1);
    
    constexpr static const int kNumScenes{12};
    
    constexpr static const int kNumWindows{2};
    
    constexpr static const int kNumValvePins{6};
    constexpr static const int kNumPumpPins{2};
    
    constexpr static const float kTopOffset{30.f};
    constexpr static const float kTextSpacing{12.f};
    constexpr static const float kMargin{5.f};
    constexpr static const float kLineWidth{320.f};
    
    constexpr static const int kUpdateFrames{10};
    
    static const string kSceneNames[];
    
    static const int kValvePins[];
    static const int kPumpPins[];
    
    static const ofColor kBackgroundColor;
    static const ofColor kTextColor;
    
    static const int kPumpPinForward;
    static const int kPumpPinBack;
    
    static const float kPumpOpenDur[];
    static const float kPumpCloseDur[];
    
    static const string kHostNameMasterHakoniwa;
    static const int kPortNumberMasterHakoniwa;
    static const string kHostNameCameraUnit;
    static const int kPortNumberCameraUnit;
    
    static const string kOscAddrRamSetScene;
    
    int mLineNum{0};
    ofVec2f mTextLeftCorner{0.f, 0.f};
    
private:
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void sendPin(int pin, bool open);
    
    void onDrawSkeleton(ofxMotioner::EventArgs &e);
    
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
    
    struct Scene {
        bool enabled{false};
        bool scr[MasterHakoniwa::kNumWindows]{false, false};
        bool dirty{false};
    };
    
    ofxOscSender mMasterHakoniwaOscServer;
    ofxOscSender mCameraUnitOscServer;
    
    ofEasyCam mCam;
    ofRectangle mCamViewport;
    
    vector<Valve> mValves;
    vector<Pump> mPumps;
    
    map<string, Scene> mScenes;
    
    enum AnalyzeType {
        AnalyzeTypeMean = 0,
        AnalyzeTypePixelate,
        NumAnalyzeType,
    };
    
    int mAnalyzeType{AnalyzeTypeMean};
    AnalyzeMean mMean;
    
    float mOpenDuration{0.5f};
    
    bool mEmergencyStop{false};
    
    bool mEnableValve{false};
    bool mEnablePump{false};
    bool mOpenPump[kNumPumpPins]{false, false};
    bool mOpenValve[kNumValvePins]{false, false, false, false, false, false};
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH()
{
    return MasterHakoniwa::instance();
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
