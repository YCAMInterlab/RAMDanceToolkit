#include "ofApp.h"
#include "dpScoreSceneVec2SimpleGraph.h"
#include "dpScoreSceneVec2Clocks.h"
#include "dpScoreSceneVec2Grid.h"
#include "dpScoreSceneVec2Plotter.h"
#include "dpScoreSceneBodyGlobe.h"
#include "dpScoreSceneBodyScan.h"
#include "dpScoreSceneBodyFlow.h"
#include "dpScoreSceneBodyPatterns.h"
#include "dpScoreSceneBodyLines.h"
#include "dpScoreSceneDataScroll.h"
#include "dpScoreSceneBodyBoids.h"

using namespace dp::score;

//struct ObjA {
//    ObjA() { cout << __func__ << endl; }
//    ~ObjA() { cout << __func__ << endl; }
//    string s = "hoge";
//};
//struct ObjB {
//    ObjB() { cout << __func__ << endl; }
//    ~ObjB() { cout << __func__ << endl; }
//    string s = "foo";
//};
//struct ObjC {
//    ObjC() { cout << __func__ << endl; }
//    ~ObjC() { cout << __func__ << endl; }
//    string s = "bar";
//};

#if 0

身体系 6/20
箱庭系 5/20

#endif

#pragma mark ___________________________________________________________________
void ofApp::setup()
{
    ofSetFrameRate(kFrameRate);
    ofBackground(ofColor::black);
    
#ifdef DEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
    
    OFX_BEGIN_EXCEPTION_HANDLING
    
    auto bodyBoids = SceneBase::Ptr(new SceneBodyBoids());
    auto bodyScan = SceneBase::Ptr(new SceneBodyScan());
    auto bodyPattern = SceneBase::Ptr(new SceneBodyPatterns());
    auto bodyFlow = SceneBase::Ptr(new SceneBodyFlow());
    auto bodyGlobe = SceneBase::Ptr(new SceneBodyGlobe());
    auto bodyLines = SceneBase::Ptr(new SceneBodyLines());
    
    auto vec2Simple = SceneBase::Ptr(new SceneVec2SimpleGraph());
    auto vec2Clocks = SceneBase::Ptr(new SceneVec2Clocks());
    auto vec2Grid = SceneBase::Ptr(new SceneVec2Grid());
    auto vec2Plotter = SceneBase::Ptr(new SceneVec2Plotter());
    
    auto dataScroll = SceneBase::Ptr(new SceneDataScroll());
    
    mSceneManager.add(bodyLines);
    mSceneManager.add(bodyPattern);
    mSceneManager.add(bodyScan);
    mSceneManager.add(bodyFlow);
    mSceneManager.add(bodyGlobe);
    mSceneManager.add(bodyBoids);
    
    mSceneManager.add(vec2Simple);
    mSceneManager.add(vec2Clocks);
    mSceneManager.add(vec2Grid);
    mSceneManager.add(vec2Plotter);
    
    mSceneManager.add(dataScroll);
    
    // make another instance for existing class
    //auto vec2Simple2 = SceneBase::Ptr(new SceneVec2SimpleGraph());
    //vec2Simple2->setName("SceneVec2SimpleGraph 2"); // set unique name
    //mSceneManager.add(vec2Simple2);
    
    // we can't change the name after added it into scene manager
    //vec2Simple->setName("SceneVec2SimpleGraph renamed");
    
    //mSceneManager.change(3);
    //mSceneManager.change("SceneVec2Clocks");
    mSceneManager.change<SceneBodyBoids>();
    
    mSceneManager.getTabBar()->loadSettings(kSettingsDir, kSettingsPrefix);
    mSceneManager.getTabBar()->setVisible(false);
        
    mOscReceiver.setup(kOscClientPort);
    
    dp::score::registerObjectEvent(this);
    
    //dp::score::ObjectEventArgs args;
    //
    //auto objA = ofPtr<ObjA>(new ObjA());
    //auto objB = ofPtr<ObjB>(new ObjB());
    //auto objC = ofPtr<ObjC>(new ObjC());
    //
    //args.addObject(objA);
    //args.addObject(objB);
    //args.addObject(objC);
    //
    //dp::score::notifyObjectEvent(args);
    
    OFX_END_EXCEPTION_HANDLING
}

#pragma mark ___________________________________________________________________
void ofApp::update()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    ofSetWindowTitle("dpScore : " + ofToString(ofGetFrameRate(), 2));
    
    bool updatedVector = false;
    bool updatedMotioner = false;
    ofVec2f v;
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiver.getNextMessage(&m);
        const string addr = m.getAddress();
        
        if (addr == kOscAddrChangeScene) {
OFX_BEGIN_EXCEPTION_HANDLING
            if (m.getNumArgs() >= 1) {
                if (m.getArgType(0) == OFXOSC_TYPE_INT32) {
                    mSceneManager.change(m.getArgAsInt32(0));
                }
                else if (m.getArgType(0) == OFXOSC_TYPE_STRING){
                    mSceneManager.change(m.getArgAsString(0));
                }
            }
OFX_END_EXCEPTION_HANDLING
        }
        else if (addr == kOscAddrPendulumVec2) {
            if (m.getNumArgs() == 20) {
                for (int i=0; i<10; i++) {
                    v.x += m.getArgAsFloat(i*2+0);
                    v.y += m.getArgAsFloat(i*2+1);
                }
                v /= 10.f;
                updatedVector = true;
            }
            else {
                ofLogError() << kOscAddrPendulumVec2 << ": receiving incorrect arguments";
            }
        }
        else if (addr == ofxMotioner::OSC_ADDR) {
            ofxMotioner::updateWithOscMessage(m);
            updatedMotioner = true;
        }
    }
    
    if (updatedVector) {
        ofxOscMessage m;
        m.setAddress(kAddrVec2);
        m.addFloatArg(v.x);
        m.addFloatArg(v.y);
        mSceneManager.update(m);
    }
    if (updatedMotioner) {
        ofxOscMessage m;
        m.setAddress(kAddrMotioner);
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
    
    if (mInvert) {
        ofPushStyle();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
        ofSetColor(ofColor::white);
        ofRect(ofGetWindowRect());
        ofPopStyle();
    }
    
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
            case 'i':
            mInvert ^= true;
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
    
//    cout << __func__ << ": " << e.getClassName(0) << endl;
//    cout << __func__ << ": " << e.getClassName(1) << endl;
//    cout << __func__ << ": " << e.getClassName(2) << endl;
//    
//    auto _objA = e.getObject<ObjA>(0);
//    auto _objB = e.getObject<ObjB>(1);
//    auto _objC = e.getObject<ObjC>(2);
//    
//    cout << __func__ << ": " << _objA->s << ", " << _objB->s << ", " << _objC->s << endl;
    
    OFX_END_EXCEPTION_HANDLING
}