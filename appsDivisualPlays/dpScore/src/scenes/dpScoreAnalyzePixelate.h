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
    void draw();
    
    void checkData();
    
    int mPixelW{0};
    int mPixelH{0};
    
    vector<vector<bool>> mPixels;
    vector<bool> mPixelsInited;
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreAnalyzePixelate__) */
