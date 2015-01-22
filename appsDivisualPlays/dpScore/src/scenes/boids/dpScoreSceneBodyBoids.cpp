#include "dpScoreSceneBodyBoids.h"
#include "dpEasing.h"

DP_SCORE_NAMESPACE_BEGIN

static const int    NUM_INITIAL_PARTICLES   = 200;
static const int    NUM_PARTICLES_TO_ADD    = 10;

static const float  DELAY = 0.f;
static const float  INTERACT_DELAY = 3.f;

#pragma mark ___________________________________________________________________
void SceneBodyBoids::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
    mCam.setDistance(300.f);
}

void SceneBodyBoids::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyBoids::enter()
{
    dpDebugFunc();
    
    // CREATE PARTICLE CONTROLLER
    mParticleController.addParticles(NUM_INITIAL_PARTICLES,
                                     ofVec3f(0.0f, 100.0f, 0.0f),
                                     10.0f,
                                     50.0f);
    mParticleController.addPredators(ofxMot::NUM_JOINTS);
    
    mEnterTime  = ofGetElapsedTimef();

    mCam.enableMouseInput();
    
    mEnterFrame = ofGetFrameNum();
}

void SceneBodyBoids::exit()
{
    dpDebugFunc();
    
    mParticleController.clear();
    
    mCam.enableMouseInput();
}

#pragma mark ___________________________________________________________________

void SceneBodyBoids::update(ofxEventMessage& m)
{
    int frame{ofGetFrameNum() - mEnterFrame + 1};
    if (frame % 60 * 90 == 0) {
        exit();
        enter();
    }
    
    const float t = ofGetElapsedTimef();
    
    if (t-mEnterTime<DELAY) return;
    
    if (m.getAddress() == kOscAddrMotioner && mActor) {
        
        mParticleController.setSpeed(easeInSine(mSpd));
        
        mSpd += 0.01f;
        mSpd = MIN(mSpd, 1.f);
        
        if(mLowerThresh > mHigherThresh) {
            mHigherThresh = mLowerThresh;
        }
        
        //mParticleController.applyForceToPredators(mZoneRadius, 0.4f, 0.7f);
        for (int i=0; i<ofxMot::NUM_JOINTS; i++) {
            
            if (mActor && mPrevActor) {
                auto v0 = mPrevActor->getJoint(i).getGlobalPosition();
                auto v1 = mActor->getJoint(i).getGlobalPosition();
                auto acc = (v0 - v1) * 5.f;
                auto pos = mActor->getJoint(i).getGlobalPosition();
                
                //pos.rotate(mEuler.y, ofVec3f(0.0f, 1.0f, 0.0f));
                //acc.rotate(mEuler.y, ofVec3f(0.0f, 1.0f, 0.0f));
                
                mParticleController.mPredators.at(i).update(pos, acc);
            }
        }
        
        mParticleController.applyForceToParticles(mZoneRadius,
                                                  mLowerThresh,
                                                  mHigherThresh,
                                                  mAttractStrength,
                                                  mRepelStrength,
                                                  mOrientStrength);
        if(mCentralGravity)
            mParticleController.pullToCenter(ofVec3f(0.0, 100.0, 0.0));
        
        mParticleController.update(mFlatten);
        
        //    if (ofGetElapsedTimef()-mEnterTime<INTERACT_DELAY) return;
        //
        //    if (getA0()) {
        //        randomizeCamera();
        //    }
        
        //if (getA1()) {
        //    mPointSize = 5.f;
        //}
        //
        //mPointSize -= 0.05f;
        //mPointSize = MAX(mPointSize, 2.f);
    }

    mEuler.y = t * 5.f;
}

#pragma mark ___________________________________________________________________
void SceneBodyBoids::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofNoFill();
    ofSetSphereResolution(2);
    mCam.begin();
    ofPushMatrix();
    ofTranslate(0.f, -120.f);
    ofRotateY(ofGetElapsedTimef()*5.f);
    
    if (mActor) {
        //ofxMot::drawSkeleton(mActor);
        ofSetColor(color::kDarkPinkHeavy, 128);
        auto& joints = mActor->getJoints();
        for (auto& j : joints) {
            j.transformGL();
            ofDrawSphere(ofVec3f::zero(), 2.f);
            j.restoreTransformGL();
        }
    }
    
    glPointSize(mPointSize);
    ofEnableAlphaBlending();
    
    glPointSize(1.f);
    mParticleController.draw();
    auto& particles = mParticleController.mParticles;
    
    glPointSize(3.f);
    ofSetColor(ofColor::white, 128);
    glBegin(GL_POINTS);
    for (int i=0; i<particles.size(); i++) {
        auto& vec = particles.at(i).mTailCoords;
        if (vec.empty()==false) {
            glVertex3fv(vec.at(0).getPtr());
        }
    }
    glEnd();
    
    ofSetColor(color::kMain, 255);
    glBegin(GL_POINTS);
    for (int i=0; i<particles.size(); i++) {
        glVertex3fv(particles.at(i).mPos.getPtr());
    }
    glEnd();

    ofPopMatrix();
    mCam.end();
    
    drawHeader();
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyBoids::randomizeCamera()
{
    const size_t n = mParticleController.mParticles.size();
    const ofVec3f eye =
    mParticleController.mParticles.at((size_t)ofRandom((float)n)).mPos;
    const ofVec3f lookAt =
    mParticleController.mParticles.at((size_t)ofRandom((float)n)).mPos;
    
    //    mCam.setPosition(eye);
    //    mCam.setTarget(lookAt);
    
}

#pragma mark ___________________________________________________________________
void SceneBodyBoids::updateSkeleton(SkeletonPtr skl)
{
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        if (mActor) mPrevActor = mActor;
        mActor = ofxMot::Skeleton::copy(skl);
    }
}

DP_SCORE_NAMESPACE_END