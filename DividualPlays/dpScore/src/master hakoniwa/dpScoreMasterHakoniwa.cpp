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
    tabbar->addToggle("Enable OSC to RAM DTK",
                      &mSceneController.enableOscOutRDTK);
    tabbar->addToggle("Enable OSC to Master Hakoniwa",
                      &mColorOfWater.enableOscOut);
    tabbar->addToggle("Enable OSC to Score",
                      &mSceneController.enableOscOutScore);
    tabbar->addToggle("Change Scene with CameraUnit",
                      &mEnableCameraUnit);
    tabbar->addToggle("Open Valve with MOTIONER",
                      &mColorOfWater.enableOpenValve);
    
    tabbar->addSpacer(w, 1.f);
    
    mSceneController.setupUI(tabbar);
    
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
    
    mSceneController.initialize();
    
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
        mSceneController.stopAll();
        
        mEnableCameraUnit = false;
        mEmergencyStop = false;
    }
    
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
    
    mSceneController.draw();
    
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
    else if (widgetName == "Presets") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        const auto& toggleName = radio->getActiveName();
        if (radio->getActive()->getValue()) {
            if (toggleName == "Maestro") {
                mColorOfWater.enableOscOut = true;
                mColorOfWater.enableOpenValve = true;
                mSceneController.enableOscOutRDTK = true;
                mSceneController.enableOscOutScore = true;
                mSceneController.enableShowHakoniwaTitle = true;
                                mEnableCameraUnit = true;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mAnalyzeMean.mMeanLimit = 10.f;
                mAnalyzeMean.mMinSetSceneTime = 90.f;
            }
            else if (toggleName == "Intro") {
                mColorOfWater.enableOscOut = false;
                mColorOfWater.enableOpenValve = false;
                mSceneController.enableOscOutRDTK = false;
                mSceneController.enableOscOutScore = true;
                mSceneController.enableShowHakoniwaTitle = false;
                mEnableCameraUnit = false;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mSceneController.sendChangeScore("dp::score::SceneVec2SimpleGraph");
                
                mSceneController.clearTimeLog();
            }
            else if (toggleName == "Outro") {
                mColorOfWater.enableOscOut = true;
                mColorOfWater.enableOpenValve = true;
                
                mSceneController.enableOscOutRDTK = false;
                mSceneController.enableOscOutScore = true;
                mSceneController.enableShowHakoniwaTitle = false;
                mEnableCameraUnit = true;
                mAnalyzeMean.mMaster = true;
                mAnalyzePixelate.mMaster = false;
                mAnalyzeMean.mMeanLimit = 3.f;
                mAnalyzeMean.mMinSetSceneTime = 30.f;
                
                
                mSceneController.writeTimeLog();
            }
        }
    }
}

DP_SCORE_NAMESPACE_END