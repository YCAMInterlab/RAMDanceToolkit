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
class FlowChartRDTK;

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
    
    template<class T> ofPtr<T>  getNode();
    template<class T> bool      isCurrentNode() const;
    template<class T> bool      isNextNode() const;
    
    SkeletonVec&                getCopiedSkeletons();
    
    void incrementNode();
    void sendLightingOsc(const string& s, float f);
    
    ofTrueTypeFont&     getFont();
    ofTrueTypeFont&     getFontSmall();
    ofxTrueTypeFontUC&  getFontJP();
    
    const float kWidth  {1920.f};
    const float kHeight {1200.f};
    
private:
    void setupNodes();
    void setupNodeOrders();
    void setupScenes();
    
    void updateGlobalTime();
    void copySkeletons();
    
    void drawScene();
    void drawToolKit();
    void drawNodes();
    void drawDancers();
    void drawLines();
    void drawCircles();
    void debugDrawCameras();
    void drawHUD();
    
    template<class T> ofPtr<T>  addNode();
    template<class T> ofPtr<T>  addScene();
    const string&               getCurrentSceneName() const;
    ofPtr<FlowChartBaseScene>   getCurrentScene();
    template<class T> void      changeScene();
    void                        changeScene(const string& name);
    template<class T> bool      isCurrentScene() const;
    
    ofPtr<FlowChartRDTK>    mRDTK;
	ofTrueTypeFont          mFont, mFontSmall;
    ofxTrueTypeFontUC       mFontJP;
    ofxOscSender            mOscSender;
    ofCamera                mCamToolKit;
    
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

template<class T> ofPtr<T> SceneFlowChart::addNode()
{
    auto p = ofPtr<T>(new T());
    mNodes[getClassName<T>()] = p;
    return p;
}

template<class T> bool SceneFlowChart::isCurrentNode() const
{
    return getCurrentNodeName() == getClassName<T>();
}

template<class T> bool SceneFlowChart::isNextNode() const
{
    return getNextNodeName() == getClassName<T>();
}

template<class T> ofPtr<T> SceneFlowChart::addScene()
{
    auto p = ofPtr<T>(new T());
    mScenes[getClassName<T>()] = p;
    return p;
}

 template<class T> void SceneFlowChart::changeScene()
{
    changeScene(getClassName<T>());
}

template<class T> bool SceneFlowChart::isCurrentScene() const
{
    return getCurrentSceneName() == getClassName<T>();
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreSceneFlowChart_h */
