#include "ofApp.h"
#include "dpScoreVec2SimpleGraph.h"
#include "dpScoreVec2Clocks.h"
#include "dpScoreVec2Grid.h"
#include "dpScoreVec2Plotter.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    OFX_BEGIN_EXCEPTION_HANDLING
    
    auto vec2Simple = dpScoreBase::Ptr(new dpScoreVec2SimpleGraph());
    auto vec2Clocks = dpScoreBase::Ptr(new dpScoreVec2Clocks());
    auto vec2Grid = dpScoreBase::Ptr(new dpScoreVec2Grid());
    auto vec2Plotter = dpScoreBase::Ptr(new dpScoreVec2Plotter());
    
    mSceneManager.addScene(vec2Simple);
    mSceneManager.addScene(vec2Clocks);
    mSceneManager.addScene(vec2Grid);
    mSceneManager.addScene(vec2Plotter);

    mSceneManager.change<dpScoreVec2SimpleGraph>();
    
    mSceneManager.getTabBar()->loadSettings("settings/", "ui-");
    mSceneManager.getTabBar()->setVisible(false);
    
    OFX_END_EXCEPTION_HANDLING
    
    ofSetFrameRate(30);
    ofBackground(ofColor::black);
    mOscReceiver.setup(10000);
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    int n = 0;
    ofVec2f v;
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiver.getNextMessage(&m);
        
        if (m.getAddress() == "/dp/cameraUnit/pendulum/vector") {
            v.x += m.getArgAsFloat(1);
            v.y += m.getArgAsFloat(2);
            n++;
        }
    }
    if (n>0) {
        v /= (float)n;
        
        ofxEventMessage m;
        m.setAddress(dpScoreMessageVec2);
        m.addFloatArg(v.x);
        m.addFloatArg(v.y);
        mSceneManager.update(m);
    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    mSceneManager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key=='f') {
        ofToggleFullscreen();
    }
    else if (key=='g') {
        mSceneManager.getTabBar()->toggleVisible();
    }
    else if (key=='s') {
        mSceneManager.getTabBar()->saveSettings("settings/", "ui-");
    }
    else if (key==OF_KEY_LEFT) {
        mSceneManager.prev();
    }
    else if (key==OF_KEY_RIGHT) {
        mSceneManager.next();
    }
    else {
        for (int i=0; i<9; i++) {
            const char c = ofToChar(ofToString(i+1));
            if (key==c) {
                mSceneManager.change(i);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}


