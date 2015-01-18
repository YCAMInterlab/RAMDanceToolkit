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
    mValves.assign(NUM_VALVE_PINS, Valve());
    
    for (int i=0; i<mValves.size(); i++) {
        mValves.at(i).pin = VALVE_PIN_0 + i;
    }
    
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
        sendPin(PUMP_PIN_FORWARD, mOpenPumpForward);
        sendPin(PUMP_PIN_BACK, mOpenPumpBack);
        return;
    }
    
    for (auto& v : mValves) {
        if (v.doOpen) {
            v.doOpen = false;
            v.opening = true;
            v.openedTime = ofGetElapsedTimef();
            if (mEnable) sendPin(v.pin, v.opening);
        }
        else if (ofGetElapsedTimef()-v.openedTime >= mOpenDuration) {
            v.opening = false;
        }
        if (!v.opening) {
            sendPin(v.pin, v.opening);
        }
    }
    
    const int loop{360};
    const int dur{120};
    if (ofGetFrameNum() % loop < dur) {
        for (int i=0; i<NUM_PUMP_PINS; i++) {
            mPumpOn = true;
            if (mEnable) sendPin(i+PUMP_PIN_FORWARD, mPumpOn);
        }
    }
    else {
        for (int i=0; i<NUM_PUMP_PINS; i++) {
            mPumpOn = false;
            sendPin(i+PUMP_PIN_FORWARD, mPumpOn);
        }
    }
}

void MasterHakoniwa::draw()
{
    
}

void MasterHakoniwa::turnOnValve(int index)
{
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException, "valve index %d is out of range", index);
    mValves.at(index).doOpen = true;
}

void MasterHakoniwa::turnOffAllPins()
{
    for (int i=0; i<NUM_VALVE_PINS; i++) {
        sendPin(i+VALVE_PIN_0, false);
    }
    for (int i=0; i<NUM_PUMP_PINS; i++) {
        sendPin(i+PUMP_PIN_FORWARD, false);
    }
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

