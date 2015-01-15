//
//  dpScoreSceneDataDisplacement.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#include "dpScoreSceneDataDisplacement.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneDataDisplacement::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
}

void SceneDataDisplacement::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataDisplacement::enter()
{
    dpDebugFunc();
}

void SceneDataDisplacement::exit()
{
    dpDebugFunc();
}

void SceneDataDisplacement::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCaneraUnitVector) {

    }
}

void SceneDataDisplacement::draw()
{

}

DP_SCORE_NAMESPACE_END