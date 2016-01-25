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
#include "ofxMotioner.h"
#include "ofxTrueTypeFontUC.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeSkeleton : public ofxMot::Node {
public:
    void customDraw() override;
};

class SceneFlowChart final: public SceneBodyBase<NodeSkeleton> {
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
    
    enum CamMode {
        CAM_MOVE,
        CAM_MAIN,
        CAM_TPS,
        CAM_EASY,
        N_CAM_MODE,
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

private:
    void setupNodes();
    void setupOrders();
    
    void drawScene();
    void drawToolKit();
    void drawNodes();
    void drawDancers();
    void drawLines();
    void debugDrawCameras();
    void drawHUD();
    
    void changeCamMode(CamMode m);
    int getCurrentNodeID() const;
    int getNextNodeID() const;
    
    template <class T> ofPtr<T> getNode();
        
	ofTrueTypeFont mFont, mFontSmall;
    ofxTrueTypeFontUC mFontJP;
	vector<ofPtr<BaseNode> > mNodes;
    vector<vector<int>> mOrders;
	ofCamera mCurrentCam;
    ofCamera mCamMain;
    ofCamera mCamToolKit;
    ofCamera mCamTPS;
	ofEasyCam mCamEasy;
    ofNode mCamMainParent;
    ofNode mCamTPSParent;
    CamMode mCamMode {CAM_MOVE};
	int mOrderIdx {2};
    int mNodeIdx {0};
    
    float mElapsedTime {0.f};
    long mLastFrameNum {0};
    float mElapsedTimeMove {0.f};
    float mElapsedTimeMainCam {0.f};
};

template <class T> ofPtr<T> SceneFlowChart::getNode()
{
    for (auto n : mNodes) {
        if (getClassName(*n) == getClassName<T>())
        return dynamic_pointer_cast<T>(n);
    }
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreSceneFlowChart_h */
