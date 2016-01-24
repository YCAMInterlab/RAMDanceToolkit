//
//  dpScoreSceneFlowChart.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

/*
   三人称
   日本語
   ハメコミ
   箱庭フォーカス
 */

#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeMasterHakoniwa.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreNodeDisplay.h"
#include "dpScoreNodeMotioner.h"
#include "dpScoreNodeDancer.h"
#include "dpScoreNodeSpeaker.h"
#include "dpScoreNodeLight.h"
#include "dpScoreNodeAudience.h"
#include "dpScoreNodeCameraUnit.h"
#include "dpScoreNodeComputer.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"

#pragma mark ___________________________________________________________________

static const float kCamMoveSpan = 2.f;
static const float kCamIdleSpan = 2.f;
static const float kLineSpan = 1.f;
static const float kModeChangeSpan = (kCamMoveSpan + kCamIdleSpan) * 10.f;
static const float kMainCamSpeed = 0.025f;
static const float kYOffset = -200.f;

static string kNodeNames[] = {
	"Motion Captures",
	"Dancers",
	"Master Hakoniwa",
	"",
	"Hakoniwa x 6",
	"Hakoniwa Analisis",
	"Displays",
	"Speakers",
	"Lights",
	"RAM Dance Toolkit",
	"Audiences",
};

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________

void NodeSkeleton::customDraw()
{
	ofDrawBox(size);
}

#pragma mark ___________________________________________________________________

void SceneFlowChart::initialize()
{
	setFixPosition(false); // for skeleton

	mFont.loadFont(kFontPath, 80.f);
	mFontSmall.loadFont(kFontPath, 40.f);

	setupNodes();
	setupCameras();
	setupOrders();

	// main camera settings
	mCamMainParent.setGlobalPosition(ofVec3f::zero());
	mCamMain.setParent(mCamMainParent);
	mCamMain.setGlobalPosition(0.f, 0.f, 1200.f);

	mCamEasy.setDistance(900.f);

	changeCamMode(CAM_MOVE);
}

void SceneFlowChart::setupNodes()
{
	// place nodes
	mNodes.clear();
	auto motioner = ofPtr<NodeMotioner>(new NodeMotioner());
	auto dancer = ofPtr<NodeDancer>(new NodeDancer());
	auto masterHakoniwa = ofPtr<NodeMasterHakoniwa>(new NodeMasterHakoniwa());
	auto stage = ofPtr<NodeStage>(new NodeStage());
	auto hakoniwa = ofPtr<NodeHakoniwa>(new NodeHakoniwa());
	auto cameraUnit = ofPtr<NodeCameraUnit>(new NodeCameraUnit());
	auto display = ofPtr<NodeDisplay>(new NodeDisplay());
	auto speaker = ofPtr<NodeSpeaker>(new NodeSpeaker());
	auto light = ofPtr<NodeLight>(new NodeLight());
	auto computer = ofPtr<NodeComputer>(new NodeComputer());
	auto audience = ofPtr<NodeAudience>(new NodeAudience());

	mNodes.push_back(motioner);
	mNodes.push_back(dancer);
	mNodes.push_back(masterHakoniwa);
	mNodes.push_back(stage);
	mNodes.push_back(hakoniwa);
	mNodes.push_back(cameraUnit);
	mNodes.push_back(display);
	mNodes.push_back(speaker);
	mNodes.push_back(light);
	mNodes.push_back(computer);
	mNodes.push_back(audience);
}

void SceneFlowChart::setupCameras()
{
	// moving camera settings
	mCams.assign(mNodes.size(), ofCamera());

	mCams.at(NODE_MASTER_HAKONIWA).setFov(45.f);
	mCams.at(NODE_MASTER_HAKONIWA).setPosition(NodeStage::kWidth * 0.5f + Desk::getDimension() * 0.5f + 50.f, -75.f, 550.f);

	mCams.at(NODE_STAGE).setFov(45.f);
	mCams.at(NODE_STAGE).setPosition(-40.f, 400.f, 1550.f);
	mCams.at(NODE_STAGE).setOrientation(ofVec3f(-10.f, 0.f, 0.f));

	mCams.at(NODE_HAKONIWA).setFov(60.f);
	mCams.at(NODE_HAKONIWA).setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() * 0.5f - 50.f, 50.f, 550.f);
	mCams.at(NODE_HAKONIWA).setOrientation(ofVec3f(-25.f, 0.f, 0.f));

	mCams.at(NODE_CAMERA_UNIT).setFov(35.f);
	mCams.at(NODE_CAMERA_UNIT).setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 300.f, 0.f, -50.f);
	mCams.at(NODE_CAMERA_UNIT).setOrientation(ofVec3f(-30.f, 180.f + 30.f, 0.f));

	mCams.at(NODE_DISPLAY).setFov(35.f);
	mCams.at(NODE_DISPLAY).setPosition(-NodeStage::kWidth * 0.5f + 80.f, 70.f, 200.f);
	mCams.at(NODE_DISPLAY).setOrientation(ofVec3f(0.f, 150.f, 0.f));

	mCams.at(NODE_MOTIONER).setFov(70.f);
	mCams.at(NODE_MOTIONER).setPosition(0.f, 200.f, 400.f);
	mCams.at(NODE_MOTIONER).setOrientation(ofVec3f(-30.f, 0.f, 0.f));

	mCams.at(NODE_DANCER).setFov(50.f);
	mCams.at(NODE_DANCER).setPosition(0.f, 200.f, 800.f);
	mCams.at(NODE_DANCER).setOrientation(ofVec3f(-20.f, 0.f, 0.f));

	mCams.at(NODE_SPEAKER).setFov(55.f);
	mCams.at(NODE_SPEAKER).setPosition(NodeStage::kWidth * 0.5f + Desk::getDimension() * 0.5f + 50.f - 40.f, -100.f, 600.f);
	mCams.at(NODE_SPEAKER).setOrientation(ofVec3f(0.f, -45.f, 0.f));

	mCams.at(NODE_LIGHT).setFov(50.f);
	mCams.at(NODE_LIGHT).setPosition(0.f, 400.f, 500.f);
	mCams.at(NODE_LIGHT).setOrientation(ofVec3f(15.f, 0.f, 0.f));

	mCams.at(NODE_COMPUTER).setFov(60.f);
	mCams.at(NODE_COMPUTER).setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 400.f, 0.f, -190.f);
	mCams.at(NODE_COMPUTER).setOrientation(ofVec3f(-20.f, -90.f, 0.f));

	mCams.at(NODE_AUDIENCE).setFov(80.f);
	mCams.at(NODE_AUDIENCE).setPosition(0.f, 100.f, 650.f);
	mCams.at(NODE_AUDIENCE).setOrientation(ofVec3f(-30.f, 180.f, 0.f));
}

void SceneFlowChart::setupOrders()
{
	mOrders.clear();
	{
		vector<int> order;
		order.push_back(NODE_STAGE);
		order.push_back(NODE_STAGE);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_MOTIONER);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_DISPLAY);
		order.push_back(NODE_DANCER);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_MOTIONER);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_HAKONIWA);
		order.push_back(NODE_CAMERA_UNIT);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_DISPLAY);
		order.push_back(NODE_DANCER);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_MOTIONER);
		order.push_back(NODE_MASTER_HAKONIWA);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_HAKONIWA);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_AUDIENCE);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_DISPLAY);
		order.push_back(NODE_DANCER);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_MOTIONER);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_LIGHT);
		order.push_back(NODE_DANCER);
		mOrders.push_back(order);
	}
	{
		vector<int> order;
		order.push_back(NODE_MOTIONER);
		order.push_back(NODE_COMPUTER);
		order.push_back(NODE_SPEAKER);
		order.push_back(NODE_DANCER);
		mOrders.push_back(order);
	}
}

void SceneFlowChart::shutDown()
{
	mNodes.clear();
	mCams.clear();
	mOrders.clear();
}

void SceneFlowChart::enter()
{
}

void SceneFlowChart::exit()
{
}

void SceneFlowChart::update(ofxEventMessage& m)
{
	if (ofGetFrameNum() == mLastFrameNum) return;
	mLastFrameNum = ofGetFrameNum();

	mElapsedTime += ofGetLastFrameTime();

	if (mElapsedTime >= kModeChangeSpan) {
		if (mCamMode == CAM_MOVE) changeCamMode(CAM_MAIN);
		else if (mCamMode == CAM_MAIN) changeCamMode(CAM_MOVE);
		mElapsedTime = 0.f;
	}

	if (getNumSkeletons() >= 2) {
		kNodeNames[NODE_DANCER] = "Dancers";
		kNodeNames[NODE_MOTIONER] = "Motion Captures";
	}
	else {
		kNodeNames[NODE_DANCER] = "Dancer";
		kNodeNames[NODE_MOTIONER] = "Motion Capture";
	}

	// animation
	mElapsedTimeMove += ofGetLastFrameTime();
	if (mElapsedTimeMove >= kCamMoveSpan + kCamIdleSpan) {
		mElapsedTimeMove = 0.f;
		++mNodeIdx %= mOrders.at(mOrderIdx).size();
	}
	if (getNumSkeletons()) {
		mNodes.at(NODE_MOTIONER)->clearAimingOffsets();
		for (auto i : rep(getNumSkeletons())) {
			mNodes.at(NODE_MOTIONER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_HIPS).getGlobalPosition());
		}
		mNodes.at(NODE_DANCER)->clearAimingOffsets();
		for (auto i : rep(getNumSkeletons())) {
			mNodes.at(NODE_DANCER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_HEAD).getGlobalPosition());
			//mNodes.at(NODE_DANCER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_LEFT_HAND).getGlobalPosition());
			//mNodes.at(NODE_DANCER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_RIGHT_HAND).getGlobalPosition());
			//mNodes.at(NODE_DANCER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_LEFT_ANKLE).getGlobalPosition());
			//mNodes.at(NODE_DANCER)->addAimingOffset(getSkeleton(i)->getJoint(ofxMot::JOINT_RIGHT_ANKLE).getGlobalPosition());
		}
	}

	float t {ofClamp(mElapsedTimeMove, 0.f, kCamMoveSpan) / kCamMoveSpan};
	t = easeInOutCubic(t);

	// move camera
	if (mCamMode == CAM_MOVE) {
		auto currCam = mCams.at(getCurrentNodeID());
		auto nextCam = mCams.at(getNextNodeID());

		for (auto i : rep(mNodes.size())) {
			auto n = mNodes.at(i);
			if (i == getNextNodeID()) {
				n->t = t;
			}
			else if (i == getCurrentNodeID()) {
				n->t = 1.f - t;
			}
			else {
				n->t = 0.f;
			}
		}
		auto q0 = currCam.getGlobalOrientation();
		auto q1 = nextCam.getGlobalOrientation();
		ofQuaternion q;
		q.slerp(t, q0, q1);
		mCurrentCam.setFov(currCam.getFov() * (1.f - t) + nextCam.getFov() * t);
		mCurrentCam.setGlobalPosition(currCam.getGlobalPosition().interpolated(nextCam.getGlobalPosition(), t));
		mCurrentCam.setGlobalOrientation(q);
	}
	else if (mCamMode == CAM_MAIN) {
		// rotate main cam
		mElapsedTimeMainCam += ofGetLastFrameTime();
		const float r {::cosf(mElapsedTimeMainCam * kMainCamSpeed) * 110.f};
		mCamMainParent.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

		for (auto i : rep(mNodes.size())) {
			if (i == NODE_LIGHT) {
				mNodes.at(i)->t = 0.f;
			}
			else {
				mNodes.at(i)->t = 1.f;
			}
		}
	}
	else {
		for (auto i : rep(mNodes.size())) {
			mNodes.at(i)->t = 1.f;
		}
	}
}

void SceneFlowChart::drawScene()
{
	ofCamera* camera;
	switch (mCamMode) {
	case CAM_MOVE: camera = &mCurrentCam; break;
	case CAM_MAIN: camera = &mCamMain; break;
	default:
	case CAM_EASY: camera = &mCamEasy; break;
	}

	camera->begin();
	{
		drawStage();
		drawCameras();
	}
	camera->end();

	drawHUD();
}

void SceneFlowChart::drawStage()
{
	ScopedTranslate t(0.f, kYOffset, 0.f);
	// draw nodes
	for (auto i : rep(mNodes.size())) {
		auto n = mNodes.at(i);
		// only render audiences when they have been focused
		if (i == NODE_AUDIENCE) {
			if (getNextNodeID() == NODE_AUDIENCE || (getCurrentNodeID() == NODE_AUDIENCE && n->t < 1.f)) {
				n->draw();
			}
		}
		else {
			n->draw();
		}
	}
	// draw skeletons
	for (auto i : rep(getNumSkeletons())) {
		auto skl = getSkeleton(i);
		for (auto& n : skl->getJoints()) {
			n.draw();
			if (!n.getParent()) continue;
			ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
		}
	}
	// draw lines
	{
		ScopedStyle s;
		setStyle();

		for (auto i : rep(mNodes.at(getCurrentNodeID())->getNumAimingPositions())) {
			for (auto j : rep(mNodes.at(getNextNodeID())->getNumAimingPositions())) {
				auto p0 = mNodes.at(getCurrentNodeID())->getAimingPosition(i);
				auto p1 = mNodes.at(getNextNodeID())->getAimingPosition(j);
				auto cp0 = p0.interpolated(p1, 0.25f);
				auto cp1 = p0.interpolated(p1, 0.75f);
				float height {p0.distance(p1) * 0.5f};
				if (height < 100.f) height = 100.f;
				cp0.y += height;
				cp1.y += height;

				const int res {100};
				ofPolyline line;
				line.addVertex(p0);
				line.bezierTo(cp0, cp1, p1, res);

				auto v = line.getVertices();
				float f {::fmodf(mElapsedTimeMove, kLineSpan) / kLineSpan};
				f = ofClamp(f, 0.f, 1.f);
				f = easeInOutQuad(f);
				const float len {0.5f};
				const float fb {ofMap(f, len, 1.f, 0.f, 1.f, true)};
				const float fe {ofMap(f, 0.f, 1.f - len, 0.f, 1.f, true)};

				ofSetColor(color::kMain);
				for (int i = (v.size() - 1) * fb; i < (v.size() - 1) * fe; i++) {
					ofLine(v.at(i), v.at(i + 1));
				}
				// line end circle
				if (f >= len) {
					ScopedStyle s;
					ofSetCircleResolution(64);
					ofDisableDepthTest();
					ofEnableAlphaBlending();
					ScopedTranslate t(v.back());
					billboard();
					ofFill();
					ofSetColor(color::kMain);
					ofCircle(ofVec3f::zero(), 6.f);
					const float r {ofMap(f, len, 1.f, 0.f, 1.f)};
					ofSetColor(color::kMain, 200 * (1.f - easeInExpo(r)));
					ofCircle(ofVec3f::zero(), r * 100.f);
					ofEnableDepthTest();
				}
			}
		}
	}
}

void SceneFlowChart::drawCameras()
{
	// debug draw cameras
	if (mCamMode == CAM_EASY) {
		ScopedStyle s;
		ofNoFill();
		for (auto c : mCams) {
			ofSetColor(ofColor::red);
			ScopedMatrix m;
			ofMultMatrix(c.getGlobalTransformMatrix());
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		{
			ofSetColor(ofColor::blue);
			ScopedMatrix m;
			ofMultMatrix(mCurrentCam.getGlobalTransformMatrix());
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		{
			ScopedStyle s;
			ScopedTranslate t(0.f, kYOffset, 0.f);
			ofSetColor(ofColor::yellow);
			ofSetSphereResolution(8);
			for (auto n : mNodes) {
				for (auto i : rep(n->getNumAimingPositions())) {
					ofDrawSphere(n->getAimingPosition(i), 10.f);
				}
			}
		}
	}
}

void SceneFlowChart::drawHUD()
{
	// display node name
	if (mCamMode == CAM_MOVE && mNodes.at(getNextNodeID())->t >= 1.f) {
		ScopedStyle s;
		ofFill();
		ofSetColor(ofColor::white);
		mFont.drawString(kNodeNames[getNextNodeID()], 30.f, 120.f);
	}
	else if (mCamMode == CAM_MAIN) {
		ScopedTranslate t(20.f, 80.f);
		ScopedStyle s;
		ofFill();
		ofSetColor(color::kMain);
		mFontSmall.drawString("from\nto", 0.f, 0.f);
		ofSetColor(ofColor::white);
		const string str0 {kNodeNames[getCurrentNodeID()]};
		const string str1 {"\n" + kNodeNames[getNextNodeID()]};
		mFontSmall.drawString(str0, mFontSmall.stringWidth("fromx"), 0.f);
		mFontSmall.drawString(str1, mFontSmall.stringWidth("fromx"), 0.f);
	}
}

void SceneFlowChart::draw()
{
	// draw scen for rear screen
	auto stage = getNode<NodeStage>(NODE_STAGE);
	stage->forFbo = true;
	stage->fbo.begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	stage->fbo.end();

	// main draw call
	stage->forFbo = false;
	drawScene();
}

void SceneFlowChart::keyPressed(int key)
{
	switch (key) {
	case 'q': changeCamMode(CAM_MOVE); break;
	case 'w': changeCamMode(CAM_MAIN); break;
	case 'e': changeCamMode(CAM_EASY); break;
	case '+':
		++mOrderIdx %= mOrders.size();
		break;
	}
}

void SceneFlowChart::changeCamMode(CamMode m)
{
	mCamMode = m;

	auto camUnit = getNode<NodeCameraUnit>(NODE_CAMERA_UNIT);
	auto computer = getNode<NodeComputer>(NODE_COMPUTER);

	switch (m) {
	case CAM_MOVE:
		mNodeIdx = 0;
		mElapsedTimeMove = 0;
		break;
	case CAM_MAIN:
		mElapsedTimeMainCam = HALF_PI * (1.f / kMainCamSpeed);
		break;
	case CAM_EASY:
		break;
	default:
		break;
	}
}

int SceneFlowChart::getCurrentNodeID() const
{
	return mOrders.at(mOrderIdx).at(mNodeIdx);
}

int SceneFlowChart::getNextNodeID() const
{
	return mOrders.at(mOrderIdx).at((mNodeIdx + 1) % mOrders.at(mOrderIdx).size());
}

DP_SCORE_NAMESPACE_END