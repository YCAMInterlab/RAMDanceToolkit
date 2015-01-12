//
//  dpScoreSceneBodyScan.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyScan__
#define __dpScore__dpScoreSceneBodyScan__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyScan final : public SceneBase {
public:
    struct Node {
        typedef ofPtr<Node> Ptr;
        Node();
        virtual ~Node() {}
        
        void update();
        void draw();
        
        vector<float> spd;
        vector<ofVec3f> axis;
        vector<ofVec3f> vertices;
        vector<ofVec3f> initialVertices;
        
        const int kNumVertices{10};
        ofVbo vbo;
        float scale{20.f};
    };
    
    
    explicit SceneBodyScan() = default;
    virtual ~SceneBodyScan() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    string mSkeletonName;
    ofxMot::SkeletonPtr mSkeleton;
    ofEasyCam mCam;
    float mScale{300.f};
    vector<Node::Ptr> mNodes;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyScan__) */
