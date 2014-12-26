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

DP_SCORE_NAMESPACE_BEGIN

class SceneBase {
public:
    typedef ofPtr<SceneBase> Ptr;
    
    SceneBase();
    virtual ~SceneBase() {}
    
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
    
    ofxUICanvas* getUICanvas() { return mUICanvas; }
    
    int getId() const { return mId; }
    void setId(int id) { mId = id; }
    
    void setName(const string& name);
    const string& getName();
    
protected:
    ofxUICanvas* mUICanvas;
    string mName;
    int mId;
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneBase__) */
