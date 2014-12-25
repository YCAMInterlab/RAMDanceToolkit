//
//  dpScoreSceneManager.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneManager.h"

dpScoreSceneManager::dpScoreSceneManager() :
mUpdateAll(true),
mSceneId(0)
{
    mScenes.clear();
    
    mTabBar = new ofxUITabBar();
    mTabBar->setPosition(20.f, 40.f);
    mTabBar->setColorBack(ofColor(ofColor::black, 200));
}

dpScoreSceneManager::~dpScoreSceneManager()
{
    if (mTabBar) {
        delete mTabBar;
        mTabBar = NULL;
    }
    
    for (auto& scene : mScenes) scene->shutDown();
}

void dpScoreSceneManager::addScene(dpScoreBase::Ptr scene)
{
    if (mScenes.empty() == false) {
        const string className = dpGetClassName(*scene);
        if (findScene(className) != mScenes.end())
            ofxThrowExceptionf(ofxException,
                               "class %s is already exists",
                               className.c_str());
    }
    scene->initialize();
    
    if (scene->getUICanvas() != NULL) {
        mTabBar->addCanvas(scene->getUICanvas());
    }
    
    scene->setId(mScenes.size());
    mScenes.push_back(scene);
}

void dpScoreSceneManager::next()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    ++mSceneId %= mScenes.size();
    change();
}

void dpScoreSceneManager::prev()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    --mSceneId;
    if (mSceneId<0) mSceneId = mScenes.size()-1;
    change();
}

void dpScoreSceneManager::change(int i)
{
    if (i<0 || i>=mScenes.size())
        ofxThrowExceptionf(ofxException, "%d is out of range", i);
    mSceneId = i;
    change();
}

void dpScoreSceneManager::change()
{
    if (mCurrentScene) mCurrentScene->exit();
    mCurrentScene = mScenes.at(mSceneId);
    mCurrentScene->enter();
    
    ofLogNotice() << "changed scene: " << mSceneId << ", " << dpGetClassName(*mCurrentScene);
}

void dpScoreSceneManager::update(ofxEventMessage& m)
{
    if (mUpdateAll) {
        for (auto& scene : mScenes) scene->update(m);
    }
    else if (mCurrentScene) {
        mCurrentScene->update(m);
    }
}

void dpScoreSceneManager::draw()
{
    if (mCurrentScene) mCurrentScene->draw();
}

ofPtr<dpScoreBase> dpScoreSceneManager::getCurrent()
{
    return mCurrentScene;
}

void dpScoreSceneManager::setUpdateAll(bool update)
{
    mUpdateAll = update;
}

dpScoreSceneManager::SceneVec::iterator
dpScoreSceneManager::findScene(const string& name)
{
    auto comarator = [&](const dpScoreBase::Ptr& rhs)
    {
        return name == dpGetClassName(*rhs);
    };
    
    return find_if(mScenes.begin(), mScenes.end(), comarator);
}
