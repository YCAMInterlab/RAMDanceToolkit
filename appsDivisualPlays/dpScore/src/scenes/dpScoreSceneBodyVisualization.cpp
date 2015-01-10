//
//  dpScoreSceneBodyVisualization.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#include "dpScoreSceneBodyVisualization.h"

OFX_MOTIONER_NAMESPACE_BEGIN
string getJointNameLower(int index)
{
    if (index==JOINT_HIPS)				return "hips";
    if (index==JOINT_ABDOMEN)			return "abdomen";
    if (index==JOINT_CHEST)				return "chest";
    if (index==JOINT_NECK)				return "neck";
    if (index==JOINT_HEAD)				return "head";
    if (index==JOINT_LEFT_HIP)			return "left hip";
    if (index==JOINT_LEFT_KNEE)			return "left knee";
    if (index==JOINT_LEFT_ANKLE)		return "left ankle";
    if (index==JOINT_LEFT_TOE)			return "left toe";
    if (index==JOINT_RIGHT_HIP)			return "right hip";
    if (index==JOINT_RIGHT_KNEE)		return "right knee";
    if (index==JOINT_RIGHT_ANKLE)		return "right ankle";
    if (index==JOINT_RIGHT_TOE)			return "right toe";
    if (index==JOINT_LEFT_COLLAR)		return "left collar";
    if (index==JOINT_LEFT_SHOULDER)		return "left shoulder";
    if (index==JOINT_LEFT_ELBOW)		return "left elbow";
    if (index==JOINT_LEFT_WRIST)		return "left wrist";
    if (index==JOINT_LEFT_HAND) 		return "left hand";
    if (index==JOINT_RIGHT_COLLAR) 		return "right collar";
    if (index==JOINT_RIGHT_SHOULDER) 	return "right shoulder";
    if (index==JOINT_RIGHT_ELBOW) 		return "right elbow";
    if (index==JOINT_RIGHT_WRIST) 		return "right wrist";
    if (index==JOINT_RIGHT_HAND)		return "right hand";
    
    return "unknown joint index";
}
OFX_MOTIONER_NAMESPACE_END

DP_SCORE_NAMESPACE_BEGIN

SceneBodyVisualization::Node::Node()
{
    points.clear();
    vertices.clear();
    vertices.assign(kMaxPoints, ofVec3f::zero());
    vbo.setVertexData(&vertices.at(0), vertices.size(), GL_DYNAMIC_DRAW);
}

void SceneBodyVisualization::Node::update()
{
    ofVec3f p = dir * scale;
    p = p * getOrientationQuat();
    
    points.push_back(p);
    
    while (points.size() > kMaxPoints) {
        points.pop_front();
    }
    
    for (int i=0; i<points.size(); i++) {
        vertices.at(i) = points.at(i);
    }
    
    vbo.updateVertexData(&vertices.at(0), vertices.size());
}

void SceneBodyVisualization::Node::customDraw()
{
    glBegin(GL_LINES);
    glColor4f(1.f, 1.f, 1.f, 0.6f);
    glVertex3fv(ofVec3f::zero().getPtr());
    glColor4f(1.f, 1.f, 1.f, 0.1f);
    glVertex3fv((dir * scale * 1.2f).getPtr());
    glEnd();
}

void SceneBodyVisualization::Node::drawPoints()
{
    glPointSize(3.f);
    ofSetColor(255, 40);
    vbo.draw(GL_POINTS, 0, vertices.size());
}

void SceneBodyVisualization::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sphere Scale", 200.f, 600.f, &mScale);
    mUICanvas->addSlider("Rotation Speed X", 0.f, 10.f, &mRotSpdX);
    mUICanvas->addSlider("Rotation Speed Y", 0.f, 10.f, &mRotSpdY);
    mUICanvas->addToggle("Magnify", &mMagnify);
    
    mNodes.assign(ofxMot::NUM_JOINTS, Node::Ptr());
    for (auto& p : mNodes) {
        p = Node::Ptr(new Node());
    }
}

void SceneBodyVisualization::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyVisualization::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyVisualization::onUpdateSkeleton);
}

void SceneBodyVisualization::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyVisualization::onUpdateSkeleton);
}

void SceneBodyVisualization::update(ofxEventMessage& m)
{
    ofSetWindowTitle(getName() + ": " + ofToString(ofGetFrameRate(), 2));
    
    if (mMagnify) {
        mScale += ofGetLastFrameTime() * 2.f;
        if (mScale >= 600.f) mScale = 100.f;
    }
    for (auto& p : mNodes) p->scale = mScale;
    
    if (ofGetFrameNum()%60==0) {
        mJointId++;
        mJointId %= ofxMot::NUM_JOINTS;
    }
}

void SceneBodyVisualization::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(getName(), 12.f, 16.f);
    
    mCam.begin();
    ofPushMatrix();
    ofRotateX(ofGetElapsedTimef() * mRotSpdX);
    ofRotateY(ofGetElapsedTimef() * mRotSpdY);
    
    ofNoFill();
    ofSetLineWidth(1.5f);
    //ofxMotioner::debugDraw();
    
    int i=0;
    for (auto& p : mNodes) {
        ofEnableAlphaBlending();
        p->drawPoints();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        p->draw();
        //if (i==mJointId && ofGetFrameNum()%60 > 0 && ofGetFrameNum()%60 < 10) {
        if (ofGetFrameNum()%120 > 0 && ofGetFrameNum()%120 < 30) {
            ofSetColor(255, 128);
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            ofPushMatrix();
            ofTranslate(p->dir * p->scale * 1.f);
            ofDrawBitmapString(ofxMot::getJointNameLower(mJointId), ofPoint::zero());
            ofPopMatrix();
        }
        
        i++;
    }
    ofPopMatrix();
    mCam.end();
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyVisualization::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSleletonName=="") mSleletonName = skl->getName();
    
    if (mSleletonName == skl->getName()) {
        auto joints = skl->getJoints();
        
        for (int i=0; i<mNodes.size(); i++) {
            auto& node = mNodes.at(i);
            node->dir = joints.at(i).getPosition();
            node->dir.normalize();
            node->setOrientation(joints.at(i).getOrientationQuat());
            node->update();
        }
    }
}

DP_SCORE_NAMESPACE_END