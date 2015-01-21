//
//  dpScoreSceneBodyGlobe.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyGlobe__
#define __dpScore__dpScoreSceneBodyGlobe__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyGlobe final : public SceneBodyBase<ofxMot::Node> {
public:
    struct Node : public ofxMot::Node {
        typedef ofPtr<Node> Ptr;
        Node();
        virtual ~Node();
        Node& operator = (const Node& rhs);
        void update();
        virtual void customDraw();
        void drawPoints();
        
        vector<ofVec3f> vertices;
        deque<ofVec3f> points;
        ofVbo vbo;
        
        ofVec3f dir;
        float scale{500};
        const int kMaxPoints{3000};
    };
    
    explicit SceneBodyGlobe() = default;
    virtual ~SceneBodyGlobe() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void setupSkeleton(SkeletonPtr skl);
    void updateSkeleton(SkeletonPtr skl);
    void exitSkeleton(SkeletonPtr skl);
    
private:
    typedef vector<Node::Ptr> NodeVec;
    struct Globe final {
        typedef ofPtr<Globe> Ptr;
        Globe();
        ~Globe() { nodes.clear(); }
        NodeVec nodes;
        ofVec3f origin;
    };
    
    typedef map<string, Globe::Ptr> GlobeMap;
    
    GlobeMap mGlobeMap;
    ofEasyCam mCam;
    bool mMagnify{false};
    float mScale{500.f};
    float mRotSpdX{2.12f};
    float mRotSpdY{4.35f};
    int mJointId{0};
    unsigned long mFrameNum{0};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyGlobe__) */
