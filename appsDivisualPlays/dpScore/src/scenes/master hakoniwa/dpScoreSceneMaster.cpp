//
//  dpScoreSceneMaster.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreSceneMaster.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________
MasterNode::MasterNode()
{
}

MasterNode& MasterNode::operator = (const MasterNode& rhs)
{
    return *this = rhs;
}

#pragma mark ___________________________________________________________________
void SceneMaster::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addToggle("Enable", &mEnable);
    mUICanvas->addSpacer();
    mUICanvas->addLabel("(Turn off Enable toggle)");
    mUICanvas->addToggle("Open Pump Forward", &mOpenPumpForward);
    mUICanvas->addToggle("Open Pump Back", &mOpenPumpBack);
    
    mCam.disableMouseInput();
    
    mMasterHakoniwaOscServer.setup(kHostNameMasterHakoniwa,
                                   kPortNumberMasterHakoniwa);
    mCameraUnitOscServer.setup(kHostNameCameraUnit,
                               kPortNumberCameraUnit);
    
    for (int i=0; i<NUM_VALVES; i++) {
        sendPin(i+VALVE_0, false);
    }
    for (int i=0; i<NUM_PUMPS; i++) {
        sendPin(i+PUMP_FORWARD, false);
    }
}

void SceneMaster::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
    
    for (int i=0; i<NUM_VALVES; i++) {
        sendPin(i+VALVE_0, false);
    }
    for (int i=0; i<NUM_PUMPS; i++) {
        sendPin(i+PUMP_FORWARD, false);
    }
}

void SceneMaster::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mFont.loadFont(kFontPath, 26);
}

void SceneMaster::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
    
    mFont = ofTrueTypeFont();
}

void SceneMaster::sendPin(int pin, bool open)
{
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(pin));
    m.addIntArg((int)open);
    mMasterHakoniwaOscServer.sendMessage(m);
}

void SceneMaster::sendScene(const string& name,
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

void SceneMaster::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        
    }
    if (m.getAddress() == kOscAddrCameraUnitMean) {
        if (m.getNumArgs() == 4) {

            mMean.x = m.getArgAsInt32(0);
            mMean.y = m.getArgAsInt32(1);
            mMean.z = m.getArgAsInt32(2);
            mMean.w = m.getArgAsInt32(3);
            
            mMeanAddtion.x += ::fabsf((mMean.x - mMeanPrev.x)) * 0.01;
            mMeanAddtion.y += ::fabsf((mMean.y - mMeanPrev.y)) * 0.01;
            mMeanAddtion.z += ::fabsf((mMean.z - mMeanPrev.z)) * 0.01;
            mMeanAddtion.w += ::fabsf((mMean.w - mMeanPrev.w)) * 0.01;
            
            mMeanPrev = mMean;
        }
    }
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<kNumCameraunitVectors ; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
        }
    }
    
    if (mPrevOfFrame == ofGetFrameNum()) return;
    
    mFrameNum++;
    mPrevOfFrame = ofGetFrameNum();
    
    if (!mEnable) {
        if (ofGetFrameNum()%10==0) {
            sendPin(PUMP_FORWARD, mOpenPumpForward);
            sendPin(PUMP_BACK, mOpenPumpBack);
        }
        return;
    }
    
    int i=0;
    for (auto& it : mValves) {
        auto& v = it.second;
        const int pin{2 + i};
        if (v.open) {
            v.open = false;
            v.opening = true;
            if (mEnable) sendPin(pin, true);
        }
        else if (ofGetElapsedTimef()-v.openedTime >= mOpenDur) {
            sendPin(pin, false);
            v.opening = false;
        }
        else if (mFrameNum % 30 == 0 && v.opening == false) {
            sendPin(pin, false);
            v.opening = false;
        }
        i++;
    }
    
    const int loop{360};
    const int dur{120};
    if (mFrameNum % 10 == 0) {
        if (mFrameNum % loop < dur) {
            for (int i=0; i<NUM_PUMPS; i++) {
                mPumpOn = true;
                if (mEnable) sendPin(i+PUMP_FORWARD, mPumpOn);
            }
        }
        else {
            for (int i=0; i<NUM_PUMPS; i++) {
                mPumpOn = false;
                sendPin(i+PUMP_FORWARD, mPumpOn);
            }
        }
    }
    
    for (int i=0; i<mMeanAddtion.DIM; i++) {
        if (mMeanAddtion[i] >= mMeanLimit) {
            mMeanAddtion[i] = 0.f;
            sendScene(kSceneNames[i], true, true, true, true, true);
            sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
            mPrevScene = i;
        }
    }
}

void SceneMaster::draw()
{
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        
        mCam.begin();
        ofPushMatrix();
        ofNoFill();
        ofTranslate(getLineUped(kW, i, getNumSkeletons()) , -300.f, 0.f);
        ofxMot::drawSkeleton(skl);
        ofPopMatrix();
        mCam.end();
    }
    
    ofSetColor(ofColor::white);
    ofPushMatrix();
    alignedTranslate(0.f, 200.f);
    int i=0;
    for (auto& it : mValves) {
        auto& v = it.second;
        ofPushMatrix();
        ofTranslate(20.f, 20.f * i + 70.f);
        stringstream ss;
        ss << boolalpha << v.opening << ", " << v.addtion << " / " << mLimit;
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        ofPopMatrix();
        i++;
    }
    stringstream ss;
    ss << boolalpha << mPumpOn << endl;
    ss << mMean << " / " << mMeanAddtion << endl;
    ss << kSceneNames[mPrevScene] << endl;
    ofDrawBitmapString(ss.str(), 20.f, 30.f);
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneMaster::setupSkeleton(SkeletonPtr skl)
{
    mValves[skl->getName()] = Valve();
}

void SceneMaster::updateSkeleton(SkeletonPtr skl)
{
    auto&v = mValves[skl->getName()];
    const auto& origin = skl->getJoint(0).getGlobalPosition();
    v.addtion = 0.f;
    for (auto& n : skl->getJoints()) {
        n.update();
        v.addtion += n.totalDistance;
    }
    if (v.addtion > mLimit) {
        v.openedTime = ofGetElapsedTimef();
        v.open = true;
        for (auto& n : skl->getJoints()) n.totalDistance = 0.f;
    }
}

void SceneMaster::exitSkeleton(SkeletonPtr skl)
{
    mValves.erase(skl->getName());
}

DP_SCORE_NAMESPACE_END