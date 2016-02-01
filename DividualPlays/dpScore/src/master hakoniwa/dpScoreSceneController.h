//
//  dpScoreSceneController.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/15.
//
//

#ifndef __dpScore__dpScoreSceneController__
#define __dpScore__dpScoreSceneController__

#include "dpScoreCommon.h"
#include "dpScoreUniqueStack.h"
#include "ofxOsc.h"

class ofxUITabBar;
class ofxUIEventArgs;
class ofxUITextInput;

DP_SCORE_NAMESPACE_BEGIN

class SceneController {
public:
    void initialize();
    
    void setupUI(ofxUITabBar* tabbar);
    void draw();
    
    void stopAll();
    void turnOffAllScenes();
    
    void sendSetScene(const string& name, bool win0, bool win1);
    
    /*
    void sendChangeScore(const string& name,
                         bool maintainSceneNames = true);
    */
    UniqueStringStack getUniqueScenes() const { return mUniqueScenes; }
    
    bool getIsWindowOn(int windowIndex) const;
    
    void setUniqueScene(int sceneIndex, bool win0, bool win1);
    
    //void setUniqueScore(int sceneIndex);
    //size_t getNumUniqueScores() const;
    
    void doSomething(int rand);
    
    void clearTimeLog();
    void writeTimeLog();
    
    bool enableOscOutRDTK{false};
    //bool enableOscOutScore{false};
    //bool enableShowHakoniwaTitle{false};
    
private:
    void loadScenes(ofxXmlSettings& xml);
    
    void guiEvent(ofxUIEventArgs& e);
    
    enum Window {
        WINDOW_0,
        WINDOW_1,
        NUM_WINDOWS,
    };
    
    struct Scene {
        bool isEnabled() const;
        bool window[NUM_WINDOWS]{false, false};
        bool dirty{false};
        bool hasCamera{false};
        bool maestro{false};
        bool allOff{false};
    };
    
    static const string kOscAddrRamSetScene;
    static const string kOscAddrRamDoSomething;
    
    //static const string kScoreBlack;
    
    ofxOscSender mCameraUnitOscSender;
    
    //ofxOscSender mScoreOscSender;
    
    UniqueStringStack mUniqueScenes;
    /*
    vector<UniqueStringStack> mUniqueScores;
    UniqueStringStack mUniqueScoreBodies;
    */
    
    map<string, Scene> mScenes;
    Scene mLatestScene;
    
    struct {
        bool screenA{false};
        bool screenB{false};
        
        ofxUITextInput *textInput{nullptr};
    } mUISceneInfo;
    
    /*
    string mCurrentScore{""};
    string mScoreCorrelation{""};
    int mCurrentScoreComplexity{0};
    int mMaxComplexity{0};
    
    float mScoreSensorScale{1.f};
    */
    
    float mPrevTimeSceneChanged{0.f};
    ofBuffer mSceneTimesBuffer;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneController__) */
