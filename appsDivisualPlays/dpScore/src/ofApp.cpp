#include "ofApp.h"
#ifdef DP_MASTER_HAKONIWA
#include "dpScoreMasterHakoniwa.h"
#include "dpScoreSceneMasterIncrement.h"
#else
#include "dpScoreSceneVec2SimpleGraph.h"
#include "dpScoreSceneVec2Clocks.h"
#include "dpScoreSceneVec2Grid.h"
#include "dpScoreSceneVec2Plotter.h"
#include "dpScoreSceneDataCircle.h"
#include "dpScoreSceneDataCircle3D.h"
#include "dpScoreSceneDataSphere.h"
#include "dpScoreSceneDataWave.h"
#include "dpScoreSceneDataDisplacement.h"
#include "dpScoreSceneDataBarcode.h"
#include "dpScoreSceneDataText.h"
#include "dpScoreSceneDataSlider.h"
#include "dpScoreSceneBodyGlobe.h"
#include "dpScoreSceneBodyScan.h"
#include "dpScoreSceneBodyFlow.h"
#include "dpScoreSceneBodyPatterns.h"
#include "dpScoreSceneBodyLines.h"
#include "dpScoreSceneDataScroll.h"
#include "dpScoreSceneBodyBoids.h"
#include "dpScoreSceneBodyRect.h"
#include "dpScoreSceneBodyBox.h"
#endif

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

身体系 8/20
箱庭系 13/20

プロッター*n
ブロブ系
フォント系小

二人つなぐ

身体パーツ
globeパーツ毎

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
    
#ifdef DP_MASTER_HAKONIWA
    getMH().initialize();
    getMH().setupUI(mSceneManager.getTabBar());
    
    auto increment = SceneBase::Ptr(new SceneMasterIncrement());
    mSceneManager.add(increment);
    mSceneManager.change<SceneMasterIncrement>();
#else
    auto black = SceneBase::Ptr(new SceneBase());
    black->setDrawHeader(false);
    black->setName("black");
    
    auto vec2Simple = SceneBase::Ptr(new SceneVec2SimpleGraph());
    auto vec2Clocks = SceneBase::Ptr(new SceneVec2Clocks());
    auto vec2Grid = SceneBase::Ptr(new SceneVec2Grid());
    auto vec2Plotter = SceneBase::Ptr(new SceneVec2Plotter());
    auto dataScroll = SceneBase::Ptr(new SceneDataScroll());
    auto dataCircle = SceneBase::Ptr(new SceneDataCircle());
    auto dataCircle3D = SceneBase::Ptr(new SceneDataCircle3D());
    auto dataSphere = SceneBase::Ptr(new SceneDataSphere());
    auto dataDisplacement = SceneBase::Ptr(new SceneDataDisplacement());
    auto dataWave = SceneBase::Ptr(new SceneDataWave());
    auto dataBarcode = SceneBase::Ptr(new SceneDataBarcode());
    auto dataText = SceneBase::Ptr(new SceneDataText());
    auto dataSlider = SceneBase::Ptr(new SceneDataSlider());
    
    //auto bodyBox = SceneBase::Ptr(new SceneBodyBox());
    //auto bodyRect = SceneBase::Ptr(new SceneBodyRect());
    auto bodyBoids = SceneBase::Ptr(new SceneBodyBoids());
    auto bodyScan = SceneBase::Ptr(new SceneBodyScan());
    auto bodyPattern = SceneBase::Ptr(new SceneBodyPatterns());
    auto bodyFlow = SceneBase::Ptr(new SceneBodyFlow());
    auto bodyGlobe = SceneBase::Ptr(new SceneBodyGlobe());
    auto bodyLines = SceneBase::Ptr(new SceneBodyLines());
    
    mSceneManager.add(black);
    
    mSceneManager.add(vec2Simple);
    mSceneManager.add(vec2Clocks);
    mSceneManager.add(vec2Grid);
    mSceneManager.add(vec2Plotter);
    mSceneManager.add(dataScroll);
    
    mSceneManager.add(dataCircle);
    mSceneManager.add(dataCircle3D);
    mSceneManager.add(dataSphere);
    mSceneManager.add(dataWave);
    mSceneManager.add(dataDisplacement);
    
    mSceneManager.add(dataBarcode);
    mSceneManager.add(dataText);
    mSceneManager.add(dataSlider);
    
    mSceneManager.add(bodyGlobe);
    mSceneManager.add(bodyScan);
    mSceneManager.add(bodyPattern);
    mSceneManager.add(bodyFlow);
    mSceneManager.add(bodyLines);
    
    //mSceneManager.add(bodyRect);
    //mSceneManager.add(bodyBox);
    mSceneManager.add(bodyBoids);
    
    
    // make another instance for existing class
    //auto vec2Simple2 = SceneBase::Ptr(new SceneVec2SimpleGraph());
    //vec2Simple2->setName("SceneVec2SimpleGraph 2"); // set unique name
    //mSceneManager.add(vec2Simple2);
    
    // we can't change the name after added it into scene manager
    //vec2Simple->setName("SceneVec2SimpleGraph renamed");
    
    //mSceneManager.change(3);
    mSceneManager.change("black");
    mSceneManager.change<SceneDataSlider>();
#endif
    
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
        else if (addr == ofxMotioner::OSC_ADDR) {
            OFX_BEGIN_EXCEPTION_HANDLING
            ofxMotioner::updateWithOscMessage(m);
            OFX_END_EXCEPTION_HANDLING
        }
        else {
            OFX_BEGIN_EXCEPTION_HANDLING
            const string newAddr = makeInternalCameraUnitAddress(addr);
            if (newAddr != "") {
                mCameraUnitMessage = m;
                mCameraUnitMessage.setAddress(newAddr);
            }
            OFX_END_EXCEPTION_HANDLING
        }
    }
    
    if (mDebugCamUnit) generateFakeVectorData();
    
    ofxMotioner::update();
    
    ofxEventMessage m;
    m.setAddress(kOscAddrMotioner);
    mSceneManager.update(m);
    
    mSceneManager.update(mCameraUnitMessage);
    
#ifdef DP_MASTER_HAKONIWA
    ofSetWindowTitle("dpMasterHakoniwa : " + ofToString(ofGetFrameRate(), 2));
    getMH().update();
#else
    ofSetWindowTitle("dpScore : " + ofToString(ofGetFrameRate(), 2));
#endif
    OFX_END_EXCEPTION_HANDLING
}

void ofApp::generateFakeVectorData()
{
    const float t = ofGetElapsedTimef();
    mCameraUnitMessage.clear();
    mCameraUnitMessage.setAddress(kOscAddrCameraUnitVector);
    for (int i=0; i<kNumCameraunitVectors; i++) {
        ofVec2f v;
        v.x = ofSignedNoise(t, 0, i) + ofSignedNoise(t*9.8f, 0, i) * 0.5f  + ofSignedNoise(t*102.f, 0, i) * 0.25f;
        v.y = ofSignedNoise(t, 1, i) + ofSignedNoise(t*9.8f, 1, i) * 0.5f  + ofSignedNoise(t*102.f, 1, i) * 0.25f;
        mCameraUnitMessage.addFloatArg(v.x);
        mCameraUnitMessage.addFloatArg(v.y);
    }
}

string ofApp::makeInternalCameraUnitAddress(const string& addr)
{
    string newAddr = "";
    auto dir = ofSplitString(addr, "/");
    if (dir.size() >= 5 && dir.at(1) == "dp" && dir.at(2) == "cameraUnit") {
        string newAddr = "/" + dir.at(1) + "/" + dir.at(2) + "/" + dir.at(4);
        for (int i=5; i<dir.size(); i++) {
            newAddr += "/" + dir.at(i);
        }
    }
    return newAddr;
}

#pragma mark ___________________________________________________________________
void ofApp::draw()
{
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mSceneManager.draw();
    
#ifdef DP_MASTER_HAKONIWA
    getMH().draw();
#endif
    
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
    
#ifdef DP_MASTER_HAKONIWA
    //getMH().shutdown();
#endif
    
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
        case 'd':
            mDebugCamUnit ^= true;
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