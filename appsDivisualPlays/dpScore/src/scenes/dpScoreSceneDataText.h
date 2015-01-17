//
//  dpScoreSceneDataText.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#ifndef __dpScore__dpScoreSceneDataText__
#define __dpScore__dpScoreSceneDataText__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataText final : public SceneBase {
public:
    explicit SceneDataText() = default;
    virtual ~SceneDataText() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    ofTrueTypeFont mFont;
    vector<ofVec2f> mVectors;
    const int kFontSize{82};
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataText__) */
