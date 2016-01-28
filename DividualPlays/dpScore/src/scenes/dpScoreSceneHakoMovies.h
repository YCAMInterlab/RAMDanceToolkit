//
//  dpScoreSceneHakoMovies.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#ifndef dpScoreSceneHakoMovies_h
#define dpScoreSceneHakoMovies_h

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneHakoMovies final : public SceneBase {
public:
    explicit SceneHakoMovies() = default;
    virtual ~SceneHakoMovies() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void keyPressed(int key) override;
    
private:
    enum MovieType {
        MOVIE_NAMA,
        MOVIE_VIS,
        NUM_MOVIES,
    };
    enum DisplayType {
        DISPLAY_NAMA,
        DISPLAY_VIS,
        DISPLAY_MIX,
        NUM_DISPLAY_TYPES,
    };
    
    const float kFadeSpeed {0.5f};
    const float kFadeDur {20.f};
    
    ofTrueTypeFont mFont;
    
    vector<ofPtr<ofVideoPlayer>> mMovies;
    long mLastFrame {0};
    float mAlphaNama {1.f};
    float mAlphaVis {0.f};
    int mDisplayType {DISPLAY_NAMA};
    float mElapsedTime;
    
};

DP_SCORE_NAMESPACE_END


#endif /* dpScoreSceneHakoMovies_h */
