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

const string MH::kOscAddrRamSetScene{"/ram/set_scene"};
const string MH::kOscAddrRamDoSomething{"/ram/do_something"};

const string MH::kScoreBlack{"black"};

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
    tabbar->addToggle("Enable OSC to Master Hakoniwa",
                      &mColorOfWater.enableOscOut);
    tabbar->addToggle("Enable OSC to Score", &mEnableOscOutScore);
    tabbar->addToggle("Change Scene with CameraUnit", &mEnableCameraUnit);
    tabbar->addToggle("Open Valve with MOTIONER",
                      &mColorOfWater.enableOpenValve);
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[RAM Dance Tool Kit]", OFX_UI_FONT_SMALL);
    ofxUICanvas* sceneSelectTab{new ofxUICanvas()};
    sceneSelectTab->setColorBack(MH::kBackgroundColor);
    sceneSelectTab->setWidth(w);
    sceneSelectTab->addButton("Send Scene OSC", false);
    sceneSelectTab->addSpacer();
    sceneSelectTab->addToggle("Screen A", &mUISceneInfo.screenA);
    sceneSelectTab->addToggle("Screen B", &mUISceneInfo.screenB);
    mUISceneInfo.textInput = sceneSelectTab->addTextInput("Scene Name", "");
    if (mUISceneInfo.textInput)
        mUISceneInfo.textInput->setAutoClear(false);
    sceneSelectTab->addButton("Disable All Scenes", false);
    sceneSelectTab->setName("Select Scene");
    sceneSelectTab->addLabel("Select Scene", OFX_UI_FONT_SMALL);
    
    sceneSelectTab->addSpacer();
    vector<string> sceneNames;
    for (auto& pair : mScenes) {
        sceneNames.push_back(pair.first);
    }
    sceneSelectTab->addRadio("Scenes", sceneNames);
    sceneSelectTab->autoSizeToFitWidgets();
    sceneSelectTab->setWidth(w);
    tabbar->addCanvas(sceneSelectTab);
    ofAddListener(sceneSelectTab->newGUIEvent, this, &MasterHakoniwa::guiEvent);
    
    tabbar->addSpacer(w, 1.f);
    tabbar->addLabel("[Score]", OFX_UI_FONT_SMALL);
    ofxUICanvas* scoreSelectTab{new ofxUICanvas()};
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
    scoreSelectTab->autoSizeToFitWidgets();
    scoreSelectTab->setWidth(w);
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
    mColorOfWater.setupGui(tabbar);
    
    tabbar->addLabel("MOTIONER method settings", OFX_UI_FONT_SMALL);
    
    ofAddListener(tabbar->newGUIEvent, this, &MasterHakoniwa::guiEvent);
}

void MasterHakoniwa::initialize()
{
    mColorOfWater.initialize();
    
    ofxXmlSettings xml;
    xml.load(kXmlSettingsPath);
    
OFX_BEGIN_EXCEPTION_HANDLING
    xml.pushTag("osc");
    string errorStr{"error"};
    int errorInt{-1};
    const string cuHost{xml.getAttribute("serverCameraUnit", "host", errorStr)};
    if (cuHost == errorStr)
        ofxThrowException(ofxException,
                          "serverCameraUnit host name didn't find in XML");
    const int cuPort{xml.getAttribute("serverCameraUnit", "port", errorInt)};
    if (cuPort == errorInt)
        ofxThrowException(ofxException,
                          "serverCameraUnit port number didn't find in XML");
    
    const string scHost{xml.getAttribute("serverScore", "host", errorStr)};
    if (scHost == errorStr)
        ofxThrowException(ofxException,
                          "serverScore host name didn't find in XML");
    const int scPort{xml.getAttribute("serverScore", "port", errorInt)};
    if (scPort == errorInt)
        ofxThrowException(ofxException,
                          "serverScore port number didn't find in XML");
    
    mCameraUnitOscSender.setup(cuHost, cuPort);
    mScoreOscSender.setup(scHost, scPort);
    xml.popTag();
OFX_END_EXCEPTION_HANDLING
    
    xml.pushTag("rdtk");
    vector<string> maestroSceneNames;
    for (int j=0; j<xml.getNumTags("scene"); j++) {
        const string name{xml.getAttribute("scene", "name", "error", j)};
        const bool hasCamera{(bool)xml.getAttribute("scene", "has_camera", 0, j)};
        const bool maestro{(bool)xml.getAttribute("scene", "maestro", 0, j)};
        const bool allOff{(bool)xml.getAttribute("scene", "all_off", 0, j)};
        mScenes[name] = Scene();
        mScenes[name].hasCamera = hasCamera;
        mScenes[name].maestro = maestro;
        mScenes[name].allOff = allOff;
        if (maestro)
            maestroSceneNames.push_back(name);
    }
    xml.popTag();
    mUniqueScenes.setInitialList(maestroSceneNames);
    
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
    
    mColorOfWater.shutdown();
}

void MasterHakoniwa::update()
{
    if (mEmergencyStop) {
        mColorOfWater.stopAll();
        
        mEnableCameraUnit = false;
        mEnableOscOutRDTK = false;
        mEnableOscOutScore = false;
        mEnableShowHakoniwaTitle = false;
        turnOffAllScenes();
        mEmergencyStop = false;
    }
    
    if (ofGetFrameNum() % kUpdateFrames) return;
    
    mColorOfWater.update();
    
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
    
    mColorOfWater.draw();
    
    ofSetColor(kTextColor);
    alignedTranslate(0.f, kTextSpacing);
    ofDrawBitmapString("[scenes]", ofPoint::zero());
    alignedTranslate(0.f, kTextSpacing);
    for (auto& pair : mScenes) {
        const string name{pair.first};
        auto& s = pair.second;
        if (!s.maestro) continue;
        
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
    
    if (mLatestScene.allOff) {
        sendChangeScore(kScoreBlack, mEnableShowHakoniwaTitle);
        return;
    }
    
    if (!mLatestScene.hasCamera) {
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
void MasterHakoniwa::sendSetScene(const string& name, bool win0, bool win1)
{
    auto& scene = mScenes[name];
    
    if (win0 || win1)
        if (scene.allOff || mLatestScene.allOff)
            win0 = win1 = true;
    
    if (scene.window[WINDOW_0] == win0 && scene.window[WINDOW_1] == win1) return;
    
    scene.window[WINDOW_0] = win0;
    scene.window[WINDOW_1] = win1;
    scene.dirty = true;
    
    mLatestScene = scene;
    
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
    
    if (!mLatestScene.allOff)
        if (mCurrentScore == mScoreCorrelation || mCurrentScore == kScoreBlack)
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
    else if (widgetName == "Send Scene OSC") {
        if (mUISceneInfo.textInput)
        sendSetScene(mUISceneInfo.textInput->getTextString(),
                     mUISceneInfo.screenA,
                     mUISceneInfo.screenB);
    }
    else if (widgetName == "Disable All Scenes") {
        mUISceneInfo.screenA = mUISceneInfo.screenB = false;
        if (mUISceneInfo.textInput)
            mUISceneInfo.textInput->setTextString("");
        turnOffAllScenes();
    }
    else if (widgetName == "Scenes") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue()) {
            if (mUISceneInfo.textInput)
                mUISceneInfo.textInput->setTextString(toggleName);
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
                mColorOfWater.enableOscOut = true;
                mColorOfWater.enableOpenValve = true;
                mEnableOscOutRDTK = true;
                mEnableCameraUnit = true;
                mEnableOscOutScore = true;
                mEnableShowHakoniwaTitle = true;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mAnalyzeMean.mMeanLimit = 10.f;
                mAnalyzeMean.mMinSetSceneTime = 90.f;
            }
            else if (toggleName == "Intro") {
                mColorOfWater.enableOscOut = false;
                mColorOfWater.enableOpenValve = false;
                mEnableOscOutRDTK = false;
                mEnableCameraUnit = false;
                mEnableOscOutScore = true;
                mEnableShowHakoniwaTitle = false;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                sendChangeScore("dp::score::SceneVec2SimpleGraph");
                
                mSceneTimesBuffer.clear();
            }
            else if (toggleName == "Outro") {
                mColorOfWater.enableOscOut = true;
                mColorOfWater.enableOpenValve = true;
                
                mEnableOscOutRDTK = false;
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