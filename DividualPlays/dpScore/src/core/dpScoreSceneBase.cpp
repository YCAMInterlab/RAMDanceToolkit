//
//  dpScoreSceneBase.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneBase::drawHeader()
{
#ifdef DP_MASTER_HAKONIWA
    const string title{"Dividual Plays"};
#else
    const string title{"Dividual Plays Score"};
#endif
    string name = getName();
    ofStringReplace(name, "dp::score::Scene", "");
    ofPushStyle();
    ofSetColor(color::kMain, 255);
    ofDrawBitmapString(title, 12.f, 16.f);
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(name, 12.f + (title.length()-1) * 9.f, 16.f);
    ofPopStyle();
}

void SceneBase::setId(int id)
{
    if (mId == -1)
        mId = id;
    else
        ofxThrowException(ofxException, "mId was already set by another process");
}

void SceneBase::setName(const string& name)
{
    if (mName == "")
        mName = name;
    else
        ofxThrowException(ofxException, "couldn't change name bacause it's already refered by another process");
}

const string& SceneBase::getName()
{
    if (mName == "")
        mName = getClassName(*this);
    return mName;
}

string SceneBase::getShortName()
{
    string name{mName};
    ofStringReplace(name, "dp::score::Scene", "");
    return name;
}

void SceneBase::onUpdate(ofxEventMessage& m)
{
    update(m);
}

void SceneBase::onDraw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofNoFill();
 
    draw();
    if (mDrawHeader) drawHeader();
    
    ofPopMatrix();
    ofPopStyle();
}

void SceneBase::onInitialize()
{
    initialize();
}

void SceneBase::onShutDown()
{
    shutDown();
}

void SceneBase::onEnter()
{
    enter();
}

void SceneBase::onExit()
{
    exit();
}

void SceneBase::onKeyPressed(int key)
{
    keyPressed(key);
}

void SceneBase::onKeyReleased(int key)
{
    keyReleased(key);
}

void SceneBase::onMouseMoved(int x, int y)
{
    mouseMoved(x, y);
}

void SceneBase::onMouseDragged(int x, int y, int button)
{
    mouseDragged(x, y, button);
}

void SceneBase::onMousePressed(int x, int y, int button)
{
    mousePressed(x, y, button);
}

void SceneBase::onMouseReleased(int x, int y, int button)
{
    mouseReleased(x, y, button);
}

void SceneBase::onWindowResized(int w, int h)
{
    windowResized(w, h);
}

void SceneBase::onDragEvent(ofDragInfo dragInfo)
{
    dragEvent(dragInfo);
}

void SceneBase::onGotMessage(ofMessage msg)
{
    gotMessage(msg);
}

void SceneBase::onGuiEvent(ofxUIEventArgs &e)
{
    guiEvent(e);
}

DP_SCORE_NAMESPACE_END