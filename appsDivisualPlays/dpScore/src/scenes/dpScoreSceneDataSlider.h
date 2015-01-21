//
//  dpScoreSceneDataSlider.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#ifndef __dpScore__dpScoreSceneDataSlider__
#define __dpScore__dpScoreSceneDataSlider__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataSlider final : public SceneBase {
public:
    explicit SceneDataSlider() = default;
    virtual ~SceneDataSlider() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    struct Slider {
        Slider();
        void update(const ofVec2f& v);
        void draw(SceneDataSlider* owner, float w);
        
        const float kClampRange{3.f};
        
        ofVec2f vec;
        ofVec2f prevVec;
    };
    friend class Slider;
    
    vector<ofPtr<Slider> > mSliders;
    ofTrueTypeFont mFont;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataSlider__) */
