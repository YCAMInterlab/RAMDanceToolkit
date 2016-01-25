//
//  dpScoreSceneFlowChart.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

/*
   箱庭フォーカス
   書き込み
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
	mFontJP.loadFont(kFontPathJP, 24.f, true, true);

	setupNodes();
	setupOrders();

	// main camera settings
	mCamMainParent.setGlobalPosition(ofVec3f::zero());
	mCamMain.setParent(mCamMainParent);
	mCamMain.setGlobalPosition(0.f, 0.f, 1200.f);

	mCamTPS.setParent(mCamTPSParent);
	mCamTPS.setGlobalPosition(0.f, 0.f, 200.f);

	mCamEasy.setDistance(900.f);

	mCamToolKit.setGlobalPosition(0.f, 200.f, 500.f);
	mCamToolKit.setFarClip(1000.f);

	changeCamMode(CAM_MOVE);
}

void SceneFlowChart::setupNodes()
{
	// place nodes
	mNodes.clear();
	mNodes.push_back(makeShared<NodeMotioner>());
	mNodes.push_back(makeShared<NodeDancer>());
	mNodes.push_back(makeShared<NodeMasterHakoniwa>());
	mNodes.push_back(makeShared<NodeStage>());
	mNodes.push_back(makeShared<NodeHakoniwa>());
	mNodes.push_back(makeShared<NodeCameraUnit>());
	mNodes.push_back(makeShared<NodeDisplay>());
	mNodes.push_back(makeShared<NodeSpeaker>());
	mNodes.push_back(makeShared<NodeLight>());
	mNodes.push_back(makeShared<NodeComputer>());
	mNodes.push_back(makeShared<NodeAudience>());
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

	// change camera mode
	if (mElapsedTime >= kModeChangeSpan) {
		if (mCamMode == CAM_MOVE) changeCamMode(CAM_MAIN);
		else if (mCamMode == CAM_MAIN) changeCamMode(CAM_TPS);
		else if (mCamMode == CAM_TPS) changeCamMode(CAM_MOVE);
		mElapsedTime = 0.f;
	}

	// update text
	if (getNumSkeletons() >= 2) {
		getNode<NodeDancer>()->title = "Dancers";
		getNode<NodeMotioner>()->title = "Motion Captures";
	}
	else {
		getNode<NodeDancer>()->title = "Dancer";
		getNode<NodeMotioner>()->title = "Motion Capture";
	}

	// camera moving and line animation
	mElapsedTimeMove += ofGetLastFrameTime();
	if (mElapsedTimeMove >= kCamMoveSpan + kCamIdleSpan) {
		mElapsedTimeMove = 0.f;
		++mNodeIdx %= mOrders.at(mOrderIdx).size();
	}

	// update for skeleton
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

		// TPS camera
		auto head = getSkeleton(0)->getJoint(ofxMot::JOINT_NECK);
		head.pan(-90.f);
		const auto v0 = head.getGlobalPosition();
		const auto v1 = mCamTPSParent.getGlobalPosition();
		const auto q0 = head.getGlobalOrientation();
		const auto q1 = mCamTPSParent.getGlobalOrientation();
		const float f {0.95f}; // tps camera smoothing
		ofQuaternion q;
		q.slerp(f, q0, q1);
		mCamTPSParent.setGlobalPosition(v0.interpolated(v1, f));
		mCamTPSParent.setGlobalOrientation(q);
	}

	// main camera animation
	float t {ofClamp(mElapsedTimeMove, 0.f, kCamMoveSpan) / kCamMoveSpan};
	t = easeInOutCubic(t);

	// move camera
	if (mCamMode == CAM_MOVE) {
		auto currCam = mNodes.at(getCurrentNodeID())->getCamera();
		auto nextCam = mNodes.at(getNextNodeID())->getCamera();

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
	// rotate main cam
	else if (mCamMode == CAM_MAIN) {
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
	// do nothing
	else { // TPS and easy cam
		for (auto i : rep(mNodes.size())) {
			mNodes.at(i)->t = 1.f;
		}
	}
}

void SceneFlowChart::drawScene()
{
	// select camera
	ofCamera* camera;
	switch (mCamMode) {
	case CAM_MOVE: camera = &mCurrentCam; break;
	case CAM_MAIN: camera = &mCamMain; break;
	case CAM_TPS: camera = &mCamTPS; break;
	default:
	case CAM_EASY: camera = &mCamEasy; break;
	}

	camera->begin();
	{
		{
			ScopedMatrix m;
			if (mCamMode != CAM_TPS) {
				ofTranslate(0.f, kYOffset, 0.f);
			}

			drawNodes();
			drawDancers();
			drawLines();
		}

		debugDrawCameras();
	}
	camera->end();
}

void SceneFlowChart::drawNodes()
{
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
}

void SceneFlowChart::drawDancers()
{
	for (auto i : rep(getNumSkeletons())) {
		auto skl = getSkeleton(i);
		for (auto& n : skl->getJoints()) {
			n.draw();
			if (!n.getParent()) continue;
			ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
		}
	}
}

void SceneFlowChart::drawLines()
{
	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();

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
			// draw circles
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

void SceneFlowChart::drawToolKit()
{
	ScopedStyle s;
	ScopedMatrix m;

	auto n = getNode<NodeDisplay>();
	n->fbo.begin();
	ofBackground(ofColor::black);
	mCamToolKit.begin();

	for (auto i : rep(getNumSkeletons())) {
		auto skl = getSkeleton(i);
		for (auto& n : skl->getJoints()) {
			ofSetColor(ofColor::blue);
			n.draw();
			{
				ScopedMatrix m;
				ofMultMatrix(n.getGlobalTransformMatrix());
				ofMultMatrix(n.getGlobalTransformMatrix());
				ofSetColor(ofColor::red);
				ofDrawBox(ofVec3f::zero(), 10.f);
			}
			if (!n.getParent()) continue;
			ofSetColor(ofColor::blue);
			ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
		}
	}

	mCamToolKit.end();
	n->fbo.end();
}

void SceneFlowChart::debugDrawCameras()
{
	// debug draw cameras
	if (mCamMode == CAM_EASY) {
		ScopedStyle s;
		ofNoFill();
		// draw cameras for each nodes
		for (auto n : mNodes) {
			const auto c = n->getCamera();
			ofSetColor(ofColor::red);
			ScopedMatrix m;
			ofMultMatrix(c.getGlobalTransformMatrix());
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		// draw current cmaera
		{
			ofSetColor(ofColor::blue);
			ScopedMatrix m;
			ofMultMatrix(mCurrentCam.getGlobalTransformMatrix());
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		// draw TPS camera
		{
			ofSetColor(ofColor::blue);
			ScopedStyle s;
			ScopedMatrix m;
			ofMultMatrix(mCamTPS.getGlobalTransformMatrix());
			ofTranslate(0.f, kYOffset);
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		// draw center positions for each nodes
		{
			ScopedStyle s;
			ofDisableDepthTest();
			ScopedTranslate t(0.f, kYOffset, 0.f);
			ofSetColor(ofColor::green);
			ofFill();
			for (auto n : mNodes) {
				for (auto i : rep(n->getNumAimingPositions())) {
					ScopedTranslate t(n->getAimingPosition(i));
					billboard();
					ofCircle(ofVec3f::zero(), 3.f);
				}
			}
		}
	}
}

void SceneFlowChart::drawHUD()
{
	ScopedStyle s;
	ofFill();
	ofDisableDepthTest();
    
    auto curr= mNodes.at(getCurrentNodeID());
    auto next = mNodes.at(getNextNodeID());

	// display node name
	if (mCamMode == CAM_MOVE && next->t >= 1.f) {
		const string str {next->title};
		{
			ScopedTranslate t(30.f, 120.f);
			ofSetColor(ofColor::white);
			mFont.drawString(str, 0.f, 0.f);
		}
		const string strJP {next->descriptionJP};
		{
			ScopedTranslate t((ofGetWidth() - mFontJP.stringWidth(strJP)) * 0.5f, ofGetHeight() - 20.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
	else if (mCamMode == CAM_MAIN) {
		const string strFrom {curr->title};
		const string strTo {"\n" + next->title};
		{
			ScopedTranslate t(20.f, 80.f);
			ofSetColor(color::kMain);
			mFontSmall.drawString("from\nto", 0.f, 0.f);
			ofSetColor(ofColor::white);
			mFontSmall.drawString(strFrom, mFontSmall.stringWidth("fromx"), 0.f);
			mFontSmall.drawString(strTo, mFontSmall.stringWidth("fromx"), 0.f);
		}
		const string strJP {curr->titleJP + "から" + next->titleJP + "へ"};
		{
			ScopedTranslate t((ofGetWidth() - mFontJP.stringWidth(strJP)) * 0.5f, ofGetHeight() - 20.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
	else if (mCamMode == CAM_TPS) {
		const string str {"Dancer's viewpoint"};
		{
			ScopedTranslate t(30.f, 120.f);
			ofSetColor(ofColor::white);
			mFont.drawString(str, 0.f, 0.f);
		}
		const string strJP {"ダンサー視点"};
		{
			ScopedTranslate t((ofGetWidth() - mFontJP.stringWidth(strJP)) * 0.5f, ofGetHeight() - 20.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
#ifdef DEBUG
	ofSetColor(ofColor::gray);
	ofDrawBitmapString("order=" + ofToString(mOrderIdx), ofGetWidth() - 70.f, ofGetHeight() - 10.f);
#endif
}

void SceneFlowChart::draw()
{
	// draw scen for rear screen
	auto stage = getNode<NodeStage>();

	stage->forFbo = true;
	stage->fbo.begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	drawHUD();
	stage->fbo.end();

	drawToolKit();

	// main draw call
	stage->forFbo = false;
	drawScene();
	drawHUD();
}

void SceneFlowChart::keyPressed(int key)
{
	switch (key) {
	case '1': changeCamMode(CAM_MOVE); break;
	case '2': changeCamMode(CAM_MAIN); break;
	case '3': changeCamMode(CAM_TPS); break;
	case '4': changeCamMode(CAM_EASY); break;
	case '+':
	case '=':
		++mOrderIdx %= mOrders.size();
		break;
	case '-':
	case '_':
		(--mOrderIdx += mOrders.size()) %= mOrders.size();
		break;
	}
}

void SceneFlowChart::changeCamMode(CamMode m)
{
	mCamMode = m;
	switch (m) {
	case CAM_MOVE:
		mNodeIdx = 0;
		mElapsedTimeMove = 0.f;
		break;
	case CAM_MAIN:
		mElapsedTimeMainCam = HALF_PI * (1.f / kMainCamSpeed);
		break;
	case CAM_EASY:
	case CAM_TPS:
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