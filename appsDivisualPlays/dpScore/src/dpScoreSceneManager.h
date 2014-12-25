//
//  dpScoreSceneManager.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreSceneManager__
#define __dpScore__dpScoreSceneManager__

#include "dpScoreBase.h"

class dpScoreSceneManager final {
public:
    typedef vector<dpScoreBase::Ptr> SceneVec;
    
    dpScoreSceneManager();
    ~dpScoreSceneManager();
    
    void addScene(dpScoreBase::Ptr scene);
    void next();
    void prev();
    void change(int i);
    template <class SceneClass> void change();

    void update(ofxEventMessage& m);
    void draw();
    
    dpScoreBase::Ptr getCurrent();
    
    void setUpdateAll(bool update);
    
    SceneVec::iterator findScene(const string& name);
    
    ofxUITabBar* getTabBar() { return mTabBar; }
    
private:
    void change();
    
    ofxUITabBar* mTabBar;

    SceneVec mScenes;
    dpScoreBase::Ptr mCurrentScene;
    
    int mSceneId;
    
    bool mUpdateAll;

};

template <class SceneClass>
void dpScoreSceneManager::change()
{
    const string className = dpGetClassName<SceneClass>();
    auto it = findScene(className);
    if (it == mScenes.end())
        ofxThrowExceptionf(ofxException,
                           "class %s is not exists",
                           className.c_str());
    mSceneId = (*it)->getId();
    change();
}

#endif /* defined(__dpScore__dpScoreSceneManager__) */
