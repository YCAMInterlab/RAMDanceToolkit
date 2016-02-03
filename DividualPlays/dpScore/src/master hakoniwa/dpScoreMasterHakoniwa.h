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
#include "dpScoreHakoniwaColorOfWater.h"
#include "dpScoreSceneController.h"
#include "ofxOsc.h"
#include "ofxMotioner.h"

class ofxUIEventArgs;

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
    
    void guiEvent(ofxUIEventArgs& e);
    
    HakoniwaColorOfWater& getColorOfWater() {return mColorOfWater; }
    SceneController& getSceneController() { return mSceneController; }
    
    enum class AnalyzeType {
        Mean = 0,
        Pixelate,
        Num,
    };
    
    constexpr static const float kTopOffset{30.f};
    constexpr static const float kTextSpacing{12.f};
    constexpr static const float kMargin{5.f};
    constexpr static const float kLineWidth{320.f};
    constexpr static const float kLineHeight{12.f};
    
    constexpr static const float kGuiWidth{250.f};
    constexpr static const float kGuiHeight{400.f};
    
    static const ofColor kBackgroundColor;
    static const ofColor kTextColor;
    static const ofColor kTextColorDark;
    
    ofVec2f mTextLeftCorner{0.f, 0.f};
    
private:
    MasterHakoniwa() = default;
    ~MasterHakoniwa() = default;
    
    MasterHakoniwa(const MasterHakoniwa&) = delete;
    MasterHakoniwa& operator = (const MasterHakoniwa&) = delete;
    
    void onDrawSkeleton(ofxMotioner::EventArgs &e);
    
    HakoniwaColorOfWater mColorOfWater;
    SceneController mSceneController;
    
    ofEasyCam mCam;
    ofRectangle mCamViewport;
    
    AnalyzeMean mAnalyzeMean;
    AnalyzePixelate mAnalyzePixelate;
    
    ofxOscSender mOscSenderLighting;
    const string kOscAddrLighting {"/dp/light/masterHakoniwa"};
    
    bool mEmergencyStop{false};
    
    bool mEnableCameraUnit{false};
};

typedef MasterHakoniwa MH;

inline MasterHakoniwa& getMH()
{
    return MasterHakoniwa::instance();
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreMasterHakoniwa__) */
