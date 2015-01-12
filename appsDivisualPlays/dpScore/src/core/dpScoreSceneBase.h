//
//  dpScoreSceneBase.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneBase__
#define __dpScore__SceneBase__

#include "dpScoreCommon.h"
#include "ofxUI.h"
#include "dpEasing.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBase {
public:
    typedef ofPtr<SceneBase> Ptr;
    
   explicit SceneBase() = default;
    virtual ~SceneBase() = default;
    
    virtual void update(ofxEventMessage& m) {}
    virtual void draw() {}
    
    virtual void initialize() {}
    virtual void shutDown() {}
    
    virtual void enter() {}
    virtual void exit() {}
    
    virtual void keyPressed(int key) {}
    virtual void keyReleased(int key) {}
    virtual void mouseMoved(int x, int y) {}
    virtual void mouseDragged(int x, int y, int button) {}
    virtual void mousePressed(int x, int y, int button) {}
    virtual void mouseReleased(int x, int y, int button) {}
    virtual void windowResized(int w, int h) {}
    virtual void dragEvent(ofDragInfo dragInfo) {}
    virtual void gotMessage(ofMessage msg) {}
    
    void guiEvent(ofxUIEventArgs &e) {}
    
    void drawHeader();
    
    // default value is nullptr
    ofxUICanvas* const getUICanvas() { return mUICanvas; }
    const ofxUICanvas* const getUICanvas() const { return mUICanvas; }
    
    // we can change the id only once
     void setId(int id);
    // we can change the name only once and can't change it after called getName()
    void setName(const string& name);
    void setEaseFunc(easeFunc func) { mEaseFunc = func; }
    
    int getId() const { return mId; }
    // return class name if we've not called setName()
    const string& getName();
    const easeFunc getEaseFunc() const { return mEaseFunc; }
    
protected:
    ofxUICanvas* mUICanvas{nullptr};
    string mName{""};
    int mId{-1};
    easeFunc mEaseFunc = easeNone;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneBase__) */
