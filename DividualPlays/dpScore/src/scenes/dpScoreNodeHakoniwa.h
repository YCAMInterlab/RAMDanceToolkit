//
//  dpScoreNodeHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeHakoniwa_h
#define dpScoreNodeHakoniwa_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"
#include "dpScoreFlowChartHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeHakoniwa: public BaseNode {
public:
    enum HakoniwaType {
        HAKO_TORNADO,
        HAKO_WORM,
        HAKO_STRUGGLE,
        HAKO_SERVO_PENDULUM,
        HAKO_GEAR,
        HAKO_MAG_PENDULUM,
        HAKO_SAND_STORM,
        NUM_HAKONIWA_TYPES,
    };
    
    NodeHakoniwa();
    virtual ~NodeHakoniwa();
    
    void customDraw() override;
    
    static const int kNumDesks;
    static float getWidth() { return Desk::getWidth();  }
    static float getDepth() { return Desk::getDepth() * kNumDesks;  }

    void setHakoniwa(int i) { mCurrHakoniwa = ofClamp(i, 0, NUM_HAKONIWA_TYPES); };
    void setFocus(bool focus) { mFocus = focus; }
    
private:
    vector<Desk> mDesks;
    bool mFocus {true};
    int mCurrHakoniwa {0};
    
    ofNode mHakoOrigin;
    vector<ofPtr<BaseHakoniwa>> mHakoniwas;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeHakoniwa_h */
