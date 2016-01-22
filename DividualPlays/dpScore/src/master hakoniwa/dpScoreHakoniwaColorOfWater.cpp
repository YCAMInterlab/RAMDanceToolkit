//
//  dpScoreHakoniwaColorOfWater.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/15.
//
//

#include "dpScoreHakoniwaColorOfWater.h"
#include "ofxUITabBar.h"
#include "dpScoreMasterHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

const int HakoniwaColorOfWater::kValvePins[kNumValvePins]{2, 3, 4, 5, 6, 7};
const int HakoniwaColorOfWater::kPumpPins[kNumPumpPins]{8, 9};
const int HakoniwaColorOfWater::kPumpPinForward{kPumpPins[0]};
const int HakoniwaColorOfWater::kPumpPinBack{kPumpPins[1]};

#pragma mark ___________________________________________________________________
void HakoniwaColorOfWater::Valve::update(HakoniwaColorOfWater* owner)
{
    const float t{ofGetElapsedTimef()};
    
    if (!prevOpen && open) {
        openedTime = t;
        nOpend++;
    }
    else if (t - openedTime >= owner->mValveOpenDuration) {
        open = false;
        if (prevOpen && !open) {
            closedTime = t;
        }
    }
    prevOpen = open;
    
    owner->sendPin(pin, open);
}

#pragma mark ___________________________________________________________________
void HakoniwaColorOfWater::Pump::update(HakoniwaColorOfWater* owner)
{
    const float t{ofGetElapsedTimef()};
    
    if (!prevOpen && open) openedTime = t;
    else if (prevOpen && !open) closedTime = t;
    prevOpen = open;
    
    owner->sendPin(pin, open);
}

#pragma mark ___________________________________________________________________

void HakoniwaColorOfWater::initialize()
{
    mValves.assign(kNumValvePins, Valve());
    for (int i=0; i<mValves.size(); i++) {
        mValves.at(i).pin = kValvePins[i];
    }
    
    mPumps.assign(kNumPumpPins, Pump());
    for (int i=0; i<mPumps.size(); i++) {
        mPumps.at(i).pin = kPumpPins[i];
    }
    
    ofxXmlSettings xml;
    xml.load(kXmlSettingsPath);
    
    string errorStr{"error"};
    int errorInt{-1};
    
    xml.pushTag("osc");
    
    const string host{xml.getAttribute("serverMasterHakoniwa",
                                       "host",
                                       errorStr)};
    if (host == errorStr)
        ofxThrowException(ofxException,
                          "serverMasterHakoniwa host name didn't find in XML");
    const int port{xml.getAttribute("serverMasterHakoniwa",
                                    "port",
                                    errorInt)};
    if (port == errorInt)
        ofxThrowException(ofxException,
                          "serverMasterHakoniwa port number didn't find in XML");
    mOscSender.setup(host, port);
    
    xml.popTag();
    
    turnOffAllPins();
}

void HakoniwaColorOfWater::shutdown()
{
    enableOscOut = true;
    turnOffAllPins();
}

void HakoniwaColorOfWater::setupGui(ofxUITabBar* tabbar)
{
    const float sizeSml{OFX_UI_GLOBAL_BUTTON_DIMENSION};
    const float w{MH::kGuiWidth};
    const float h{MH::kGuiHeight};
    
    tabbar->addLabel("[Master Hakoniwa]", OFX_UI_FONT_SMALL);
    
    tabbar->addLabel("Open Valve (Color Water)", OFX_UI_FONT_SMALL);
    tabbar->addToggle(ofToString(0), &mValves.at(0).open);
    for (int i=1; i<kNumValvePins; i++) {
        tabbar->addWidgetRight(new ofxUIToggle(ofToString(i),
                                               &mValves.at(i).open,
                                               sizeSml,
                                               sizeSml));
    }
    
    tabbar->addLabel("Open Pump (Clear Water)", OFX_UI_FONT_SMALL);
    tabbar->addToggle("In", &mPumps.at(0).open);
    tabbar->addWidgetRight(new ofxUIToggle("Out",
                                           &mPumps.at(1).open,
                                           sizeSml,
                                           sizeSml));
    tabbar->addSlider("Valve Open Duration",
                      0.f,
                      1.f,
                      &mValveOpenDuration,
                      w,
                      MH::kLineHeight);
    tabbar->addSpacer(w, 1.f);
}

void HakoniwaColorOfWater::update()
{
    if (ofGetFrameNum() % kUpdateFrames) return;
    
    for (int i=0; i<kNumValvePins; i++) {
        mValves[i].update(this);
    }
    
    for (int i=0; i<kNumPumpPins; i++) {
        mPumps[i].update(this);
    }
}

void HakoniwaColorOfWater::draw()
{
    const float t{ofGetElapsedTimef()};
    
    ofSetColor(MH::kTextColor);
    ofDrawBitmapString("[valves]", ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing);
    
    int i{0};
    for (auto& v : mValves) {
        v.open ? ofSetColor(color::kMain) : ofSetColor(MH::kTextColor);
        stringstream ss;
        ss << i << " = " << (v.open ? "on " : "off")
        << setprecision(2) << fixed
        << " : " << (v.open ? t - v.openedTime : 0.f)
        << " / " << mValveOpenDuration
        << " | " << (!v.open ? t - v.closedTime : 0.f);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        alignedTranslate(0.f, MH::kTextSpacing);
        i++;
    }
    
    ofSetColor(MH::kTextColor);
    alignedTranslate(0.f, MH::kTextSpacing);
    ofDrawBitmapString("[pumps]", ofPoint::zero());
    
    alignedTranslate(0.f, MH::kTextSpacing);
    i = 0;
    for (auto& p : mPumps) {
        p.open ? ofSetColor(color::kMain) : ofSetColor(MH::kTextColor);
        stringstream ss;
        ss << i << " = " << (p.open ? "on " : "off")
        << setprecision(1) << fixed
        << " : " << (p.open ? t - p.openedTime : 0.f)
        << " | " << (!p.open ? t - p.closedTime : 0.f);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        alignedTranslate(0.f, MH::kTextSpacing);
        i++;
    }
}

void HakoniwaColorOfWater::turnOnValve(int index)
{
    if (!enableOpenValve) return;
    
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException,
                           "valve index %d is out of range",
                           index);
    mValves.at(index).open = true;
}

void HakoniwaColorOfWater::turnOffAllPins()
{
    for (auto& v : mValves) {
        v.open = false;
        sendPin(v.pin, v.open);
    }
    for (auto& p : mPumps) {
        p.open = false;
        sendPin(p.pin, p.open);
    }
}

bool HakoniwaColorOfWater::getIsOpeningValve(int index)
{
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException,
                           "valve index %d is out of range",
                           index);
    return mValves.at(index).open;
}

void HakoniwaColorOfWater::sendPin(int pin, bool open)
{
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
    m.addIntArg((int)open);
    if (enableOscOut) mOscSender.sendMessage(m);
}

void HakoniwaColorOfWater::stopAll()
{
    enableOpenValve = false;
    turnOffAllPins();
    enableOscOut = false;
}

DP_SCORE_NAMESPACE_END
