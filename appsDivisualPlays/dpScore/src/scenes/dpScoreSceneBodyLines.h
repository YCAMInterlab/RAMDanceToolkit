//
//  dpScoreSceneBodyLines.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/11/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyLines__
#define __dpScore__dpScoreSceneBodyLines__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyLines final : public SceneBase {
public:
    struct Node final : public ofxMot::Node {
        Node();
        virtual ~Node() = default;
        Node& operator = (const Node& rhs) { return *this = rhs; }
        
        void setupPoints();
        void setupLines();
        
        void updatePoints();
        void updateLines();
        void customDraw() final override;
        void drawLines();
        
        vector<float> spd;
        vector<ofVec3f> axis;
        vector<ofVec3f> vertices;
        vector<ofVec3f> initialVertices;
        
        //const int kNumVertices = 10*23;
        const int kNumVertices{30};
        ofVbo vbo;
        
        vector<ofVec3f> verticesLines;
        vector<ofFloatColor> verticesColors;
        ofVbo vboLines;
        float scale{15.f};
    };
    
    
    explicit SceneBodyLines() = default;
    virtual ~SceneBodyLines() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    string mSkeletonName;
    ofEasyCam mCam;
    float mScale{20.f};
    ofxMot::SkeletonBase<Node>::Ptr mSkeleton;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyLines__) */
