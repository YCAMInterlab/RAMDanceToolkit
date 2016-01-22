//
//  dpScoreSceneFlowChart.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

// カメラ追加？

#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeMasterHakoniwa.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreNodeDisplay.h"
#include "dpScoreNodeMotioner.h"
#include "dpScoreNodeSpeaker.h"
#include "dpScoreNodeLight.h"
#include "dpScoreNodeAudience.h"
#include "dpScoreNodeCameraUnit.h"
#include "dpScoreNodeComputer.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreShared.h"

static const float kCamMoveSpan = 8.f;
static const float kCamIdleSpan = 8.f;
static const float kLineAnimSpan = 4.f;
static const float kMainCamSpeed = 0.05f;

static const string kNodeNames[] = {
	"Dancers\n(Motion Capture)",
	"Master Hakoniwa",
	"",
	"Hakoniwa x 6",
	"Analyze Hakoniwa",
	"Displays\n(for Dancers)",
	"Speakers",
	"Lights",
	"RAM Dance Toolkit",
	"Audiences",
};

DP_SCORE_NAMESPACE_BEGIN

void SceneFlowChart::initialize()
{
	mFont.loadFont(kFontPath, 80.f);

	// place nodes
	mNodes.clear();
	auto motioner = ofPtr<NodeMotioner>(new NodeMotioner());
	auto masterHakoniwa = ofPtr<NodeMasterHakoniwa>(new NodeMasterHakoniwa());
	masterHakoniwa->setGlobalPosition(ofVec3f(450.f, 0.f, 400.f - Desk::getDimension()));
	auto stage = ofPtr<NodeStage>(new NodeStage());
	auto hakoniwa = ofPtr<NodeHakoniwa>(new NodeHakoniwa());
	hakoniwa->setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 50.f, 0.f, 400.f - NodeHakoniwa::getDepth());
	auto cameraUnit = ofPtr<NodeCameraUnit>(new NodeCameraUnit());
	cameraUnit->setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 250.f, 0.f, 100.f);
	auto display = ofPtr<NodeDisplay>(new NodeDisplay());
	auto speaker = ofPtr<NodeSpeaker>(new NodeSpeaker());
	auto light = ofPtr<NodeLight>(new NodeLight());
	auto computer = ofPtr<NodeComputer>(new NodeComputer());
	computer->setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - Deck::getWidth() - 50.f - 30.f, 0.f, -450.f);
	auto audience = ofPtr<NodeAudience>(new NodeAudience());
	audience->setGlobalPosition(0.f, 0.f,  NodeStage::kDepth * 0.5f + 500.f);

	mNodes.push_back(motioner);
	mNodes.push_back(masterHakoniwa);
	mNodes.push_back(stage);
	mNodes.push_back(hakoniwa);
	mNodes.push_back(cameraUnit);
	mNodes.push_back(display);
	mNodes.push_back(speaker);
	mNodes.push_back(light);
	mNodes.push_back(computer);
	mNodes.push_back(audience);

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

	mCams.at(NODE_MOTIONER).setFov(50.f);
	mCams.at(NODE_MOTIONER).setPosition(0.f, 200.f, 800.f);
	mCams.at(NODE_MOTIONER).setOrientation(ofVec3f(-20.f, 0.f, 0.f));

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

	mCamOrders.clear();
	mCamOrders.push_back(NODE_STAGE);

	mCamOrders.push_back(NODE_MOTIONER);
	mCamOrders.push_back(NODE_MASTER_HAKONIWA);
	mCamOrders.push_back(NODE_COMPUTER);

	mCamOrders.push_back(NODE_STAGE);

	mCamOrders.push_back(NODE_MOTIONER);
	mCamOrders.push_back(NODE_HAKONIWA);
	mCamOrders.push_back(NODE_CAMERA_UNIT);
	mCamOrders.push_back(NODE_COMPUTER);
	mCamOrders.push_back(NODE_DISPLAY);
	mCamOrders.push_back(NODE_MOTIONER);

	mCamOrders.push_back(NODE_STAGE);

	mCamOrders.push_back(NODE_MOTIONER);
	mCamOrders.push_back(NODE_COMPUTER);
	mCamOrders.push_back(NODE_LIGHT);
	mCamOrders.push_back(NODE_SPEAKER);
	mCamOrders.push_back(NODE_MOTIONER);

	mCamOrders.push_back(NODE_STAGE);

	mCamOrders.push_back(NODE_AUDIENCE);
	mCamOrders.push_back(NODE_COMPUTER);

	// bezier line settings
	{
		auto p0 = mNodes.at(NODE_STAGE)->getGlobalPosition();
		auto p1 = mNodes.at(NODE_MASTER_HAKONIWA)->getGlobalPosition();
		p1.x += Desk::getDimension() * 0.5f;
		p1.y += Desk::getHeight() + 50.f;
		p1.z += Desk::getDimension() * 0.5f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_MASTER_HAKONIWA)->getGlobalPosition();
		p0.x += Desk::getDimension() * 0.5f;
		p0.y += Desk::getHeight() + 50.f;
		p0.z += Desk::getDimension() * 0.5f;
		auto p1 = mNodes.at(NODE_COMPUTER)->getGlobalPosition();
		p1.x += Deck::getWidth() * 0.5f;
		p1.y += Deck::getHeight();
		p1.z += Deck::getDepth() * 3.f * 0.5f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_STAGE)->getGlobalPosition();
		auto p1 = mNodes.at(NODE_HAKONIWA)->getGlobalPosition();
		p1.x += NodeHakoniwa::getWidth() * 0.5f;
		p1.y += Desk::getHeight();
		p1.z += NodeHakoniwa::getDepth() * 0.5f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_HAKONIWA)->getGlobalPosition();
		p0.x += Desk::getDimension() * 3 * 0.5f;
		p0.z += Desk::getDimension() * 2 * 0.5f;
		p0.y += Desk::getHeight();
		auto p1 = mNodes.at(NODE_CAMERA_UNIT)->getGlobalPosition();
		p1.x += NodeCameraUnit::getWidth() * 0.5f;
		p1.y += NodeCameraUnit::getHeight() * 0.82f;
		p1.z -= 10.f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_CAMERA_UNIT)->getGlobalPosition();
		p0.x += NodeCameraUnit::getWidth() * 0.5f;
		p0.y += NodeCameraUnit::getHeight() * 0.82f;
		p0.z -= 10.f;
		auto p1 = mNodes.at(NODE_COMPUTER)->getGlobalPosition();
		p1.x += Deck::getWidth() * 0.5f;
		p1.y += Deck::getHeight();
		p1.z += Deck::getDepth() * 3.f * 0.5f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_COMPUTER)->getGlobalPosition();
		p0.x += Deck::getWidth() * 0.5f;
		p0.y += Deck::getHeight();
		p0.z += Deck::getDepth() * 3.f * 0.5f;
		auto p1 = mNodes.at(NODE_LIGHT)->getGlobalPosition();
		p1.y += 800.f;
		mPoints.push_back({p0, p1});
	}
	{
		auto p0 = mNodes.at(NODE_COMPUTER)->getGlobalPosition();
		p0.x += Deck::getWidth() * 0.5f;
		p0.y += Deck::getHeight();
		p0.z += Deck::getDepth() * 3.f * 0.5f;

		auto p1 = mNodes.at(NODE_SPEAKER)->getGlobalPosition();
		p1 += NodeSpeaker::getLeft();
		p1.x += NodeSpeaker::getWidth() * 0.6f;
		p1.y += NodeSpeaker::getHeight() * 0.8f;
		//p1.z += NodeSpeaker::getDepth() * 0.75f;
		mPoints.push_back({p0, p1});
	}

	// main camera settings
	mCamMainParent.setGlobalPosition(ofVec3f::zero());
	mCamMain.setParent(mCamMainParent);
	mCamMain.setGlobalPosition(0.f, 0.f, 1200.f);

	mCamEasy.setDistance(900.f);

	changeCamMode(CAM_MOVE);

	Shared.fbo.allocate(NodeStage::kWidth, NodeStage::kScrH, GL_RGBA16);
}

void SceneFlowChart::shutDown()
{
	mNodes.clear();
	mCams.clear();
	mPoints.clear();
	mCamOrders.clear();
}

void SceneFlowChart::enter()
{
}

void SceneFlowChart::exit()
{
}

void SceneFlowChart::update(ofxEventMessage& m)
{
	if (mCamMode == CAM_MOVE) {
		// move camera
		mElapsedTimeMove += ofGetLastFrameTime();
		if (mElapsedTimeMove >= kCamMoveSpan + kCamIdleSpan) {
			mElapsedTimeMove = 0.f;

			++mCurrentCamOrderIdx %= mCamOrders.size();
		}
		float t {ofClamp(mElapsedTimeMove, 0.f, kCamMoveSpan) / kCamMoveSpan};
		t = easeInOutCubic(t);
		Shared.t = t;

		// open MacBooks
		if (getNextCamID() == NODE_CAMERA_UNIT) {
			Shared.macAngleCameraUnit = t * 110.f;
		}
		else if (getCurrentCamID() == NODE_CAMERA_UNIT) {
			Shared.macAngleCameraUnit = (1.f - t) * 110.f;
		}
		else {
			Shared.macAngleCameraUnit = 0.f;
		}

		if (getNextCamID() == NODE_COMPUTER) {
			Shared.macAngleComputers = t * 110.f;
		}
		else if (getCurrentCamID() == NODE_COMPUTER) {
			Shared.macAngleComputers = (1.f - t) * 110.f;
		}
		else {
			Shared.macAngleComputers = 0.f;
		}

		// move camera
		auto currCam = mCams.at(getCurrentCamID());
		auto nextCam = mCams.at(getNextCamID());

		mCurrentCam.setFov(currCam.getFov() * (1.f - t) + nextCam.getFov() * t);
		mCurrentCam.setGlobalPosition(currCam.getGlobalPosition() * (1.f - t) + nextCam.getGlobalPosition() * t);
		mCurrentCam.setOrientation(currCam.getOrientationEuler() * (1.f - t) + nextCam.getOrientationEuler() * t);
	}
	else {
		// rotate main cam
		mElapsedTimeMainCam += ofGetLastFrameTime() * 0.5f;
		if (mCamMode == CAM_MAIN) {
			const float r {::cosf(mElapsedTimeMainCam * kMainCamSpeed) * 110.f};
			mCamMainParent.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));
		}
		mElapsedTimeLine += ofGetLastFrameTime() * 0.5f;

		// change line
		if (mElapsedTimeLine >= kLineAnimSpan) {
			mElapsedTimeLine = 0.f;
			++mCurrentLine %= mPoints.size();
		}
	}
}

void SceneFlowChart::draw()
{
	auto drawScene = [&]() {
				 ofCamera* camera = &mCamEasy;
				 switch (mCamMode) {
				 case CAM_MOVE: camera = &mCurrentCam; break;
				 case CAM_MAIN: camera = &mCamMain; break;
				 case CAM_EASY: camera = &mCamEasy; break;
				 default: break;
				 }

				 camera->begin();

				 // draw nodes
				 for (auto i : rep(mNodes.size())) {
					 ScopedTranslate t(0.f, -200.f, 0.f);
					 if (mCamMode == CAM_MOVE) {
						 i == getCurrentCamID() ? (Shared.focused = true) : (Shared.focused = false);
						 i == getNextCamID() ? (Shared.nextFocus = true) : (Shared.nextFocus = false);
					 }
					 else {
						 Shared.nextFocus = true;
						 Shared.focused = false;
					 }
					 if (i == NODE_AUDIENCE) {
						 if (getNextCamID() == NODE_AUDIENCE) {
							 mNodes.at(i)->draw();
						 }
						 if (getCurrentCamID() == NODE_AUDIENCE && Shared.t < 1.f) {
							 mNodes.at(i)->draw();
						 }
					 }
					 else {
						 mNodes.at(i)->draw();
					 }
				 }
				 if (mCamMode != CAM_MOVE) {
					 ScopedMatrix m;
					 ofTranslate(0.f, -200.f, 0.f);
					 ScopedStyle s;
					 setStyle();

					 // draw lines
					 auto pp = mPoints.at(mCurrentLine);

					 const auto& p0 = pp.p0;
					 const auto& p1 = pp.p1;
					 auto cp0 = p0.interpolated(p1, 0.25f);
					 auto cp1 = p1.interpolated(p1, 0.75f);
					 cp0.y += 300.f;
					 cp1.y += 300.f;

					 const int res {100};
					 ofPolyline line;
					 line.addVertex(p0);
					 line.bezierTo(cp0, cp1, p1, res);

					 auto v = line.getVertices();
					 float f {::fmodf(mElapsedTimeLine * 2.f, kLineAnimSpan) / kLineAnimSpan};
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
					 {
						 ScopedStyle s;
						 ofDisableDepthTest();
						 ofEnableAlphaBlending();
						 ScopedTranslate t(v.back());
						 billboard();
						 ofFill();
						 ofSetColor(color::kMain);
						 ofCircle(ofVec3f::zero(), 6.f);
						 const float r {::fmodf(mElapsedTimeLine, 2.f) / 2.f};
						 ofSetColor(color::kMain, 200 * (1.f - easeInExpo(r)));
						 ofCircle(ofVec3f::zero(), r * 100.f);
						 ofEnableDepthTest();
					 }
				 }

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
					 ofSetColor(ofColor::blue);
					 ScopedMatrix m;
					 ofMultMatrix(mCurrentCam.getGlobalTransformMatrix());
					 ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
					 ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
				 }

				 camera->end();

				 // display node name
				 if (mCamMode == CAM_MOVE && Shared.t >= 1.f) {
					 ScopedStyle s;
					 ofFill();
					 ofSetColor(ofColor::white);
					 mFont.drawString(kNodeNames[getNextCamID()], 40.f, 120.f);
				 }
			 };

	// draw scen for rear screen
	Shared.forFbo = true;
	Shared.fbo.begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	Shared.fbo.end();

	// main draw call
	Shared.forFbo = false;
	drawScene();
}

void SceneFlowChart::keyPressed(int key)
{
	switch (key) {
	case 'q': changeCamMode(CAM_MOVE); break;
	case 'w': changeCamMode(CAM_MAIN); break;
	case 'e': changeCamMode(CAM_EASY); break;
	}
}

void SceneFlowChart::changeCamMode(CamMode m)
{
	mCamMode = m;
	switch (m) {
	case CAM_MOVE:
		mCurrentCamOrderIdx = 0;
		mElapsedTimeMove = 0;

		break;
	case CAM_MAIN:
		mElapsedTimeMainCam = HALF_PI * (1.f / kMainCamSpeed);
		mElapsedTimeLine = 0.f;
		mCurrentLine = 0;
		Shared.macAngleComputers = Shared.macAngleCameraUnit = 110.f;
		Shared.t = 1.f;
		break;

	case CAM_EASY:
		mElapsedTimeLine = 0.f;
		mCurrentLine = 0;
		Shared.macAngleComputers = Shared.macAngleCameraUnit = 110.f;
		Shared.t = 1.f;
		break;

	default:
		break;
	}
}

int SceneFlowChart::getCurrentCamID() const
{
	return mCamOrders.at(mCurrentCamOrderIdx);
}

int SceneFlowChart::getNextCamID() const
{
	return mCamOrders.at((mCurrentCamOrderIdx + 1) % mCamOrders.size());
}

DP_SCORE_NAMESPACE_END