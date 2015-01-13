//
//  dpScoreSceneBodyBase.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/13/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyBase__
#define __dpScore__dpScoreSceneBodyBase__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

template<class Node>
class SceneBodyBase : public SceneBase {
public:
    typedef ofxMot::SkeletonBase<Node> Skeleton;
    typedef vector<typename Skeleton::Ptr> SkeletonVec;
    typedef typename Skeleton::Ptr SkeletonPtr;
    
    explicit SceneBodyBase() = default;
    virtual ~SceneBodyBase() = default;
    
    virtual void setupSkeleton(SkeletonPtr skl) {}
    virtual void updateSkeleton(SkeletonPtr skl) {}
    virtual void drawSkeleton(SkeletonPtr skl) {}
    virtual void exitSkeleton(SkeletonPtr skl) {}
    
    typename SkeletonVec::iterator findSkeleton(const string& name);
    
protected:
    SkeletonPtr getSkeleton(int i) { return mSkeletons.at(i); }
    SkeletonVec getSkeletons() { return mSkeletons(); }
    size_t getNumSkeletons() const { return mSkeletons.size(); }
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onSetupSkeleton(ofxMotioner::EventArgs &e);
    virtual void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    virtual void onDrawSkeleton(ofxMotioner::EventArgs &e);
    virtual void onExitSkeleton(ofxMotioner::EventArgs &e);
    
    SkeletonVec mSkeletons;
};

#pragma mark ___________________________________________________________________
template<class Node>
void SceneBodyBase<Node>::onEnter()
{
    mSkeletons.clear();
    
    enter();
    
    ofAddListener(ofxMotioner::setupSkeletonEvent, this, &SceneBodyBase::onSetupSkeleton);
    ofAddListener(ofxMotioner::updateSkeletonEvent, this, &SceneBodyBase::onUpdateSkeleton);
    ofAddListener(ofxMotioner::drawSkeletonEvent, this, &SceneBodyBase::onDrawSkeleton);
    ofAddListener(ofxMotioner::exitSkeletonEvent, this, &SceneBodyBase::onExitSkeleton);
}

template<class Node>
void SceneBodyBase<Node>::onExit()
{
    ofAddListener(ofxMotioner::exitSkeletonEvent, this, &SceneBodyBase::onExitSkeleton);
    ofAddListener(ofxMotioner::drawSkeletonEvent, this, &SceneBodyBase::onDrawSkeleton);
    ofAddListener(ofxMotioner::updateSkeletonEvent, this, &SceneBodyBase::onUpdateSkeleton);
    ofAddListener(ofxMotioner::setupSkeletonEvent, this, &SceneBodyBase::onSetupSkeleton);
    
    exit();
    
    for (auto s : mSkeletons) {
        exitSkeleton(s);
    }
    mSkeletons.clear();
}

#pragma mark ___________________________________________________________________

template<class Node>
typename SceneBodyBase<Node>::SkeletonVec::iterator
SceneBodyBase<Node>::findSkeleton(const string& name)
{
    return find_if(mSkeletons.begin(),
                   mSkeletons.end(),
                   [&name](const SkeletonPtr& s)
                   { return s->getName() == name; });
}

template<class Node>
void SceneBodyBase<Node>::onSetupSkeleton(ofxMotioner::EventArgs &e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    const string& name = e.skeleton->getName();
    
    if (findSkeleton(name) == mSkeletons.end()) {
        auto newSkeleton = Skeleton::create();
        newSkeleton->setName(name);
        setupSkeleton(newSkeleton);
        mSkeletons.push_back(newSkeleton);
    }
    OFX_END_EXCEPTION_HANDLING
}

template<class Node>
void SceneBodyBase<Node>::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    auto it = findSkeleton(e.skeleton->getName());
    if (it != mSkeletons.end()) {
        (*it)->copyMatrices(e.skeleton);
        updateSkeleton(*it);
    }
    else {
        onSetupSkeleton(e);
    }
    OFX_END_EXCEPTION_HANDLING
}

template<class Node>
void SceneBodyBase<Node>::onDrawSkeleton(ofxMotioner::EventArgs &e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    auto it = findSkeleton(e.skeleton->getName());
    
    if (it != mSkeletons.end()) {
        drawSkeleton(*it);
    }
    else {
        onSetupSkeleton(e);
    }
    OFX_END_EXCEPTION_HANDLING
}

template<class Node>
void SceneBodyBase<Node>::onExitSkeleton(ofxMotioner::EventArgs &e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    auto it = findSkeleton(e.skeleton->getName());
    if (it != mSkeletons.end()) {
        exitSkeleton(*it);
        mSkeletons.erase(it);
    }
    OFX_END_EXCEPTION_HANDLING
}

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyBase__) */
