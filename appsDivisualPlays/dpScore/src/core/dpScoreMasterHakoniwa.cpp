//
//  dpScoreMasterHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreMasterHakoniwa.h"
#include "ofxUI.h"

static const string kHostNameMasterHakoniwa = "192.168.20.60";
static const int kPortNumberMasterHakoniwa = 8528;

static const string kHostNameCameraUnit = "192.168.20.5";
static const int kPortNumberCameraUnit = 12400;

static const string kOscAddrRamSetScene = "/ram/set_scene";

const int MasterHakoniwa::kNumScenes{7};

const string MasterHakoniwa::kSceneNames[kNumScenes]{
    "dpVisPLink_Laser",
    "dpVisMagPendulum",
    "dpVisServoPendulum",
    "dpVisSandStorm",
    "dpVisStruggle",
    "dpVisStage",
    "dpVisTheta",
};

MasterHakoniwa& MasterHakoniwa::instance()
{
    static MasterHakoniwa instance;
    return instance;
}

void MasterHakoniwa::setupUI(ofxUITabBar* tabbar)
{
    tabbar->addToggle("Enable", &mEnable);
    tabbar->addSpacer();
    tabbar->addLabel("(Turn off Enable toggle)");
    tabbar->addToggle("Open Pump Forward", &mOpenPumpForward);
    tabbar->addToggle("Open Pump Back", &mOpenPumpBack);
}

void MasterHakoniwa::initialize()
{
    mValves.assign(kNumValvePins, Valve());
    
    for (int i=0; i<mValves.size(); i++) {
        mValves.at(i).pin = kValvePins[i];
    }
    
    mPumps.assign(kNumPumpPins, Pump());
    mPumps.at(0).duration = 8.f;
    mPumps.at(0).idle = 2.f;
    mPumps.at(1).duration = 7.f;
    mPumps.at(1).idle = 3.f;
    mPumps.at(0).pin = kPumpPinForward;
    mPumps.at(1).pin = kPumpPinBack;
    
    mMasterHakoniwaOscServer.setup(kHostNameMasterHakoniwa,
                                   kPortNumberMasterHakoniwa);
    mCameraUnitOscServer.setup(kHostNameCameraUnit,
                               kPortNumberCameraUnit);
    
    turnOffAllPins();
}

void MasterHakoniwa::shutdown()
{
    turnOffAllPins();
}

void MasterHakoniwa::update()
{
    if (ofGetFrameNum() % kUpdateFrames)
    
    if (!mEnable) {
        sendPin(kPumpPinForward, mOpenPumpForward);
        sendPin(kPumpPinBack, mOpenPumpBack);
        return;
    }
    
    const float t = ofGetElapsedTimef();

    for (auto& v : mValves) {
        if (v.doOpen) {
            v.doOpen = false;
            v.opening = true;
            v.openedTime = t;
            if (mEnable) sendPin(v.pin, v.opening);
        }
        else if (t - v.openedTime >= mOpenDuration) {
            v.opening = false;
        }
        if (!v.opening) {
            sendPin(v.pin, v.opening);
        }
    }
    
    for (auto& p : mPumps) {
        const float d = t - p.openedTime;
        if (d < p.duration) {
            p.opening = true;
            sendPin(p.pin, p.opening);
        }
        else if (d >= p.duration && d < p.duration + p.idle) {
            p.opening = false;
            sendPin(p.pin, p.opening);
        }
        else {
            p.openedTime = t;
        }
    };
}

void MasterHakoniwa::draw()
{
    ofPushMatrix();
    ofTranslate(12.f, 30.f);
    const float lineH{12.f};
    ofTranslate(0.f, lineH);
    for (auto& v : mValves) {
        ofTranslate(0.f, lineH);
        stringstream ss;
        ss << boolalpha << "valve " << v.pin << ": " << v.opening
        << ", " << ofToString(ofGetElapsedTimef() - v.openedTime, 1)  << "/"
        << mOpenDuration;
        ofDrawBitmapString(ss.str(), ofPoint::zero());
    }
    ofTranslate(0.f, lineH);
    for (auto& p : mPumps) {
        ofTranslate(0.f, lineH);
        stringstream ss;
        ss << boolalpha << "pump " << p.pin << " : " << p.opening
        << ", " << ofToString(ofGetElapsedTimef() - p.openedTime, 1)  << "/"
        << p.duration << "/" << p.idle << endl;
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        
    }
    ofPopMatrix();
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
        sendPin(kValvePins[i], false);
    }
    for (int i=0; i<kNumPumpPins; i++) {
        sendPin(kPumpPins[i], false);
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
                            bool scr1,
                            bool scr2,
                            bool scr3)
{
    ofxOscMessage m;
    m.setAddress(kOscAddrRamSetScene);
    m.addStringArg(name);
    m.addIntArg((int)enabled);
    m.addIntArg((int)scr0);
    m.addIntArg((int)scr1);
    m.addIntArg((int)scr2);
    m.addIntArg((int)scr3);
    mCameraUnitOscServer.sendMessage(m);
    
    ofLogNotice() << "set scene: " << boolalpha << name << ", " << enabled << ", " << scr0 << ", " << scr1 << ", " << scr2 << ", " << scr3;
}

