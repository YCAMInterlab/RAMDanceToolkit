#include "ofApp.h"
#include "dpScoreVec2SimpleGraph.h"
#include "dpScoreVec2Clocks.h"
#include "dpScoreVec2Grid.h"
#include "dpScoreVec2Plotter.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(dpScoreFrameRate);
    ofBackground(ofColor::black);
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
    
    mSceneManager.getTabBar()->loadSettings(dpScoreSettingsDir,
                                            dpScoreSettingsPrefix);
    mSceneManager.getTabBar()->setVisible(false);
    
    mOscReceiver.setup(dpScoreOscClientPort);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    OFX_BEGIN_EXCEPTION_HANDLING
    
    int n = 0;
    ofVec2f v;
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiver.getNextMessage(&m);
        
        if (m.getAddress() == dpOscAddrCameraUnitPendulumVector) {
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
    
    OFX_END_EXCEPTION_HANDLING
}
//--------------------------------------------------------------
void ofApp::draw()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.draw();
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            mSceneManager.getTabBar()->toggleVisible();
            break;
        case 's':
            mSceneManager.getTabBar()->saveSettings(dpScoreSettingsDir,
                                                    dpScoreSettingsPrefix);
            break;
        case OF_KEY_LEFT:
            mSceneManager.prev();
            break;
        case OF_KEY_RIGHT:
            mSceneManager.next();
            break;
        default:
            mSceneManager.keyPressed(key);
            break;
    }
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.keyReleased(key);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseMoved(x, y);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseDragged(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mousePressed(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseReleased(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.windowResized(w, h);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.gotMessage(msg);
    
    OFX_END_EXCEPTION_HANDLING
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.dragEvent(dragInfo);
    
    OFX_END_EXCEPTION_HANDLING
}