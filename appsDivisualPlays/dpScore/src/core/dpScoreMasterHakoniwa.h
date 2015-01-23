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
#include "dpScoreAnalyzeMean.h"
#include "dpScoreAnalyzePixelate.h"
#include "dpScoreUniqueStack.h"
#include "ofxOsc.h"
#include "ofxMotioner.h"
#include "ofxUI.h"

DP_SCORE_NAMESPACE_BEGIN

class MasterHakoniwa final {
public:
    static MasterHakoniwa& instance();
    
    void initialize();
    void shutdown();
    
    void setupUI(ofxUITabBar* tabbar);
    void update();
    void updateCameraUnit(ofxEventMessage& m);
    void draw();
    
    void turnOnValve(int index);
    void turnOffAllPins();
    bool getIsOpeningValve(int index);
    void turnOffAllScenes();
    
    UniqueStringStack getUniqueScenes() const { return mUniqueScenes; }
    
    bool getIsWindowOn(int windowIndex) const;

    void setUniqueScene(int sceneIndex, bool win0, bool win1);
    
    void setUniqueScore(int sceneIndex);
    size_t getNumUniqueScores() const;
    
    void guiEvent(ofxUIEventArgs& e);
    
    void doSomething(int rand);
    
    enum class AnalyzeType {
        Mean = 0,
        Pixelate,
        Num,
    };
    
    enum class StageElements {
        Hakoniwa = 0,
        Dancer,
        Lignt,
        Sound,
        Score,
        Num,
    };
    
    enum Window {
        WINDOW_0,
        WINDOW_1,
        NUM_WINDOWS,
    };
    
    constexpr static const int kNumValvePins{6};
    constexpr static const int kNumPumpPins{2};
    
    constexpr static const float kTopOffset{30.f};
    constexpr static const float kTextSpacing{12.f};
    constexpr static const float kMargin{5.f};
    constexpr static const float kLineWidth{320.f};
    
    constexpr static const int kUpdateFrames{10};
    
    constexpr static const float kGuiWidth{250.f};
    
    static const int kValvePins[];
    static const int kPumpPins[];
    
    static const ofColor kBackgroundColor;
    static const ofColor kTextColor;
    static const ofColor kTextColorDark;
    
    static const int kPumpPinForward;
    static const int kPumpPinBack;
    
    static const float kPumpOpenDur[];
    static const float kPumpCloseDur[];
    
    static const string kHostNameMasterHakoniwa;
    static const int kPortNumberMasterHakoniwa;
    static const string kHostNameCameraUnit;
    static const int kPortNumberCameraUnit;
    static const string kHostNameScore;
    static const int kPortNumberScore;
    
    static const string kOscAddrRamSetScene;
    
    static const string kScoreBlack;
    
    ofVec2f mTextLeftCorner{0.f, 0.f};
    
private:
    struct Valve {
        void update(MasterHakoniwa* mh);
        
        bool open{false};
        bool prevOpen{false};
        float openedTime{0.f};
        float closedTime{0.f};
        int pin{0};
        int nOpend{0};
    };
    
    struct Pump {
        void update(MasterHakoniwa* mh);
        
        bool open{false};
        bool prevOpen{false};
        float openedTime{0.f};
        float closedTime{0.f};
        int pin{0};
    };
    
    struct Scene {
        bool isEnabled() const;
        bool window[NUM_WINDOWS]{false, false};
        bool dirty{false};
    };
    
    struct AnalyzeNone {};
    
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void sendSetScene(const string& name, bool win0, bool win1);
    
    void sendPin(int pin, bool open);
    
    void sendChangeScore(const string& name,
                         bool maintainSceneNames = true);
    
    void onDrawSkeleton(ofxMotioner::EventArgs &e);
    
    ofxOscSender mMasterHakoniwaOscSender;
    ofxOscSender mCameraUnitOscSender;
    ofxOscSender mScoreOscSender;
    
    UniqueStringStack mUniqueScenes;
    vector<UniqueStringStack> mUniqueScores;
    UniqueStringStack mUniqueScoreBodies;
    
    string mScoreCorrelation;
    
    ofEasyCam mCam;
    ofRectangle mCamViewport;
    
    vector<Valve> mValves;
    vector<Pump> mPumps;
    
    map<string, Scene> mScenes;
    
    vector<string> mNoCamScenes;
    bool mNoCameraData{false};
    
    string mCurrentScore{""};
    int mCurrentScoreComplexity{0};
    int mMaxComplexity{0};
    
    AnalyzeType mAnalyzeType{AnalyzeType::Mean};
    AnalyzeMean mAnalyzeMean;
    AnalyzePixelate mAnalyzePixelate;
    
    float mValveOpenDuration{0.3f};
    
    bool mEmergencyStop{false};
    
    ofxUIToggle* mEnableAllToggle{nullptr};
    bool mEnableMotioner{false};
    bool mEnableCameraUnit{false};
    bool mEnableOscOutRDTK{false};
    bool mEnableOscOutMH{false};
    bool mEnableOscOutScore{false};
    bool mEnableShowHakoniwaTitle{false};
    
    float mEnabledTime{0.f};
    
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH()
{
    return MasterHakoniwa::instance();
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
