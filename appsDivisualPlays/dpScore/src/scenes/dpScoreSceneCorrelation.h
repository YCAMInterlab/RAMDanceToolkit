//
//  dpScoreSceneCorrelation.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/15.
//
//

#ifndef __dpScore__dpScoreSceneCorrelation__
#define __dpScore__dpScoreSceneCorrelation__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneCorrelation final : public SceneBase {
public:
    struct Node : public ofNode {
        Node(SceneCorrelation* _owner);
        virtual ~Node() = default;
        virtual void update() = 0;
        void customDraw() override;
        void drawName();
        
        SceneCorrelation* owner{nullptr};
        string name;
    };
    
    struct NodeVoid : public Node {
        NodeVoid(SceneCorrelation* _owner) :
        Node(_owner)
        {
        }
        virtual ~NodeVoid() {}
        void update() override {}
        void customDraw() override {}
    };
    
    struct NodeDancer : public SceneCorrelation::Node {
        NodeDancer(SceneCorrelation* _owner);
        virtual ~NodeDancer() {}
        void update() override;
        void customDraw() override;
        
        
        static const int kNumOctave{3};
        vector<ofVec3f> noiseVec{kNumOctave};
        ofVec3f rotAxis;
    };
    
    struct NodeRDTK : public NodeDancer {
        NodeRDTK(SceneCorrelation* _owner);
        virtual ~NodeRDTK() {}
        void update() override;
        void customDraw() override;
    };
    
    struct NodeHakoniwa : public NodeRDTK {
        NodeHakoniwa(SceneCorrelation* _owner);
        virtual ~NodeHakoniwa() {}
        void update() override;
        void customDraw() override;
        
        const float PointSize{30.f};
        
        struct Point {
            ofVec3f pos;
            ofVec3f spd;
        };
        vector<Point> points;
    };
    
    struct NodeAnalyze : public NodeHakoniwa {
        NodeAnalyze(SceneCorrelation* _owner);
        virtual ~NodeAnalyze() {}
        void update() override;
        void customDraw() override;
    };
    
    struct NodeSound : public NodeAnalyze {
        NodeSound(SceneCorrelation* _owner);
        virtual ~NodeSound() {}
        void update() override;
        void customDraw() override;
    };
    
    struct NodeLight : public NodeAnalyze {
        NodeLight(SceneCorrelation* _owner);
        virtual ~NodeLight() {}
        void update() override;
        void customDraw() override;
        
        float prevT{0.f};
        ofQuaternion fixedQuat;
    };
    
    struct NodeHakoVis : public NodeAnalyze {
        NodeHakoVis(SceneCorrelation* _owner);
        virtual ~NodeHakoVis() {}
        void update() override;
        void customDraw() override;
        
        ofTexture tex;
        const int dim{4};
    };
    
    struct NodeScore : public NodeAnalyze {
        NodeScore(SceneCorrelation* _owner);
        virtual ~NodeScore() {}
        void update() override;
        void customDraw() override;
        
        vector<ofVec3f> points;
        ofVbo vbo;
        const int numPoints{100};
    };
    
    explicit SceneCorrelation() = default;
    virtual ~SceneCorrelation() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    float getTime() const { return mTime; }
    
    ofTrueTypeFont& getFont() { return mFont; }
    ofTrueTypeFont& getFontInitial() { return mFontInitial; }
    
private:
    typedef multimap<string, Node*> NodeMap;
    typedef pair<string, Node*> NodePair;
    
    void deleteNodes();
    
    NodeMap mNodes;
    
    ofEasyCam mCam;
    
    ofTrueTypeFont mFontInitial;
    ofTrueTypeFont mFont;
    
    float mEnterTime{0.f};
    float mTime{0.f};
    
    float mPrevT{0.f};
    int mCurrentTarget{0};
    int mPrevTarget{-1};
    
    ofVec3f mLookAt;
    
    Node* mFrom{nullptr};
    Node* mTo{nullptr};
    
    Node* mCurrentDancer{nullptr};
    Node* mCurrentHakoniwa{nullptr};
    Node* mCurrentOutput{nullptr};
    
    bool mMoveCamera{true};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneCorrelation__) */
