//
//  dpScoreSceneController.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/15.
//
//

#include "dpScoreSceneController.h"
#include "dpScoreMasterHakoniwa.h"
#include "ofxUI.h"

DP_SCORE_NAMESPACE_BEGIN

const string SceneController::kOscAddrRamSetScene{"/ram/set_scene"};
const string SceneController::kOscAddrRamDoSomething{"/ram/do_something"};

//const string SceneController::kScoreBlack{"black"};

#pragma mark ___________________________________________________________________
bool SceneController::Scene::isEnabled() const
{
    return window[WINDOW_0] || window[WINDOW_1];
}

#pragma mark ___________________________________________________________________
void SceneController::initialize()
{
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
    
    /*
    const string scHost{xml.getAttribute("serverScore", "host", errorStr)};
    if (scHost == errorStr)
        ofxThrowException(ofxException,
                          "serverScore host name didn't find in XML");
    const int scPort{xml.getAttribute("serverScore", "port", errorInt)};
    if (scPort == errorInt)
        ofxThrowException(ofxException,
                          "serverScore port number didn't find in XML");
    */
    mCameraUnitOscSender.setup(cuHost, cuPort);
    //mScoreOscSender.setup(scHost, scPort);
    xml.popTag();
    OFX_END_EXCEPTION_HANDLING
    
    loadScenes(xml);
}

void SceneController::loadScenes(ofxXmlSettings& xml)
{
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
    xml.popTag(); // rdtk
    
    mUniqueScenes.setInitialList(maestroSceneNames);
    
    /*
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
        xml.popTag(); // complexity
    }
    xml.pushTag("body");
    vector<string> bodies;
    for (int i=0; i<xml.getNumTags("scene"); i++) {
        const string name{xml.getAttribute("scene", "name", "error", i)};
        bodies.push_back(name);
    }
    mUniqueScoreBodies.setInitialList(bodies);
    xml.popTag(); // body
    
    xml.pushTag("correlation");
    mScoreCorrelation = xml.getAttribute("scene", "name", "error");
    xml.popTag(); // correlation
    
    xml.popTag(); // score
     */
}

void SceneController::setupUI(ofxUITabBar* tabbar)
{
    tabbar->addLabel("[RAM Dance Tool Kit]", OFX_UI_FONT_SMALL);
    ofxUICanvas* sceneSelectTab{new ofxUICanvas()};
    sceneSelectTab->setColorBack(MH::kBackgroundColor);
    sceneSelectTab->setWidth(MH::kGuiWidth);
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
    sceneSelectTab->setWidth(MH::kGuiWidth);
    tabbar->addCanvas(sceneSelectTab);
    ofAddListener(sceneSelectTab->newGUIEvent,
                  this,
                  &SceneController::guiEvent);
    
    tabbar->addSpacer(MH::kGuiWidth, 1.f);
    
    /*
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
    scoreSelectTab->setWidth(MH::kGuiWidth);
    tabbar->addCanvas(scoreSelectTab);
    ofAddListener(scoreSelectTab->newGUIEvent,
                  this,
                  &SceneController::guiEvent);
    
    tabbar->addToggle("Display Hakoniwa title on Score",
                      &enableShowHakoniwaTitle);
    tabbar->addSlider("Score Sensor scale",
                      1.f,
                      50.f,
                      &mScoreSensorScale,
                      MH::kGuiWidth,
                      MH::kLineHeight);
    
    tabbar->addSpacer(MH::kGuiWidth, 1.f);
     */
}

void SceneController::draw()
{
    ofSetColor(MH::kTextColor);
    alignedTranslate(0.f, MH::kTextSpacing);
    ofDrawBitmapString("[scenes]", ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing);
    for (auto& pair : mScenes) {
        const string name{pair.first};
        auto& s = pair.second;
        if (!s.maestro) continue;
        
        auto findIt = mUniqueScenes.find(name);
        if (s.isEnabled()) ofSetColor(color::kMain);
        else if (findIt != mUniqueScenes.end()) ofSetColor(MH::kTextColor);
        else ofSetColor(MH::kTextColorDark);
        
        ofDrawBitmapString(name, ofPoint::zero());
        stringstream ss;
        ss << "= " << (s.isEnabled() ? "on " : "off") << " : ";
        for (int i=0; i<NUM_WINDOWS; i++) {
            ss << (int)s.window[i];
            if (i < NUM_WINDOWS - 1) ss << ", ";
        }
        ofDrawBitmapString(ss.str(), ofPoint(MH::kTextSpacing * 15.f, 0.f));
        alignedTranslate(0.f, MH::kTextSpacing);
    }
    alignedTranslate(0.f, MH::kTextSpacing);
    
    /*
    ofSetColor(MH::kTextColor);
    ofDrawBitmapString("[score]", ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing);
    mCurrentScore == kScoreBlack ? ofSetColor(color::kMain) : ofSetColor(MH::kTextColor);
    ofDrawBitmapString(kScoreBlack, ofPoint::zero());
    
    for (int i=0; i<mUniqueScores.size(); i++) {
        auto& stack = mUniqueScores.at(i);
        alignedTranslate(0.f, MH::kTextSpacing);
        
        ofSetColor(MH::kTextColor);
        ofDrawBitmapString("- complexity " + ofToString(i) + " -", ofPoint::zero());
        alignedTranslate(0.f, MH::kTextSpacing);
        for (int j=0; j < stack.getInitialList().size(); j++) {
            const auto& s = stack.getInitialList().at(j);
            auto findIt = stack.find(s);
            if (s == mCurrentScore) ofSetColor(color::kMain);
            else if (findIt != stack.end()) ofSetColor(MH::kTextColor);
            else ofSetColor(MH::kTextColorDark);
            string ss{s};
            ofStringReplace(ss, "dp::score::Scene", "");
            ofDrawBitmapString(ss, ofPoint::zero());
            alignedTranslate(0.f, MH::kTextSpacing);
        }
    }
    alignedTranslate(0.f, MH::kTextSpacing);
    
    ofSetColor(MH::kTextColor);
    ofDrawBitmapString("- body -", ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing);
    auto& stack = mUniqueScoreBodies;
    for (int i=0; i<stack.getInitialList().size(); i++) {
        const auto& s = stack.getInitialList().at(i);
        auto findIt = stack.find(s);
        if (s == mCurrentScore) ofSetColor(color::kMain);
        else if (findIt != stack.end()) ofSetColor(MH::kTextColor);
        else ofSetColor(MH::kTextColorDark);
        string ss{s};
        ofStringReplace(ss, "dp::score::Scene", "");
        ofDrawBitmapString(ss, ofPoint::zero());
        alignedTranslate(0.f, MH::kTextSpacing);
    }
    
    alignedTranslate(0.f, MH::kTextSpacing);
    if (mCurrentScore == mScoreCorrelation) ofSetColor(color::kMain);
    else ofSetColor(MH::kTextColor);
    string sc{mScoreCorrelation};
    ofStringReplace(sc, "dp::score::Scene", "");
    ofDrawBitmapString(sc, ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing);
    */
}

void SceneController::guiEvent(ofxUIEventArgs& e)
{
    const string& widgetName{e.widget->getName()};
    
    if (widgetName == "Send Scene OSC") {
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
    /*
    else if (widgetName == "Scores") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue())
            sendChangeScore(toggleName, enableShowHakoniwaTitle);
    }
    else if (widgetName == "Score Sensor scale") {
        auto* slider =static_cast<ofxUISlider*>(e.widget);
        const float value{slider->getScaledValue()};
        if (enableOscOutScore) {
            ofxOscMessage m;
            m.setAddress(kOscAddrSensorScale);
            m.addFloatArg(value);
            mScoreOscSender.sendMessage(m);
        }
    }
     */
}

void SceneController::stopAll()
{
    //enableShowHakoniwaTitle = false;
    
    turnOffAllScenes();
    //sendChangeScore(kScoreBlack);
    
    enableOscOutRDTK = false;
    //enableOscOutScore = false;
}

void SceneController::setUniqueScene(int sceneIndex, bool win0, bool win1)
{
    if (!getIsWindowOn(0)) win0 = true;
    if (!getIsWindowOn(1)) win1 = true;
    
    if (!win0 && !win1) win0 = win1 = true;
    
    const string sceneName{mUniqueScenes.get(sceneIndex)};
    
    sendSetScene(sceneName, win0, win1);
}

/*
void SceneController::setUniqueScore(int sceneIndex)
{
    if (mCurrentScoreComplexity < 0 ||
        mCurrentScoreComplexity >= mMaxComplexity)
        ofxThrowExceptionf(ofxException,
                           "score complexity %d out of range",
                           mCurrentScoreComplexity);
    
    auto& stack = mUniqueScores.at(mCurrentScoreComplexity);
    
    if (mLatestScene.allOff) {
        sendChangeScore(kScoreBlack, enableShowHakoniwaTitle);
        return;
    }
    
    if (!mLatestScene.hasCamera) {
        const int newIndex{(int)ofMap(sceneIndex,
                                      0.f,
                                      stack.size(),
                                      0.f,
                                      mUniqueScoreBodies.size())};
        sendChangeScore(mUniqueScoreBodies.get(newIndex),
                        enableShowHakoniwaTitle);
        return;
    }
    
    const string& s{stack.get(sceneIndex)};
    
    sendChangeScore(s, enableShowHakoniwaTitle);
    
    ++mCurrentScoreComplexity %= mMaxComplexity;
}

size_t SceneController::getNumUniqueScores() const
{
    if (mCurrentScoreComplexity < 0 ||
        mCurrentScoreComplexity >= mMaxComplexity)
        ofxThrowExceptionf(ofxException,
                           "score complexity %d out of range",
                           mCurrentScoreComplexity);
    
    auto& stack = mUniqueScores.at(mCurrentScoreComplexity);
    
    return stack.size();
}
*/

bool SceneController::getIsWindowOn(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= NUM_WINDOWS)
        ofxThrowExceptionf(ofxException,
                           "window index %d out of range",
                           windowIndex);
    
    bool on{false};
    for (auto& pair : mScenes) {
        if (pair.second.window[windowIndex]) on = true;
    }
    return on;
}

void SceneController::turnOffAllScenes()
{
    for (auto& pair : mScenes) {
        const string& name{pair.first};
        const auto&s = pair.second;
        sendSetScene(name, false, false);
    }
}

void SceneController::sendSetScene(const string& name, bool win0, bool win1)
{
    auto& scene = mScenes[name];
    
    if (win0 || win1)
        if (scene.allOff || mLatestScene.allOff)
            win0 = win1 = true;
    
    if (scene.window[WINDOW_0] == win0 && scene.window[WINDOW_1] == win1)
        return;
    
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
            if (enableOscOutRDTK) mCameraUnitOscSender.sendMessage(m);
        }
    }
    
    ofxOscMessage m;
    m.setAddress(kOscAddrRamSetScene);
    m.addStringArg(name);
    m.addIntArg((int)scene.isEnabled());
    
    for (int i=0; i<NUM_WINDOWS; i++) {
        m.addIntArg((int)scene.window[i]);
    }
    if (enableOscOutRDTK) mCameraUnitOscSender.sendMessage(m);
    
    // write scene times
    vector<string> enabledSceneNames;
    for (auto& pair : mScenes) {
        if (pair.second.isEnabled())
            enabledSceneNames.push_back(pair.first);
    }
    
    if (enabledSceneNames.empty() == false) {
        const float sceneElapsedTime{ofGetElapsedTimef()
                                     - mPrevTimeSceneChanged};
        mSceneTimesBuffer.append(ofToString(sceneElapsedTime, 2));
        mSceneTimesBuffer.append("\n");
        for (int i=0; i<enabledSceneNames.size(); i++) {
            mSceneTimesBuffer.append(enabledSceneNames.at(i));
            mSceneTimesBuffer.append(", ");
        }
        
        mPrevTimeSceneChanged = ofGetElapsedTimef();
    }
}

/*
void SceneController::sendChangeScore(const string& name,
                                      bool maintainSceneNames)
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
    
    if (enableOscOutScore) mScoreOscSender.sendMessage(m);
}
 */

void SceneController::doSomething(int rand)
{
    ofxOscMessage m;
    m.setAddress(kOscAddrRamDoSomething);
    m.addIntArg(rand);
    
    if (enableOscOutRDTK) mCameraUnitOscSender.sendMessage(m);
    //if (enableOscOutScore) mScoreOscSender.sendMessage(m);
}

void SceneController::clearTimeLog()
{
    mSceneTimesBuffer.clear();
}

void SceneController::writeTimeLog()
{
    const float sceneElapsedTime{ofGetElapsedTimef() - mPrevTimeSceneChanged};
    mSceneTimesBuffer.append(ofToString(sceneElapsedTime, 2));
    string fileName{"log/scene times-" + ofGetTimestampString() + ".txt"};
    ofBufferToFile(fileName, mSceneTimesBuffer);
}


DP_SCORE_NAMESPACE_END