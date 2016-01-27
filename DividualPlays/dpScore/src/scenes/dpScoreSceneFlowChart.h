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
#include "dpScoreBaseNode.h"
#include "dpScoreToolBox.h"
#include "ofxMotioner.h"
#include "ofxTrueTypeFontUC.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeSkeleton : public ofxMot::Node {
public:
    void customDraw() override
    {
        ofDrawBox(size);
    }
};

class SceneFlowChart final : public SceneBodyBase<NodeSkeleton> {
public:
	enum NodeType {
		NODE_MOTIONER = 0,
        NODE_DANCER,
		NODE_MASTER_HAKONIWA,
		NODE_STAGE,
		NODE_HAKONIWA,
		NODE_CAMERA_UNIT,
		NODE_DISPLAY,
		NODE_SPEAKER,
		NODE_LIGHT,
		NODE_COMPUTER,
		NODE_AUDIENCE,
		NUM_NODES,
	};
    
    enum Scene {
        SCENE_MOVE,
        SCENE_MAIN,
        SCENE_TPS,
        SCENE_CIRCULATION,
        SCENE_MEMORY,
        SCENE_DEBUG,
        NUM_SCENES,
    };

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
    void windowResized(int w, int h) override;

private:
    void setupNodes();
    void setupOrders();
    void setupScenes();
    
    void updateTime();
    void updateWithSkeleton();
    void updateMovingCam();
    void updateMainCam();
    void updateMemoryCam();
    
    void drawScene();
    void drawToolKit();
    void drawNodes();
    void drawDancers();
    void drawLines();
    void debugDrawCameras();
    void drawHUD();
    
    void changeScene(int index);
    int getCurrentNodeID() const;
    int getNextNodeID() const;
    
    template <class T> ofPtr<T> getNode();
    
    struct Property {
        Property(float move, float idle, float line, float totalTime, bool easeyCam = false);
        ofPtr<ofCamera> camera {makeShared<ofCamera>()};
        float moveSpan {2.f};
        float idleSpan {2.f};
        float lineSpan {1.f};
        float totalTime {60.f};
    };
    
    const float kMainCamSpeed {0.025f};
    const float kYOffset {-200.f};
    
    ofFbo mFbo[2]; // double buffer
    int mCurrentFbo {0};
	ofTrueTypeFont mFont, mFontSmall;
    ofxTrueTypeFontUC mFontJP;
	vector<ofPtr<BaseNode> > mNodes;
    vector<vector<int>> mOrders;
    vector<Property> mProperties;
    ofCamera mCamToolKit;
    ofNode mNodeCenter;
    ofNode mNodeHead;
    int mCurrentScene {SCENE_MOVE};
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
    for (auto n : mNodes) {
        if (getClassName(*n) == getClassName<T>())
        return dynamic_pointer_cast<T>(n);
    }
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreSceneFlowChart_h */
