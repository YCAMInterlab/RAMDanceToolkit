//
//  dpScoreSceneBodyVisualization.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#include "dpScoreSceneBodyVisualization.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneBodyVisualization::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
}

void SceneBodyVisualization::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyVisualization::enter()
{
    dpDebugFunc();

    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyVisualization::onDrawSkeleton);
    ofAddListener(ofxMotioner::drawSkeletonEvent,
                  this,
                  &SceneBodyVisualization::onDrawSkeleton);
}

void SceneBodyVisualization::exit()
{
    dpDebugFunc();

    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyVisualization::onDrawSkeleton);
    ofRemoveListener(ofxMotioner::drawSkeletonEvent,
                  this,
                  &SceneBodyVisualization::onDrawSkeleton);
}

void SceneBodyVisualization::update(ofxEventMessage& m)
{

}

void SceneBodyVisualization::draw()
{
    mCam.begin();
    ofxMotioner::draw();
    ofxMotioner::debugDraw();
    mCam.end();
}

#pragma mark ___________________________________________________________________
void SceneBodyVisualization::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    auto joints = skl->getJoints();
    decltype(joints) nodes(joints.size());
    
}

void SceneBodyVisualization::onDrawSkeleton(ofxMotioner::EventArgs &e)
{
    
}

DP_SCORE_NAMESPACE_END