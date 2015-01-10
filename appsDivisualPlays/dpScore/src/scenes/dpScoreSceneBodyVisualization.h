//
//  dpScoreSceneBodyVisualization.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyVisualization__
#define __dpScore__dpScoreSceneBodyVisualization__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyVisualization final : public SceneBase {
public:
    struct Node : public ofxMot::Node {
        typedef ofPtr<Node> Ptr;
        Node();
        virtual ~Node() {}
        void update();
        virtual void customDraw();
        void drawPoints();
        
        vector<ofVec3f> vertices;
        deque<ofVec3f> points;
        ofVbo vbo;
        
        ofVec3f dir;
        float scale = 300;
        const int kMaxPoints = 2000;
    };
    
    typedef vector<Node::Ptr> NodeVec;
    
    explicit SceneBodyVisualization() = default;
    virtual ~SceneBodyVisualization() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    string mSleletonName;
    ofEasyCam mCam;
    NodeVec mNodes;
    bool mMagnify = false;
    float mScale = 300.f;
    float mRotSpdX = 2.12f;
    float mRotSpdY = 4.35f;
    int mJointId = 0;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyVisualization__) */
