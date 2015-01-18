//
//  dpScoreSceneMaster.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#ifndef __dpScore__dpScoreSceneMaster__
#define __dpScore__dpScoreSceneMaster__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"
#include "ofxOsc.h"

DP_SCORE_NAMESPACE_BEGIN

struct MasterNode final : public ofxMot::Node {
    MasterNode();
    virtual ~MasterNode() = default;
    MasterNode& operator = (const MasterNode& rhs);
    
    void update()
    {
        const auto pos = getGlobalPosition();
        if (!vecInited) {
            prevPos = pos;
            vecInited = true;
        }
        const auto d = pos.distance(prevPos);
        totalDistance += d * 0.1f;
        prevPos = pos;
    }
    
    ofVec3f prevPos;
    float totalDistance{0.f};
    bool vecInited{false};
};

class SceneMaster final : public SceneBodyBase<MasterNode> {
public:
    explicit SceneMaster() = default;
    virtual ~SceneMaster() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void setupSkeleton(SkeletonPtr skl) override;
    void updateSkeleton(SkeletonPtr skl) override;
    void exitSkeleton(SkeletonPtr skl) override;
    
private:
    void sendPin(int pin, bool open);
    void sendScene(const string& name,
                   bool enabled,
                   bool scr0,
                   bool scr1,
                   bool scr2,
                   bool scr3);
    
    struct Valve {
        float addtion{0.f};
        bool open{false};
        bool opening{false};
        float openedTime{0.f};
    };
    
    ofEasyCam mCam;
    map<string, Valve> mValves;
    ofTrueTypeFont mFont;
    
    ofxOscSender mMasterHakoniwaOscServer;
    ofxOscSender mCameraUnitOscServer;
    float mLimit{1000.f};
    float mOpenDur{1.f};
    unsigned long mFrameNum{0};
    unsigned long mPrevOfFrame{0};
    
    bool mEnable{false};
    bool mPumpOn{false};
    
    bool mOpenPumpForward{false};
    bool mOpenPumpBack{false};
    
    enum Valves {
        VALVE_0 = 2,
        VALVE_1 = 3,
        VALVE_2 = 4,
        NUM_VALVES,
    };
    enum Pumps {
        PUMP_FORWARD = 8,
        PUMP_BACK = 9,
        NUM_PUMPS,
    };
    
    ofVec4f mMean;
    ofVec4f mMeanPrev;
    ofVec4f mMeanAddtion;
    float mMeanLimit{5};
    int mPrevScene{0};
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneMaster__) */
