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
    struct Node : public ofxMot::Node {
        Node();
        virtual ~Node() {}
        Node& operator = (const Node& rhs) { return *this = rhs; }
        
        void setupPoints();
        void setupLines();
        
        void update();
        void draw();
        
        vector<float> spd;
        vector<ofVec3f> axis;
        vector<ofVec3f> vertices;
        vector<ofVec3f> initialVertices;
        
        //const int kNumVertices = 10*23;
        const int kNumVertices = 20;
        ofVbo vbo;
        
        vector<ofVec3f> verticesLines;
        ofVbo vboLines;
        float scale = 15.f;
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
    ofxMot::SkeletonPtr mSkeleton;
    ofEasyCam mCam;
    float mScale = 300.f;
    vector<Node> mNodes;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyLines__) */
