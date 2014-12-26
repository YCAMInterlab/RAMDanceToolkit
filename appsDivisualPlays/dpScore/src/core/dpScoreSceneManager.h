//
//  dpScoreSceneManager.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneManager__
#define __dpScore__SceneManager__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneManager final {
public:
    typedef vector<SceneBase::Ptr> SceneVec;
    
    SceneManager();
    ~SceneManager();
    
    void add(SceneBase::Ptr scene);
    void next();
    void prev();
    void change(int index);
    void change(const string& name);
    template <class SceneClass> void change();
    
    void update(ofxEventMessage& m);
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void guiEvent(ofxUIEventArgs &e);
    
    SceneBase::Ptr getCurrent();
    
    void setUpdateAll(bool update);
    
    SceneVec::iterator findScene(const string& name);
    
    ofxUITabBar* getTabBar() { return mTabBar; }
    
private:
    void change();
    
    ofxUITabBar* mTabBar;

    SceneVec mScenes;
    SceneBase::Ptr mCurrentScene;
    
    int mSceneId;
    
    bool mUpdateAll;

};

template <class SceneClass>
void SceneManager::change()
{
    const string className = getClassName<SceneClass>();
    auto it = findScene(className);
    if (it == mScenes.end())
        ofxThrowExceptionf(ofxException,
                           "class %s is not exists",
                           className.c_str());
    mSceneId = (*it)->getId();
    change();
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneManager__) */
