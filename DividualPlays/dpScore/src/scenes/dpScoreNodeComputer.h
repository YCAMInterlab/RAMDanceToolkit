//
//  dpScoreNodeComputer.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeComputer_h
#define dpScoreNodeComputer_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeComputer: public BaseNode {
public:
    NodeComputer();
    virtual ~NodeComputer();
    
    void customDraw() override;
    
    constexpr static const int kNumDecks {3};
    constexpr static const int kNumMacBooks {8};
    constexpr static const float kMaxMacBookAngle {110.f};
    
    static ofVec3f getMacBookPosition(int index);
    
    string descriptionJPFromHakoniwa;
    
private:
    vector<Deck> mDecks;
    vector<MacBook> mMBPs;
    ofFbo mFbo;
    float mTime{0.f};
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeComputer_h */
