//
//  dpScoreAnalyzePixelate.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/15.
//
//

#ifndef __dpScore__dpScoreAnalyzePixelate__
#define __dpScore__dpScoreAnalyzePixelate__

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class AnalyzePixelate {
public:
    AnalyzePixelate();
    
    enum class Color { R, G, B, Num, };
    void updateColor(Color color, ofxEventMessage& m);
    void update();
    void draw();
    
    void checkData();
    
    int mPixelW{0};
    int mPixelH{0};
    
    vector<vector<bool>> mPixels;
    vector<bool> mPixelsInited;
    
    int mNumR{0};
    int mNumG{0};
    int mNumB{0};
    
    floatInt mDiffR{0.f};
    floatInt mDiffG{0.f};
    floatInt mDiffB{0.f};
    
    float mPrevSetSceneTime{-60.f};
    float mMinSetSceneTime{120.f};
    float mLimit{30.f};
    floatInt mTotalDiff{0.f};
    
    int mWhich{0};
    bool mWin0{false};
    bool mWin1{false};
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreAnalyzePixelate__) */
