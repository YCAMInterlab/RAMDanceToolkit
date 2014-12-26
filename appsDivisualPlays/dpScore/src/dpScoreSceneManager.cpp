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
    mTabBar->addToggle("Update All", &mUpdateAll);
    mTabBar->addSpacer();
}

dpScoreSceneManager::~dpScoreSceneManager()
{
    if (mTabBar) {
        delete mTabBar;
        mTabBar = NULL;
    }
    
    for (auto& scene : mScenes) scene->shutDown();
}

void dpScoreSceneManager::add(dpScoreBase::Ptr scene)
{
    if (mScenes.empty() == false) {
        const string className = scene->getName();
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

void dpScoreSceneManager::keyPressed(int key)
{
    if (mCurrentScene) mCurrentScene->keyPressed(key);
}

void dpScoreSceneManager::keyReleased(int key)
{
    if (mCurrentScene) mCurrentScene->keyReleased(key);
}

void dpScoreSceneManager::mouseMoved(int x, int y)
{
    if (mCurrentScene) mCurrentScene->mouseMoved(x, y);
}

void dpScoreSceneManager::mouseDragged(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mouseDragged(x, y, button);
}

void dpScoreSceneManager::mousePressed(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mousePressed(x, y, button);
}

void dpScoreSceneManager::mouseReleased(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mouseReleased(x, y, button);
}

void dpScoreSceneManager::windowResized(int w, int h)
{
    if (mCurrentScene) mCurrentScene->windowResized(w, h);
}

void dpScoreSceneManager::dragEvent(ofDragInfo dragInfo)
{
    if (mCurrentScene) mCurrentScene->dragEvent(dragInfo);
}

void dpScoreSceneManager::gotMessage(ofMessage msg)
{
    if (mCurrentScene) mCurrentScene->gotMessage(msg);
}

void dpScoreSceneManager::guiEvent(ofxUIEventArgs &e)
{
    
}

dpScoreSceneManager::SceneVec::iterator
dpScoreSceneManager::findScene(const string& name)
{
    return find_if(mScenes.begin(),
                   mScenes.end(),
                   [&](const dpScoreBase::Ptr& rhs)
                   {
                       return name == rhs->getName();
                   });
}
