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

const ofColor MH::kBackgroundColor{255, 20};
const ofColor MH::kTextColor{255, 200};
const ofColor MH::kTextColorDark{255, 100};

const int MH::kValvePins[kNumValvePins]{2, 3, 4, 5, 6, 7};
const int MH::kPumpPins[kNumPumpPins]{8, 9};
const int MH::kPumpPinForward{kPumpPins[0]};
const int MH::kPumpPinBack{kPumpPins[1]};

const float MH::kPumpOpenDur[MH::kNumPumpPins]{8.0f, 5.0f};
const float MH::kPumpCloseDur[MH::kNumPumpPins]{2.0f, 5.0f};

const string MH::kHostNameMasterHakoniwa{"192.168.20.60"};
const int MH::kPortNumberMasterHakoniwa {8528};
const string MH::kHostNameCameraUnit{"192.168.20.5"};
const int MH::kPortNumberCameraUnit{12400};
const string MH::kHostNameScore{"192.168.20.11"};
const int MH::kPortNumberScore{10000};

const string MH::kOscAddrRamSetScene{"/ram/set_scene"};
const string MH::kOscAddrRamDoSomething{"/ram/do_something"};

const string MH::kScoreBlack{"black"};

#pragma mark ___________________________________________________________________
void MasterHakoniwa::Valve::update(MasterHakoniwa* mh)
{
    const float t{ofGetElapsedTimef()};
    
    if (!prevOpen && open) {
        openedTime = t;
        nOpend++;
    }
    else if (t - openedTime >= mh->mValveOpenDuration) {
        open = false;
        if (prevOpen && !open) {
            closedTime = t;
        }
    }
    prevOpen = open;
    
    mh->sendPin(pin, open);
}

#pragma mark ___________________________________________________________________
void MasterHakoniwa::Pump::update(MasterHakoniwa* mh)
{
    const float t{ofGetElapsedTimef()};
    
    if (!prevOpen && open) {
        openedTime = t;
    }
    else if (prevOpen && !open) {
        closedTime = t;
    }
    prevOpen = open;
    
    mh->sendPin(pin, open);
}

#pragma mark ___________________________________________________________________
bool MasterHakoniwa::Scene::isEnabled() const
{
    return window[WINDOW_0] || window[WINDOW_1];
}

#pragma mark ___________________________________________________________________
MasterHakoniwa& MasterHakoniwa::instance()
{
    static MasterHakoniwa instance;
    return instance;
}

void MasterHakoniwa::setupUI(ofxUITabBar* tabbar)
{
    const float sizeBig{40.f};
    const float sizeMid{25.f};
    const float sizeSml{OFX_UI_GLOBAL_BUTTON_DIMENSION};
    const float w{kGuiWidth};
    const float h{kGuiHeight};
    tabbar->setWidth(w);
    tabbar->setColorBack(kBackgroundColor);
    tabbar->setPosition(kLineWidth, kTopOffset);
    tabbar->addToggle("[[[ Stop!!! ]]]", &mEmergencyStop, sizeBig, sizeBig);
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[Presets]", OFX_UI_FONT_SMALL);
    vector<string> presetNames;
    presetNames.push_back("Intro");
    presetNames.push_back("Maestro");
    presetNames.push_back("Outro");
    tabbar->addRadio("Presets", presetNames);
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[OSC]", OFX_UI_FONT_SMALL);
    tabbar->addToggle("Enable OSC to RAM DTK", &mEnableOscOutRDTK);
    tabbar->addToggle("Enable OSC to Master Hakoniwa", &mEnableOscOutMH);
    tabbar->addToggle("Enable OSC to Score", &mEnableOscOutScore);
    tabbar->addToggle("Change Scene with CameraUnit", &mEnableCameraUnit);
    tabbar->addToggle("Open Valve with MOTIONER", &mEnableMotioner);
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[RAM Dance Tool Kit]", OFX_UI_FONT_SMALL);
    ofxUICanvas* sceneSelectTab{new ofxUICanvas()};
    sceneSelectTab->setWidth(w);
    sceneSelectTab->setHeight(h);
    sceneSelectTab->setColorBack(MH::kBackgroundColor);
    
    sceneSelectTab->setName("Select Scene");
    sceneSelectTab->addLabel("Select Scene", OFX_UI_FONT_SMALL);
    sceneSelectTab->addSpacer();
    vector<string> sceneNames;
    sceneNames.push_back("disable all");
    for (auto& pair : mScenes) {
        sceneNames.push_back(pair.first);
    }
    sceneSelectTab->addRadio("Scenes", sceneNames);
    tabbar->addCanvas(sceneSelectTab);
    ofAddListener(sceneSelectTab->newGUIEvent, this, &MasterHakoniwa::guiEvent);
    
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[Score]", OFX_UI_FONT_SMALL);
    ofxUICanvas* scoreSelectTab{new ofxUICanvas()};
    scoreSelectTab->setWidth(w);
    scoreSelectTab->setHeight(h);
    scoreSelectTab->setColorBack(MH::kBackgroundColor);
    scoreSelectTab->setName("Select Score");
    scoreSelectTab->addLabel("Select Score", OFX_UI_FONT_SMALL);
    scoreSelectTab->addSpacer();
    vector<string> scoreNames;
    scoreNames.push_back(kScoreBlack);
    for (int i=0; i<mUniqueScores.size(); i++) {
        for (int j=0; j<mUniqueScores.at(i).getInitialList().size(); j++) {
            scoreNames.push_back(mUniqueScores.at(i).getInitialList().at(j));
        }
    }
    for (int i=0; i<mUniqueScoreBodies.getInitialList().size(); i++) {
        scoreNames.push_back(mUniqueScoreBodies.getInitialList().at(i));
    }
    scoreNames.push_back(mScoreCorrelation);
    
    scoreSelectTab->addRadio("Scores", scoreNames);
    tabbar->addCanvas(scoreSelectTab);
    ofAddListener(scoreSelectTab->newGUIEvent, this, &MasterHakoniwa::guiEvent);

    tabbar->addToggle("Display Hakoniwa title on Score", &mEnableShowHakoniwaTitle);
    tabbar->addSlider("Score Sensor scale", 1.f, 50.f, &mScoreSensorScale, w, kLineHeight);
    
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[CameraUnit]", OFX_UI_FONT_SMALL);
    vector<string> radioNames;
    radioNames.push_back("Use Mean");
    radioNames.push_back("Use Pixelate");
    tabbar->addRadio("Analyze", radioNames, OFX_UI_ORIENTATION_HORIZONTAL)->activateToggle("Use Mean");

    
    ofxUICanvas* meanTab{new ofxUICanvas};
    meanTab->setWidth(w);
    meanTab->setHeight(h);
    meanTab->setColorBack(MH::kBackgroundColor);
    meanTab->setName("Mean Settings");
    meanTab->addLabel("Mean Settings", OFX_UI_FONT_SMALL);
    meanTab->addSpacer();
    meanTab->addSlider("Limit", 0.1f, 100.f, &mAnalyzeMean.mMeanLimit, w - kMargin, kLineHeight);
    meanTab->addSlider("Min Time", 0.1f, 60.f * 3.f, &mAnalyzeMean.mMinSetSceneTime, w - kMargin, kLineHeight);
    tabbar->addCanvas(meanTab);
    
    ofxUICanvas* pixelataTab{new ofxUICanvas};
    pixelataTab->setWidth(w);
    pixelataTab->setHeight(h);
    pixelataTab->setColorBack(MH::kBackgroundColor);
    pixelataTab->setName("Pixelate Settings");
    pixelataTab->addLabel("Pixelate Settings", OFX_UI_FONT_SMALL);
    pixelataTab->addSpacer();
    pixelataTab->addSlider("Limit", 0.1f, 100.f, &mAnalyzePixelate.mLimit, w - kMargin, kLineHeight);
    pixelataTab->addSlider("Min Time", 0.1f, 60.f * 3.f, &mAnalyzePixelate.mMinSetSceneTime, w - kMargin, kLineHeight);
    pixelataTab->addSlider("Do Something Limit", 50.f, 1000.f, &mAnalyzePixelate.mDoSomethingLimit, w - kMargin, kLineHeight);
    pixelataTab->addToggle("Do Something", &mAnalyzePixelate.mDoSomething);
    pixelataTab->addToggle("Enable Do Something", &mAnalyzePixelate.mEnableDoSomething);
    tabbar->addCanvas(pixelataTab);
    
    tabbar->addSpacer(w, 1.f);
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
                                           &mPumps.at(1).open, sizeSml, sizeSml));
    tabbar->addSlider("Valve Open Duration", 0.f, 1.f, &mValveOpenDuration, w, kLineHeight);
    tabbar->addSpacer(w, 1.f);
    
    tabbar->addLabel("MOTIONER method settings", OFX_UI_FONT_SMALL);
    
    ofAddListener(tabbar->newGUIEvent, this, &MasterHakoniwa::guiEvent);
}

void MasterHakoniwa::initialize()
{
    ofxXmlSettings xml;
    xml.load(kXmlSettingsPath);
    
    mValves.assign(kNumValvePins, Valve());
    for (int i=0; i<mValves.size(); i++) {
        mValves.at(i).pin = kValvePins[i];
    }
    
    mPumps.assign(kNumPumpPins, Pump());
    for (int i=0; i<mPumps.size(); i++) {
        mPumps.at(i).pin = kPumpPins[i];
    }
    
    mMasterHakoniwaOscSender.setup(kHostNameMasterHakoniwa, kPortNumberMasterHakoniwa);
    mCameraUnitOscSender.setup(kHostNameCameraUnit, kPortNumberCameraUnit);
    mScoreOscSender.setup(kHostNameScore, kPortNumberScore);
    
    turnOffAllPins();
    
    xml.pushTag("rdtk");
    xml.pushTag("phase", 0);
    vector<string> sceneNames;
    for (int j=0; j<xml.getNumTags("scene"); j++) {
        const string name{xml.getAttribute("scene", "name", "error", j)};
        mScenes[name] = Scene();
        sceneNames.push_back(name);
    }
    xml.popTag();
    
    xml.pushTag("nocam");
        mNoCamScenes.clear();
    for (int j=0; j<xml.getNumTags("scene"); j++) {
        const string name{xml.getAttribute("scene", "name", "error", j)};
        mNoCamScenes.push_back(name);
    }
    xml.popTag();
    
    xml.pushTag("alloff");
    mAllOffScenes.clear();
    for (int j=0; j<xml.getNumTags("scene"); j++) {
        const string name{xml.getAttribute("scene", "name", "error", j)};
        mAllOffScenes.push_back(name);
    }
    xml.popTag();
    
    xml.popTag();
    mUniqueScenes.setInitialList(sceneNames);
    
    xml.pushTag("score");
    mMaxComplexity = xml.getNumTags("complexity");
    mUniqueScores.assign(mMaxComplexity, UniqueStringStack());
    for (int i=0; i<mMaxComplexity; i++) {
        xml.pushTag("complexity", i);
        auto& stack = mUniqueScores.at(i);
        const int numScenes{xml.getNumTags("scene")};
        vector<string> sceneNames;
        for (int j=0; j<numScenes; j++) {
            const string name{xml.getAttribute("scene", "name", "error", j)};
            sceneNames.push_back(name);
        }
        stack.setInitialList(sceneNames);
        xml.popTag();
    }
    xml.pushTag("body");
    vector<string> bodies;
    for (int i=0; i<xml.getNumTags("scene"); i++) {
        const string name{xml.getAttribute("scene", "name", "error", i)};
        bodies.push_back(name);
    }
    mUniqueScoreBodies.setInitialList(bodies);
    xml.popTag();
    
    xml.pushTag("correlation");
    mScoreCorrelation = xml.getAttribute("scene", "name", "error");
    xml.popTag();
    
    xml.popTag();
    
    mAnalyzePixelate.mMaster = false;
    mAnalyzeMean.mMaster = true;
    
    ofAddListener(ofxMot::drawSkeletonEvent,
                  this,
                  &MasterHakoniwa::onDrawSkeleton);
}

void MasterHakoniwa::shutdown()
{
    ofRemoveListener(ofxMot::drawSkeletonEvent,
                     this,
                     &MasterHakoniwa::onDrawSkeleton);
    
    mEnableOscOutMH = true;
    turnOffAllPins();
}

void MasterHakoniwa::update()
{
    if (mEmergencyStop) {
        mEnableMotioner = false;
        mEnableCameraUnit = false;
        mEnableOscOutMH = false;
        mEnableOscOutRDTK = false;
        mEnableOscOutScore = false;
        mEnableShowHakoniwaTitle = false;
        turnOffAllPins();
        turnOffAllScenes();
        mEmergencyStop = false;
    }
    
    if (ofGetFrameNum() % kUpdateFrames) return;
    
    for (int i=0; i<kNumValvePins; i++) {
        mValves[i].update(this);
    }
    
    for (int i=0; i<kNumPumpPins; i++) {
        mPumps[i].update(this);
    }
    
    if (mEnableCameraUnit) {
        mAnalyzeMean.update();
        mAnalyzePixelate.update();
    }
}

void MasterHakoniwa::updateCameraUnit(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitMean) {
        if (m.getNumArgs() == ofVec4f::DIM) {
            ofVec4f mean;
            for (int i=0; i<ofVec4f::DIM; i++) {
                mean[i] = m.getArgAsInt32(i);
            }
            mAnalyzeMean.mMean = mean;
        }
    }
    else if (m.getAddress() == kOscAddrCameraUnitPixelateR) {
        mAnalyzePixelate.updateColor(AnalyzePixelate::Color::R, m);
    }
    else if (m.getAddress() == kOscAddrCameraUnitPixelateG) {
        mAnalyzePixelate.updateColor(AnalyzePixelate::Color::G, m);
    }
    else if (m.getAddress() == kOscAddrCameraUnitPixelateB) {
        mAnalyzePixelate.updateColor(AnalyzePixelate::Color::B, m);
    }
}

void MasterHakoniwa::draw()
{
    const float x{kLineWidth + kGuiWidth + kMargin * 2.f + 3.f};
    const float y{kGuiHeight + kMargin + kTopOffset};
    mCamViewport.set(x,
                     y,
                     (ofGetWidth() - x) * 0.5f - kMargin * 2.f,
                     ofGetHeight() - y - kMargin * 2.f);
    
    mTextLeftCorner.set(mCamViewport.x + mCamViewport.width + kMargin * 2.f,
                        mCamViewport.y + kTextSpacing);
    
    const float t{ofGetElapsedTimef()};
    
    ofFill();
    ofSetColor(255, 0, 0, 20);
    ofRect(kMargin,
           kTopOffset - kMargin,
           ofGetWidth() - kMargin * 2.f,
           ofGetHeight() - kTopOffset);
    
    ofPushMatrix();
    alignedTranslate(kTextSpacing, kTopOffset);
    
    ofSetColor(kBackgroundColor);
    ofRect(0.f,
           0.f,
           kLineWidth - kTextSpacing - kMargin,
           ofGetHeight() - kTopOffset - kMargin * 2.f);
    
    ofSetColor(kTextColor);
    alignedTranslate(kMargin, kTextSpacing);
    
    ofSetColor(kTextColor);
    ofDrawBitmapString("[valves]", ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    
    int i{0};
    for (auto& v : mValves) {
        v.open ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
        stringstream ss;
        ss << i << " = " << (v.open ? "on " : "off")
        << setprecision(2) << fixed
        << " : " << (v.open ? t - v.openedTime : 0.f)
        << " / " << mValveOpenDuration
        << " | " << (!v.open ? t - v.closedTime : 0.f);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        alignedTranslate(0.f, kTextSpacing);
        i++;
    }
    
    ofSetColor(kTextColor);
    alignedTranslate(0.f, kTextSpacing);
    ofDrawBitmapString("[pumps]", ofPoint::zero());
    
    alignedTranslate(0.f, kTextSpacing);
    i = 0;
    for (auto& p : mPumps) {
        p.open ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
        stringstream ss;
        ss << i << " = " << (p.open ? "on " : "off")
        << setprecision(1) << fixed
        << " : " << (p.open ? t - p.openedTime : 0.f)
        << " | " << (!p.open ? t - p.closedTime : 0.f);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        alignedTranslate(0.f, kTextSpacing);
        i++;
    }
    
    ofSetColor(kTextColor);
    alignedTranslate(0.f, kTextSpacing);
    ofDrawBitmapString("[scenes]", ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    for (auto& pair : mScenes) {
        const string name{pair.first};
        auto& s = pair.second;
        
        auto findIt = mUniqueScenes.find(name);
        if (s.isEnabled()) ofSetColor(color::kMain);
        else if (findIt != mUniqueScenes.end()) ofSetColor(kTextColor);
        else ofSetColor(kTextColorDark);
        
        ofDrawBitmapString(name, ofPoint::zero());
        stringstream ss;
        ss << "= " << (s.isEnabled() ? "on " : "off") << " : ";
        for (int i=0; i<NUM_WINDOWS; i++) {
            ss << (int)s.window[i];
            if (i < NUM_WINDOWS - 1) ss << ", ";
        }
        ofDrawBitmapString(ss.str(), ofPoint(kTextSpacing * 15.f, 0.f));
        alignedTranslate(0.f, kTextSpacing);
    }
    alignedTranslate(0.f, kTextSpacing);
    
    ofSetColor(kTextColor);
    ofDrawBitmapString("[score]", ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    mCurrentScore == kScoreBlack ? ofSetColor(color::kMain) : ofSetColor(kTextColor);
    ofDrawBitmapString(kScoreBlack, ofPoint::zero());
    
    for (int i=0; i<mUniqueScores.size(); i++) {
        auto& stack = mUniqueScores.at(i);
        alignedTranslate(0.f, kTextSpacing);
        
        ofSetColor(kTextColor);
        ofDrawBitmapString("- complexity " + ofToString(i) + " -", ofPoint::zero());
        alignedTranslate(0.f, kTextSpacing);
        for (int j=0; j < stack.getInitialList().size(); j++) {
            const auto& s = stack.getInitialList().at(j);
            auto findIt = stack.find(s);
            if (s == mCurrentScore) ofSetColor(color::kMain);
            else if (findIt != stack.end()) ofSetColor(kTextColor);
            else ofSetColor(kTextColorDark);
            string ss{s};
            ofStringReplace(ss, "dp::score::Scene", "");
            ofDrawBitmapString(ss, ofPoint::zero());
            alignedTranslate(0.f, kTextSpacing);
        }
    }
    alignedTranslate(0.f, kTextSpacing);
    
    ofSetColor(kTextColor);
    ofDrawBitmapString("- body -", ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    auto& stack = mUniqueScoreBodies;
    for (int i=0; i<stack.getInitialList().size(); i++) {
        const auto& s = stack.getInitialList().at(i);
        auto findIt = stack.find(s);
        if (s == mCurrentScore) ofSetColor(color::kMain);
        else if (findIt != stack.end()) ofSetColor(kTextColor);
        else ofSetColor(kTextColorDark);
        string ss{s};
        ofStringReplace(ss, "dp::score::Scene", "");
        ofDrawBitmapString(ss, ofPoint::zero());
        alignedTranslate(0.f, kTextSpacing);
    }
    
    alignedTranslate(0.f, kTextSpacing);
    if (mCurrentScore == mScoreCorrelation) ofSetColor(color::kMain);
    else ofSetColor(kTextColor);
    string sc{mScoreCorrelation};
    ofStringReplace(sc, "dp::score::Scene", "");
    ofDrawBitmapString(sc, ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    
    ofPopMatrix();
    

    ofPushMatrix();
    
    alignedTranslate(kLineWidth + kGuiWidth * 2.f + kMargin * 3.f - 1.f, kTopOffset);
    ofSetColor(kBackgroundColor);
    alignedRect(0.f,
                0.f,
                ofGetWidth() - (kLineWidth + kGuiWidth * 2.f  + kMargin * 6.f),
                kGuiHeight);
    
    alignedTranslate(kMargin, 0.f);
    ofSetColor(kTextColor);
    alignedTranslate(0.f, kTextSpacing);
    
    ofPushMatrix();
    alignedTranslate(0.f, kTextSpacing);
    mAnalyzeMean.draw();
    ofPopMatrix();
    
    alignedTranslate(0.f, kTextSpacing * 12.f);
    
    ofPushMatrix();
    mAnalyzePixelate.draw();
    ofPopMatrix();
    
    ofPopMatrix();
    
    
    ofSetColor(kBackgroundColor);
    ofRect(mCamViewport);
    ofPushMatrix();
    alignedTranslate(mCamViewport.width + kMargin, 0.f);
    ofRect(mCamViewport);
    ofPopMatrix();
    
    ofPushStyle();
    ofEnableDepthTest();
    mCam.begin(mCamViewport);
    alignedTranslate(0.f, -100.f);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRotateX(90.f);
    ofSetColor(kBackgroundColor, 5);
    ofRect(ofPoint::zero(), 10000, 1000);
    mCam.end();
    ofPopStyle();
}

void MasterHakoniwa::turnOnValve(int index)
{
    if (!mEnableMotioner) return;
    
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException, "valve index %d is out of range", index);
    mValves.at(index).open = true;
}

void MasterHakoniwa::turnOffAllPins()
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

bool MasterHakoniwa::getIsOpeningValve(int index)
{
    if (index <0 || index >= mValves.size())
        ofxThrowExceptionf(ofxException, "valve index %d is out of range", index);
    return mValves.at(index).open;
}

void MasterHakoniwa::setUniqueScene(int sceneIndex, bool win0, bool win1)
{
    if (!getIsWindowOn(0)) win0 = true;
    if (!getIsWindowOn(1)) win1 = true;
    
    if (!win0 && !win1) win0 = win1 = true;
    
    const string sceneName{mUniqueScenes.get(sceneIndex)};
    
    sendSetScene(sceneName, win0, win1);
}

void MasterHakoniwa::setUniqueScore(int sceneIndex)
{
    if (mCurrentScoreComplexity < 0 || mCurrentScoreComplexity >= mMaxComplexity) {
        ofxThrowExceptionf(ofxException,
                           "score complexity %d out of range",
                           mCurrentScoreComplexity);
    }
    
    auto& stack = mUniqueScores.at(mCurrentScoreComplexity);
    
    if (mAllOffScene) {
        sendChangeScore(kScoreBlack, mEnableShowHakoniwaTitle);
        return;
    }
    
    if (mNoCameraData) {
        const int newIndex{(int)ofMap(sceneIndex, 0.f, stack.size(), 0.f, mUniqueScoreBodies.size())};
        sendChangeScore(mUniqueScoreBodies.get(newIndex), mEnableShowHakoniwaTitle);
        return;
    }
    
    const string& s{stack.get(sceneIndex)};
    
    sendChangeScore(s, mEnableShowHakoniwaTitle);
    
    ++mCurrentScoreComplexity %= mMaxComplexity;
}

size_t MasterHakoniwa::getNumUniqueScores() const
{
    if (mCurrentScoreComplexity < 0 || mCurrentScoreComplexity >= mMaxComplexity) {
        ofxThrowExceptionf(ofxException,
                           "score complexity %d out of range",
                           mCurrentScoreComplexity);
    }
    
    auto& stack = mUniqueScores.at(mCurrentScoreComplexity);
    
    return stack.size();
}

bool MasterHakoniwa::getIsWindowOn(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= NUM_WINDOWS) {
        ofxThrowExceptionf(ofxException, "window index %d out of range", windowIndex);
    }
    
    bool on{false};
    for (auto& pair : mScenes) {
        if (pair.second.window[windowIndex]) on = true;
    }
    return on;
}

void MasterHakoniwa::turnOffAllScenes()
{
    for (auto& pair : mScenes) {
        const string& name{pair.first};
        const auto&s = pair.second;
        sendSetScene(name, false, false);
    }
}

#pragma mark ___________________________________________________________________
void MasterHakoniwa::sendPin(int pin, bool open)
{
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
    m.addIntArg((int)open);
    if (mEnableOscOutMH) mMasterHakoniwaOscSender.sendMessage(m);
    //if (open)
    //    cout << pin << "=" << open << endl;
}

void MasterHakoniwa::sendSetScene(const string& name, bool win0, bool win1)
{
    if (mAllOffScene) win0 = win1 = true;
    
    auto& scene = mScenes[name];
    if (scene.window[WINDOW_0] == win0 && scene.window[WINDOW_1] == win1) {
        return;
    }
    scene.window[WINDOW_0] = win0;
    scene.window[WINDOW_1] = win1;
    scene.dirty = true;
    
    auto it = find(mAllOffScenes.begin(), mAllOffScenes.end(), name);
    mAllOffScene = (it != mAllOffScenes.end());
    if (mAllOffScene) {
        scene.window[WINDOW_0] = scene.window[WINDOW_1] = true;
    }
    
    for (auto& pair : mScenes) {
        if (pair.first != name) {
            for (int i=0; i<NUM_WINDOWS; i++) {
                if (scene.window[i] && pair.second.window[i]) {
                    pair.second.window[i] = false;
                    pair.second.dirty = true;
                }
            }
            if (!pair.second.dirty) continue;
        
            ofxOscMessage m;
            m.setAddress(kOscAddrRamSetScene);
            m.addStringArg(pair.first);
            m.addIntArg((int)pair.second.isEnabled());
            
            for (int i=0; i<NUM_WINDOWS; i++) {
                m.addIntArg((int)pair.second.window[i]);
            }
            if (mEnableOscOutRDTK) mCameraUnitOscSender.sendMessage(m);
        }
    }
    
    if (scene.isEnabled()) {
        auto it = find(mNoCamScenes.begin(), mNoCamScenes.end(), name);
        mNoCameraData = (it != mNoCamScenes.end());
    }
    
    ofxOscMessage m;
    m.setAddress(kOscAddrRamSetScene);
    m.addStringArg(name);
    m.addIntArg((int)scene.isEnabled());
    
    for (int i=0; i<NUM_WINDOWS; i++) {
        m.addIntArg((int)scene.window[i]);
    }
    if (mEnableOscOutRDTK) mCameraUnitOscSender.sendMessage(m);
    
    // write scene times
    vector<string> enabledSceneNames;
    for (auto& pair : mScenes) {
        if (pair.second.isEnabled()) {
            enabledSceneNames.push_back(pair.first);
        }
    }
    
    if (enabledSceneNames.empty() == false) {
        const float sceneElapsedTime{ofGetElapsedTimef() - mPrevTimeSceneChanged};
        mSceneTimesBuffer.append(ofToString(sceneElapsedTime, 2));
        mSceneTimesBuffer.append("\n");
        for (int i=0; i<enabledSceneNames.size(); i++) {
            mSceneTimesBuffer.append(enabledSceneNames.at(i));
            mSceneTimesBuffer.append(", ");
        }
        
        mPrevTimeSceneChanged = ofGetElapsedTimef();
    }
}

void MasterHakoniwa::sendChangeScore(const string& name, bool maintainSceneNames)
{
    ofxOscMessage m;
    m.setAddress(kOscAddrChangeScene);
    m.addStringArg(name);
    mCurrentScore = name;
    
    if (mAllOffScene)
        maintainSceneNames = true;
    else if (mCurrentScore == mScoreCorrelation || mCurrentScore == kScoreBlack)
        maintainSceneNames = false;
    
    if (maintainSceneNames) {
        for (auto& pair : mScenes) {
            if (pair.second.isEnabled()) {
                m.addStringArg(pair.first);
            }
        }
    }
    
    if (mEnableOscOutScore) mScoreOscSender.sendMessage(m);
}

void MasterHakoniwa::onDrawSkeleton(ofxMotioner::EventArgs &e)
{
    mCam.begin(mCamViewport);
    ofEnableDepthTest();
    ofPushMatrix();
    ofPushStyle();
    alignedTranslate(0.f, -100.f + 8.f);
    
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

void MasterHakoniwa::guiEvent(ofxUIEventArgs& e)
{
    const string& widgetName{e.widget->getName()};
    
    if (widgetName == "Analyze") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (toggleName == "Use Mean") {
            mAnalyzeMean.mMaster = true;
            mAnalyzePixelate.mMaster = false;
            ofLogNotice() << "Cnahged analyze type to mean";
        }
        else if (toggleName == "Use Pixelate") {
            mAnalyzeMean.mMaster = false;
            mAnalyzePixelate.mMaster = true;
            ofLogNotice() << "Cnahged analyze type to pixelate";
        }
    }
    else if (widgetName == "Scenes") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue()) {
            if (toggleName == "disable all") {
                turnOffAllScenes();
            }
            else {
                sendSetScene(toggleName, true, true);
            }
        }
    }
    else if (widgetName == "Scores") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue()) {
            sendChangeScore(toggleName, mEnableShowHakoniwaTitle);
        }
    }
    else if (widgetName == "Presets") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue()) {
            if (toggleName == "Maestro") {
                mEnableOscOutMH = true;
                mEnableOscOutRDTK = true;
                mEnableMotioner = true;
                mEnableCameraUnit = true;
                mEnableOscOutScore = true;
                mEnableShowHakoniwaTitle = true;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mAnalyzeMean.mMeanLimit = 10.f;
                mAnalyzeMean.mMinSetSceneTime = 90.f;
            }
            else if (toggleName == "Intro") {
                mEnableOscOutMH = false;
                mEnableOscOutRDTK = false;
                mEnableMotioner = false;
                mEnableCameraUnit = false;
                mEnableOscOutScore = true;
                mEnableShowHakoniwaTitle = false;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                sendChangeScore("dp::score::SceneVec2SimpleGraph");
                
                mSceneTimesBuffer.clear();
            }
            else if (toggleName == "Outro") {
                mEnableOscOutMH = true;
                mEnableOscOutRDTK = false;
                mEnableMotioner = true;
                mEnableCameraUnit = true;
                mEnableOscOutScore = true;
                mEnableShowHakoniwaTitle = false;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mAnalyzeMean.mMeanLimit = 3.f;
                mAnalyzeMean.mMinSetSceneTime = 30.f;
                

                const float sceneElapsedTime{ofGetElapsedTimef() - mPrevTimeSceneChanged};
                mSceneTimesBuffer.append(ofToString(sceneElapsedTime, 2));
                string fileName{"log/scene times-" + ofGetTimestampString() + ".txt"};
                ofBufferToFile(fileName, mSceneTimesBuffer);
            }
        }
    }
    else if (widgetName == "Score Sensor scale") {
        auto* slider =static_cast<ofxUISlider*>(e.widget);
        const float value{slider->getScaledValue()};
        if (mEnableOscOutScore) {
            ofxOscMessage m;
            m.setAddress(kOscAddrSensorScale);
            m.addFloatArg(value);
            mScoreOscSender.sendMessage(m);
        }
    }
}

void MasterHakoniwa::doSomething(int rand)
{
    ofxOscMessage m;
    m.setAddress(kOscAddrRamDoSomething);
    m.addIntArg(rand);
    
    if (mEnableOscOutRDTK) {
        mCameraUnitOscSender.sendMessage(m);
    }
    if (mEnableOscOutScore) {
        mScoreOscSender.sendMessage(m);
    }
}

DP_SCORE_NAMESPACE_END