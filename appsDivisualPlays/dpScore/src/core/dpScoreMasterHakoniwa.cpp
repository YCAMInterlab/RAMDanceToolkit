//
//  dpScoreMasterHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreMasterHakoniwa.h"
#include "ofxUI.h"

DP_SCORE_NAMESPACE_BEGIN

const string MH::kSceneNames[MH::kNumScenes]{
    //"dpVisPLink_Laser",
    "dpVisServoPendulum",
    "dpVisMagPendulum",
    "dpVisSandStorm",
    "dpVisTheta",
    "dpVisStruggle",
    "dpVisStage",
    "dpHGearMove",
    "dpHWorm",
    "dpVisTornado",
    "",
    "",
    "",
};

const ofColor MH::kBackgroundColor{255, 20};
const ofColor MH::kTextColor{255, 200};

const int MH::kValvePins[kNumValvePins]{2, 3, 4, 5, 6, 7};
const int MH::kPumpPins[kNumPumpPins]{8, 9};
const int MH::kPumpPinForward{kPumpPins[0]};
const int MH::kPumpPinBack{kPumpPins[1]};

const float MH::kPumpOpenDur[MH::kNumPumpPins]{8.0f, 5.0f};
const float MH::kPumpCloseDur[MH::kNumPumpPins]{2.0f, 5.0f};

const string MH::kHostNameMasterHakoniwa{"192.168.20.60"};
const int MH::kPortNumberMasterHakoniwa {8528};
const string MH::kHostNameCameraUnit{"192.168.20.5"};
//const int MH::kPortNumberCameraUnit{12400};
const int MH::kPortNumberCameraUnit{12345};

const string MH::kOscAddrRamSetScene{"/ram/set_scene"};

#pragma mark ___________________________________________________________________
MasterHakoniwa& MasterHakoniwa::instance()
{
    static MasterHakoniwa instance;
    return instance;
}

void MasterHakoniwa::setupUI(ofxUITabBar* tabbar)
{
    const float sizeBig{40.f};
    const float w{250.f};
    
    tabbar->setColorBack(kBackgroundColor);
    tabbar->setPosition(kLineWidth, kTopOffset);
    tabbar->addToggle("[[[Stop!!!]]]", &mEmergencyStop, sizeBig, sizeBig);
    tabbar->addSpacer(w, 1.f);
    tabbar->addToggle("Enable Valve", &mEnableValve);
    tabbar->addToggle("Enable Pump", &mEnablePump);
     tabbar->addSpacer(w, 1.f);
    tabbar->addSlider("Valve open duration", 0.f, 1.f, &mOpenDuration);
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("(Turn off [Enable pump] toggle)", OFX_UI_FONT_SMALL);
    for (int i=0; i<kNumValvePins; i++) {
        tabbar->addToggle("Open valve " + ofToString(i), &mOpenValve[i]);
    }
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("(Turn off [Enable valve] toggle)", OFX_UI_FONT_SMALL);
    tabbar->addToggle("Open pump forward", &mOpenPump[0]);
    tabbar->addToggle("Open pump back", &mOpenPump[1]);
    tabbar->addSpacer(w, 1.f);
    
    // UICanvasセットポジションして入れる
}

void MasterHakoniwa::initialize()
{
    mValves.assign(kNumValvePins, Valve());
    for (int i=0; i<mValves.size(); i++) {
        mValves.at(i).pin = kValvePins[i];
    }
    
    mPumps.assign(kNumPumpPins, Pump());
    for (int i=0; i<mPumps.size(); i++) {
        mPumps.at(i).duration = kPumpOpenDur[i];
        mPumps.at(i).idle = kPumpCloseDur[i];
        mPumps.at(i).pin = kPumpPins[i];
    }
    
    mMasterHakoniwaOscServer.setup(kHostNameMasterHakoniwa,
                                   kPortNumberMasterHakoniwa);
    mCameraUnitOscServer.setup(kHostNameCameraUnit,
                               kPortNumberCameraUnit);
    turnOffAllPins();
    
    for (int i=0; i<kNumScenes; i++) {
        mScenes[kSceneNames[i]] = Scene();
    }
    
    ofAddListener(ofxMot::drawSkeletonEvent, this, &MasterHakoniwa::onDrawSkeleton);
}

void MasterHakoniwa::shutdown()
{
    ofRemoveListener(ofxMot::drawSkeletonEvent, this, &MasterHakoniwa::onDrawSkeleton);
    
    turnOffAllPins();
}

void MasterHakoniwa::update()
{
    if (mEmergencyStop) {
        mEnableValve = false;
        mEnablePump = false;
        turnOffAllPins();
        mEmergencyStop = false;
    }
    
    if (ofGetFrameNum() % kUpdateFrames) return;
        
    if (!mEnableValve) {
        for (int i=0; i<kNumValvePins; i++) {
               sendPin(kValvePins[i], mOpenValve[i]);
        }
    }
    if (!mEnablePump) {
        for (int i=0; i<kNumPumpPins; i++) {
            sendPin(kPumpPins[i], mOpenPump[i]);
        }
    }
    
    const float t = ofGetElapsedTimef();

    for (int i=0; i<mValves.size(); i++) {
        auto& v = mValves.at(i);
        if (v.doOpen) {
            v.doOpen = false;
            v.opening = true;
            v.openedTime = t;
        }
        else if (t - v.openedTime >= mOpenDuration) {
            v.opening = false;
        }
        if (mEnableValve) {
            sendPin(v.pin, v.opening);
            mOpenPump[i] = v.opening;
        }
    }
    
    for (int i=0; i<mPumps.size(); i++) {
        auto& p = mPumps.at(i);
        const float d = t - p.openedTime;
        if (d < p.duration) {
            p.opening = true;
        }
        else if (d >= p.duration && d < p.duration + p.idle) {
            p.opening = false;
        }
        else {
            p.openedTime = t;
        }
        if (mEnablePump) {
            sendPin(p.pin, p.opening);
            mOpenPump[i] = p.opening;
        }
    }
}

void MasterHakoniwa::draw()
{
    const float t{ofGetElapsedTimef()};

    mLineNum = kNumPumpPins + kNumValvePins + mScenes.size() + 5;
    
    mTextLeftCorner.set(kTextSpacing + kMargin,
                        kTopOffset + kTextSpacing + kTextSpacing * mLineNum + kMargin);
    
    ofFill();
    ofSetColor(255, 0, 0, 20);
    ofRect(kMargin,
           kTopOffset - kMargin,
           ofGetWidth() - kMargin * 2.f,
           ofGetHeight() - kTopOffset);
    
    ofPushMatrix();
    ofTranslate(kTextSpacing, kTopOffset);
    
    ofSetColor(kBackgroundColor);
    ofRect(0.f,
           0.f,
           kLineWidth - kTextSpacing - kMargin,
           mLineNum * kTextSpacing);
    ofRect(0.f,
           mLineNum * kTextSpacing + kMargin,
           kLineWidth - kTextSpacing - kMargin,
           ofGetHeight() - mLineNum * kTextSpacing - kMargin * 3.f - kTopOffset);
    
    ofSetColor(kTextColor);
    ofTranslate(kMargin, kTextSpacing);
    ofDrawBitmapString("valve", ofPoint::zero());
    ofTranslate(0.f, kTextSpacing);
    for (auto& v : mValves) {
        v.opening ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
        stringstream ss;
        ss << v.pin << " = " << (v.opening ? "on " : "off")
        << setprecision(1) << fixed
        << " : " << t - v.openedTime  << " / " << mOpenDuration;
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        ofTranslate(0.f, kTextSpacing);
    }
    ofTranslate(0.f, kTextSpacing);
    ofDrawBitmapString("pump", ofPoint::zero());
    ofTranslate(0.f, kTextSpacing);
    for (auto& p : mPumps) {
        p.opening ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
        stringstream ss;
        ss << p.pin << " = " << (p.opening ? "on " : "off")
        << setprecision(1) << fixed
        << " : " << t - p.openedTime  << " / "
        << p.duration << " / " << p.idle << endl;
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        ofTranslate(0.f, kTextSpacing);
    }
    ofTranslate(0.f, kTextSpacing);
    for (auto it : mScenes) {
        const string name{it.first};
        auto& s = it.second;
        s.enabled ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
        ofDrawBitmapString(name, ofPoint::zero());
        stringstream ss;
        ss << "= " << (s.enabled ? "on " : "off") << " : ";
        for (int i=0; i<kNumWindows; i++) {
            ss << (int)s.scr[i];
            if (i<kNumWindows-1) ss << ", ";
        }
        ofDrawBitmapString(ss.str(), ofPoint(kTextSpacing * 15.f, 0.f));
        ofTranslate(0.f, kTextSpacing);
    }
    ofPopMatrix();
    
    ofSetColor(kBackgroundColor);
    ofRect(mCamViewport);
    
    ofPushStyle();
    ofEnableDepthTest();
    mCam.begin(mCamViewport);
    ofTranslate(0.f, -100.f);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRotateX(90.f);
    ofSetColor(kBackgroundColor, 5);
    ofRect(ofPoint::zero(), 10000, 1000);
    mCam.end();
    ofPopStyle();
}

void MasterHakoniwa::turnOnValve(int index)
{
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException, "valve index %d is out of range", index);
    mValves.at(index).doOpen = true;
}

void MasterHakoniwa::turnOffAllPins()
{
    for (int i=0; i<kNumValvePins; i++) {
        mOpenValve[i] = false;
        sendPin(kValvePins[i], mOpenValve[i]);
    }
    for (int i=0; i<kNumPumpPins; i++) {
        mOpenPump[i] = false;
        sendPin(kPumpPins[i], mOpenPump[i]);
    }
}

bool MasterHakoniwa::getIsOpeningValve(int index)
{
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException, "valve index %d is out of range", index);
    return mValves.at(index).opening;
}

void MasterHakoniwa::sendPin(int pin, bool open)
{
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
    m.addIntArg((int)open);
    mMasterHakoniwaOscServer.sendMessage(m);
}

void MasterHakoniwa::sendScene(const string& name,
                               bool enabled,
                               bool scr0,
                               bool scr1)
{
    auto& scene = mScenes[name];
    scene.enabled = enabled;
    scene.scr[0] = scr0;
    scene.scr[1] = scr1;
    scene.dirty = true;
    
    for (auto& it : mScenes) {
        auto& s = it.second;
        const string& sName = it.first;
        
        if (sName != name) {
            bool allOff = true;
            for (int i=0; i<kNumWindows; i++) {
                if (scene.scr[i] && s.scr[i]) {
                    s.scr[i] = false;
                    s.dirty = true;
                }
                if (s.scr[i]) allOff = false;
            }
            if (allOff) {
                s.enabled = false;
            }
        }
        if (!s.dirty) continue;
        
        stringstream ss;
        ofxOscMessage m;
        m.setAddress(kOscAddrRamSetScene);
        m.addStringArg(sName);
        m.addIntArg((int)s.enabled);
        
        ss << boolalpha << sName << "=" << s.enabled << " / ";
        
        for (int i=0; i<kNumWindows; i++) {
            m.addIntArg((int)s.scr[i]);
            ss << s.scr[i] << ", ";
        }
        mCameraUnitOscServer.sendMessage(m);
        ofLogNotice() << ss.str();
    }
}

void MasterHakoniwa::onDrawSkeleton(ofxMotioner::EventArgs &e)
{
    const float x{kLineWidth};
    const float y{407.f};
    mCamViewport.set(x,
                     y,
                     (ofGetWidth() - x) * 0.5f - kMargin,
                     ofGetHeight() - y - kMargin * 2.f);
    mCam.begin(mCamViewport);
    ofEnableDepthTest();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(0.f, -100.f + 8.f);
    
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    
    auto& joints = e.skeleton->getJoints();
    
    ofNoFill();
    ofSetSphereResolution(2);
    
    for (size_t i=0; i<joints.size(); i++) {
        
        ofSetColor(dp::score::color::kDarkPinkHeavy);
        
        ofSetLineWidth(1.0f);
        auto& n = joints.at(i);
        n.transformGL();
        ofDrawSphere(8.f);
        n.restoreTransformGL();
        
        if (!n.getParent()) continue;
        
        ofSetColor(dp::score::color::kDarkPinkHeavy);
        ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
    }
    
    ofPushMatrix();
    ofSetColor(dp::score::color::kMain);
    ofMultMatrix(joints.at(ofxMot::JOINT_HEAD).getGlobalTransformMatrix());
    ofDrawBitmapString(e.skeleton->getName(),
                       ofPoint(0.0f, joints.at(ofxMot::JOINT_HEAD).size));
    ofPopMatrix();
    
    ofPopStyle();
    ofPopMatrix();
    
    mCam.end();
}

DP_SCORE_NAMESPACE_END