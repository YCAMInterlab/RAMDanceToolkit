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
#include "dpScoreHakoniwaColorOfWater.h"
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
    
    void turnOffAllScenes();
    
    UniqueStringStack getUniqueScenes() const { return mUniqueScenes; }
    
    bool getIsWindowOn(int windowIndex) const;

    void setUniqueScene(int sceneIndex, bool win0, bool win1);
    
    void setUniqueScore(int sceneIndex);
    size_t getNumUniqueScores() const;
    
    void guiEvent(ofxUIEventArgs& e);
    
    void doSomething(int rand);
    
    HakoniwaColorOfWater& getColorOfWater() {return mColorOfWater; }
    
    enum class AnalyzeType {
        Mean = 0,
        Pixelate,
        Num,
    };
    
    enum Window {
        WINDOW_0,
        WINDOW_1,
        NUM_WINDOWS,
    };
    
    constexpr static const float kTopOffset{30.f};
    constexpr static const float kTextSpacing{12.f};
    constexpr static const float kMargin{5.f};
    constexpr static const float kLineWidth{320.f};
    constexpr static const float kLineHeight{12.f};
    
    constexpr static const int kUpdateFrames{10};
    
    constexpr static const float kGuiWidth{250.f};
    constexpr static const float kGuiHeight{400.f};
    
    static const ofColor kBackgroundColor;
    static const ofColor kTextColor;
    static const ofColor kTextColorDark;
    
    static const string kOscAddrRamSetScene;
    static const string kOscAddrRamDoSomething;
    
    static const string kScoreBlack;
    
    ofVec2f mTextLeftCorner{0.f, 0.f};
    
private:
    struct Scene {
        bool isEnabled() const;
        bool window[NUM_WINDOWS]{false, false};
        bool dirty{false};
        bool hasCamera{false};
        bool maestro{false};
        bool allOff{false};
    };
    
    struct AnalyzeNone {};
    
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void sendSetScene(const string& name, bool win0, bool win1);
    
    void sendChangeScore(const string& name,
                         bool maintainSceneNames = true);
    
    void onDrawSkeleton(ofxMotioner::EventArgs &e);
    
    HakoniwaColorOfWater mColorOfWater;
    
    ofxOscSender mCameraUnitOscSender;
    ofxOscSender mScoreOscSender;
    
    UniqueStringStack mUniqueScenes;
    vector<UniqueStringStack> mUniqueScores;
    UniqueStringStack mUniqueScoreBodies;
    
    string mScoreCorrelation;
    
    ofEasyCam mCam;
    ofRectangle mCamViewport;
    
    map<string, Scene> mScenes;
    Scene mLatestScene;
    
    struct {
        bool screenA{false};
        bool screenB{false};
        
        ofxUITextInput *textInput{nullptr};
    } mUISceneInfo;
    
    string mCurrentScore{""};
    int mCurrentScoreComplexity{0};
    int mMaxComplexity{0};
    
    AnalyzeMean mAnalyzeMean;
    AnalyzePixelate mAnalyzePixelate;
    
    bool mEmergencyStop{false};
    
    bool mEnableCameraUnit{false};
    bool mEnableOscOutRDTK{false};
    bool mEnableOscOutScore{false};
    bool mEnableShowHakoniwaTitle{false};
        
    float mScoreSensorScale{1.f};
    
    float mPrevTimeSceneChanged{0.f};
    ofBuffer mSceneTimesBuffer;
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH()
{
    return MasterHakoniwa::instance();
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
