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
#include "dpScoreFlowChartBaseScene.h"

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
    friend class FlowChartBaseScene;
public:
    typedef map<string, ofPtr<BaseNode>> NodeMap;
    typedef map<string, ofPtr<FlowChartBaseScene>> SceneMap;
    
	explicit SceneFlowChart()                           = default;
	virtual ~SceneFlowChart()                           = default;
	SceneFlowChart(const SceneFlowChart &)              = delete;
	SceneFlowChart& operator = (const SceneFlowChart&)  = delete;

	void initialize()               override;
	void shutDown()                 override;
	void enter()                    override;
	void exit()                     override;
	void update(ofxEventMessage& m) override;
	void draw()                     override;
	void keyPressed(int key)        override;

    NodeMap&        getNodes();
    const NodeMap&  getNodes() const;
    const string&   getCurrentNodeName() const;
    const string&   getNextNodeName() const;
    ofPtr<BaseNode> getCurrentNode();
    ofPtr<BaseNode> getNextNode();
    
    template<class T> ofPtr<T> getNode();
    
    void incrementNode();
    
    ofTrueTypeFont&     getFont();
    ofTrueTypeFont&     getFontSmall();
    ofxTrueTypeFontUC&  getFontJP();
    
    const float kWidth  {1920.f};
    const float kHeight {1200.f};
    
private:
    void setupNodes();
    void setupNodeOrders();
    void setupScenes();
    
    void updateTime();
    void updateWithSkeleton();
    
    void drawScene();
    void drawToolKit();
    void drawNodes();
    void drawDancers();
    void drawLines();
    void drawCircles();
    void debugDrawCameras();
    void drawHUD();
    
    template<class T> void      addNode();
    
    const string&               getCurrentSceneName() const;
    ofPtr<FlowChartBaseScene>   getCurrentScene();
    template<class T> void      changeScene();
    void                        changeScene(const string& name);
    template<class T>           shared_ptr<T> addScene();
    
	ofTrueTypeFont      mFont, mFontSmall;
    ofxTrueTypeFontUC   mFontJP;
    ofxOscSender        mOscSender;
    ofCamera            mCamToolKit;
    
	NodeMap                 mNodes;
    vector<vector<string>>  mNodeOrders;
    SceneMap                mScenes;
    vector<string>          mSceneOrders;
    SkeletonVec             mSkeletons;
    vector<ofFbo>           mFbos; // double buffer
    
    
    const string    kOscAddrLighting    {"/dp/light/moving"};
    const int       kNumFbos            {2};
    int             mCurrentFbo         {kNumFbos};
	int             mOrderIdx           {2};
    int             mNodeIdx            {0};
    int             mSceneIdx           {0};
    
    float           mElapsedTime        {0.f};
    long            mLastFrameNum       {0};
    
    bool            mOscInited          {false};
    bool            mPaused             {false};
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

template<class T> shared_ptr<T> SceneFlowChart::addScene()
{
    auto p = shared_ptr<T>(new T());
    mScenes[getClassName<T>()] = p;
    return p;
}

 template<class T> void SceneFlowChart::changeScene()
{
    changeScene(getClassName<T>());
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreSceneFlowChart_h */
