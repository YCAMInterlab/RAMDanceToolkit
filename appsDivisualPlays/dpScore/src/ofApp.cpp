#include "ofApp.h"
#include "dpScoreSceneVec2SimpleGraph.h"
#include "dpScoreSceneVec2Clocks.h"
#include "dpScoreSceneVec2Grid.h"
#include "dpScoreSceneVec2Plotter.h"
#include "dpScoreSceneBodyVisualization.h"

using namespace dp::score;

struct ObjA {
    ObjA() { cout << __func__ << endl; }
    ~ObjA() { cout << __func__ << endl; }
    string s = "hoge";
};
struct ObjB {
    ObjB() { cout << __func__ << endl; }
    ~ObjB() { cout << __func__ << endl; }
    string s = "foo";
};
struct ObjC {
    ObjC() { cout << __func__ << endl; }
    ~ObjC() { cout << __func__ << endl; }
    string s = "bar";
};

#pragma mark ___________________________________________________________________
void ofApp::setup()
{
    ofSetFrameRate(kFrameRate);
    ofBackground(ofColor::black);
    
#ifdef DEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
    
    OFX_BEGIN_EXCEPTION_HANDLING
    
    auto vec2Simple = SceneBase::Ptr(new SceneVec2SimpleGraph());
    auto vec2Clocks = SceneBase::Ptr(new SceneVec2Clocks());
    auto vec2Grid = SceneBase::Ptr(new SceneVec2Grid());
    auto vec2Plotter = SceneBase::Ptr(new SceneVec2Plotter());
    auto bodyVis = SceneBase::Ptr(new SceneBodyVisualization());
    
    mSceneManager.add(vec2Simple);
    mSceneManager.add(vec2Clocks);
    mSceneManager.add(vec2Grid);
    mSceneManager.add(vec2Plotter);
    mSceneManager.add(bodyVis);
    
    // make another instance for existing class
    //auto vec2Simple2 = SceneBase::Ptr(new SceneVec2SimpleGraph());
    //vec2Simple2->setName("SceneVec2SimpleGraph 2"); // set unique name
    //mSceneManager.add(vec2Simple2);
    
    // we can't change the name after added it into scene manager
    //vec2Simple->setName("SceneVec2SimpleGraph renamed");
    
    //mSceneManager.change(3);
    //mSceneManager.change("dp::score::SceneVec2Clocks");
    mSceneManager.change<SceneBodyVisualization>();
    
    mSceneManager.getTabBar()->loadSettings(kSettingsDir, kSettingsPrefix);
    mSceneManager.getTabBar()->setVisible(false);
        
    mOscReceiver.setup(kOscClientPort);
    
    dp::score::registerObjectEvent(this);
    
    dp::score::ObjectEventArgs args;
    
    auto objA = ofPtr<ObjA>(new ObjA());
    auto objB = ofPtr<ObjB>(new ObjB());
    auto objC = ofPtr<ObjC>(new ObjC());
    
    args.addObject(objA);
    args.addObject(objB);
    args.addObject(objC);
    
    dp::score::notifyObjectEvent(args);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::update()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    int n = 0;
    ofVec2f v;
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiver.getNextMessage(&m);
        const string addr = m.getAddress();
        
        if (addr == kOscAddrPendulumVec2) {
            v.x += m.getArgAsFloat(1);
            v.y += m.getArgAsFloat(2);
            n++;
        }
        else if (addr == ofxMotioner::OSC_ADDR) {
            ofxMotioner::updateWithOscMessage(m);
        }
    }
    if (n>0) {
        v /= (float)n;
        
        ofxEventMessage m;
        m.setAddress(kAddrVec2);
        m.addFloatArg(v.x);
        m.addFloatArg(v.y);
        mSceneManager.update(m);
    }
    
    ofxMotioner::update();
    
    OFX_END_EXCEPTION_HANDLING
}
#pragma mark ___________________________________________________________________
void ofApp::draw()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.draw();
        
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::exit()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.clear();
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
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
            mSceneManager.getTabBar()->saveSettings(kSettingsDir, kSettingsPrefix);
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

#pragma mark ___________________________________________________________________
void ofApp::keyReleased(int key)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.keyReleased(key);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::mouseMoved(int x, int y)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseMoved(x, y);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::mouseDragged(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseDragged(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::mousePressed(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mousePressed(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::mouseReleased(int x, int y, int button)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.mouseReleased(x, y, button);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::windowResized(int w, int h)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.windowResized(w, h);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::gotMessage(ofMessage msg)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.gotMessage(msg);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.dragEvent(dragInfo);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    

    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::onObjectReceived(dp::score::ObjectEventArgs& e)
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    cout << __func__ << ": " << e.getClassName(0) << endl;
    cout << __func__ << ": " << e.getClassName(1) << endl;
    cout << __func__ << ": " << e.getClassName(2) << endl;
    
    auto _objA = e.getObject<ObjA>(0);
    auto _objB = e.getObject<ObjB>(1);
    auto _objC = e.getObject<ObjC>(2);
    
    cout << __func__ << ": " << _objA->s << ", " << _objB->s << ", " << _objC->s << endl;
    
    OFX_END_EXCEPTION_HANDLING
}