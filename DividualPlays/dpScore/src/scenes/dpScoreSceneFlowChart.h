//
//  dpScoreSceneFlowChart.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

#ifndef dpScoreSceneFlowChart_h
#define dpScoreSceneFlowChart_h

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxOsc.h"

DP_SCORE_NAMESPACE_BEGIN

class BaseNode;

class NodeSkeleton final : public ofxMot::Node {
public:
    virtual ~NodeSkeleton() {}
    
    void customDraw() override
    {
        ofDrawBox(size);
    }
};

class SceneFlowChart final : public SceneBodyBase<NodeSkeleton> {
public:
	explicit SceneFlowChart() = default;
	virtual ~SceneFlowChart() = default;
	SceneFlowChart(const SceneFlowChart &) = delete;
	SceneFlowChart& operator = (const SceneFlowChart&) = delete;

	void initialize() override;
	void shutDown() override;

	void enter() override;
	void exit() override;

	void update(ofxEventMessage& m) override;
	void draw() override;

	void keyPressed(int key) override;

private:
    void setupNodes();
    void setupOrders();
    void setupScenes();
    
    void updateTime();
    void updateWithSkeleton();
    void updateMoving();
    void updateCirculation();
    void updateDescription();
    void updateMemory();
    
    void drawScene();
    void drawToolKit();
    void drawNodes();
    void drawDancers();
    void drawLines();
    void drawCircles();
    void debugDrawCameras();
    void drawHUD();
    
    void changeScene(int index);
    const string& getCurrentNodeName() const;
    const string& getNextNodeName() const;
    ofPtr<BaseNode> getCurrentNode();
    ofPtr<BaseNode> getNextNode();
    
    template <class T> ofPtr<T> getNode();
    template<class T> void addNode();
    
    enum SceneType {
        SCENE_MOVE = 0,
        SCENE_CIRCULATION,
        SCENE_TPS,
        SCENE_DESCRIPTION,
        SCENE_MEMORY,
        SCENE_DEBUG,
        NUM_SCENES,
    };
    
    struct Property final {
        Property() = default;
        Property(float move, float idle, float line, float totalTime, bool easeyCam = false);
        Property(const Property&) = default;
        Property& operator = (const Property&) = default;
        ~Property() = default;
        
        ofPtr<ofCamera> camera {shared_ptr<ofCamera>(new ofCamera())};
        float moveSpan {2.f};
        float idleSpan {2.f};
        float lineSpan {1.f};
        float totalTime {60.f};
    };
    
    const float kWidth {1920.f};
    const float kHeight {1200.f};
    
    const float kMainCamSpeed {0.025f};
    const int kNumFbos {2};
    
	ofTrueTypeFont mFont, mFontSmall;
    ofxTrueTypeFontUC mFontJP;
    ofxOscSender mOscSender;
    bool mOscInited {false};
    const string kOscAddrLighting {"/dp/light/moving"};
    
	map<string, ofPtr<BaseNode>> mNodes;
    vector<vector<string>> mOrders;
    map<int, Property> mProperties;
    SkeletonVec mSkeletons;
    
    vector<ofFbo> mFbos; // double buffer
    
    ofCamera mCamToolKit;
    
    ofNode mNodeCenter;
    ofNode mNodeHead;
    
    int mCurrentScene {SCENE_MOVE};
    int mCurrentFbo {kNumFbos};
	int mOrderIdx {2};
    int mNodeIdx {0};
    
    float mElapsedTime {0.f};
    long mLastFrameNum {0};
    float mTimeCamMove {0.f};
    float mTimeCamRotation {0.f};
    
    bool mPaused {false};
};

template <class T>
ofPtr<T> SceneFlowChart::getNode()
{
    return dynamic_pointer_cast<T>(mNodes[getClassName<T>()]);
}

template<class T> void SceneFlowChart::addNode()
{
    mNodes[getClassName<T>()] = shared_ptr<T>(new T());
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreSceneFlowChart_h */
