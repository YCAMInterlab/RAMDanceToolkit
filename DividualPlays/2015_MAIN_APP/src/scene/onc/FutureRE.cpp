//
//  FutureRE.cpp
//  BasicTraining_Aug2014
//
//  Created by Onishi Yoshito on 8/11/14.
//
//

#include "FutureRE.h"

FutureRE::FutureRE()
{
    
}

FutureRE::~FutureRE()
{
    
}

void FutureRE::setupControlPanel()
{
    lowpass = 0.0;
    ramGetGUI().addSlider("low pass filter", 0.0, 1.0, &lowpass);
}

void FutureRE::update()
{
    
}

void FutureRE::draw()
{
    
}

void FutureRE::onPanelChanged(ofxUIEventArgs& e)
{
    
}

void FutureRE::drawActor(const ramActor &actor)
{
    Diff& diff = diffs[actor.getName()];
    
    ramActor result = actor;
    
    if (actor.getNode(ramActor::JOINT_HIPS).getGlobalPosition() != diff.previousActor.getNode(ramActor::JOINT_HIPS).getGlobalPosition()) {
        const float fp = 5.0f;
        const float fa = 5.0f;
        ofVec3f p0 = actor.getNode(ramActor::JOINT_HIPS).getGlobalPosition();
        ofVec3f p1 = diff.previousActor.getNode(ramActor::JOINT_HIPS).getGlobalPosition();
        
        ofVec3f p = p0 * fp + p1 * (1.0f - fp);
        if (lowpass > 0.01f) {
            p = p * (1.0 - lowpass) + diff.previousResult.getNode(ramActor::JOINT_HIPS).getGlobalPosition() * lowpass;
        }
        result.getNode(ramActor::JOINT_HIPS).setGlobalPosition(p);
        
        for (int i=0; i<actor.getNumNode(); i++) {
            ofQuaternion q0 = actor.getNode(i).getGlobalOrientation();
            ofQuaternion q1 = diff.previousActor.getNode(i).getGlobalOrientation();
            ofQuaternion q;
            q.slerp(fa, q1, q0);
            
            if (lowpass > 0.01f) {
                ofQuaternion _q;
                _q.slerp(lowpass, q, diff.previousResult.getNode(i).getGlobalOrientation());
                q = _q;
            }
            
            result.getNode(i).setGlobalOrientation(q);
        }
        diff.previousActor = actor;
        
        diff.previousResult = result;
    }
    else {
        result = diff.previousResult;
    }
    
    ofSetColor(ofColor::blue);
    ramDrawBasicActor(result);
}









