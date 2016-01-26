//
//  dpScoreSceneFlowChart.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

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

DP_SCORE_NAMESPACE_BEGIN

SceneFlowChart::Property::Property(float move, float idle, float line, bool easeyCam) :
	moveSpan(move),
	idleSpan(idle),
	lineSpan(line)
{
	if (easeyCam) {
		auto easyCam = makeShared<ofEasyCam>();
		easyCam->setDistance(900.f);
		camera = easyCam;
	}
	else {
		camera = makeShared<ofCamera>();
	}
}

void SceneFlowChart::initialize()
{
	setFixPosition(false); // for skeleton

	mFont.loadFont(kFontPath, 80.f);
	mFontSmall.loadFont(kFontPath, 40.f);
	mFontJP.loadFont(kFontPathJP, 32.f, true, true);

	setupNodes();
	setupOrders();
	setupScenes();

	mCamToolKit.setGlobalPosition(0.f, 200.f, 500.f);
	mCamToolKit.setFarClip(1000.f);

	changeScene(SCENE_MOVE);

	windowResized(ofGetWidth(), ofGetHeight());
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

void SceneFlowChart::setupScenes()
{
	mProperties.clear();
	mProperties.push_back(Property(2.f, 2.f, 1.f, false));
	mProperties.push_back(Property(2.f, 2.f, 1.f, false));
	mProperties.push_back(Property(2.f, 2.f, 1.f, false));
	mProperties.push_back(Property(1.f, 0.f, 1.f, false));
	mProperties.push_back(Property(2.f, 2.f, 1.f, false));
	mProperties.push_back(Property(2.f, 2.f, 1.f, true));

	mNodeCenter.setGlobalPosition(ofVec3f::zero());

	mProperties.at(SCENE_MAIN).camera->setParent(mNodeCenter);
	mProperties.at(SCENE_MAIN).camera->setGlobalPosition(0.f, 0.f, 1200.f);

	mProperties.at(SCENE_TPS).camera->setParent(mNodeHead);
	mProperties.at(SCENE_TPS).camera->setGlobalPosition(0.f, 0.f, 200.f);

	mProperties.at(SCENE_CIRCULATION).camera->setGlobalPosition(0.f, 200.f, 1200.f);

	mProperties.at(SCENE_MEMORY).camera->setParent(mNodeCenter);
	mProperties.at(SCENE_MEMORY).camera->setGlobalPosition(0.f, 0.f, 1200.f);
}

void SceneFlowChart::shutDown()
{
	mNodes.clear();
	mOrders.clear();
	mProperties.clear();
}

void SceneFlowChart::enter()
{
}

void SceneFlowChart::exit()
{
}

void SceneFlowChart::update(ofxEventMessage& m)
{
	if (mPaused) return;

	if (ofGetFrameNum() == mLastFrameNum) return;
	mLastFrameNum = ofGetFrameNum();

	updateTime();

	updateWithSkeleton();

	// move camera
	if (mCurrentScene == SCENE_MOVE) {
		updateMovingCam();
	}
	// rotate main cam
	else if (mCurrentScene == SCENE_MAIN) {
		updateMainCam();
	}
	// do nothing
	else { // TPS and easy cam
		for (auto i : rep(mNodes.size())) {
			mNodes.at(i)->t = 1.f;
		}
	}
}

void SceneFlowChart::updateTime()
{
	mElapsedTime += ofGetLastFrameTime();

	// change camera mode
	if (mElapsedTime >= kModeChangeSpan) {
		++mCurrentScene %= SCENE_MEMORY;
		changeScene(mCurrentScene);
		mElapsedTime = 0.f;
	}

	// camera moving and line animation
	auto& prop = mProperties.at(mCurrentScene);
	mTimeCamMove += ofGetLastFrameTime();
	if (mTimeCamMove >= prop.moveSpan + prop.idleSpan) {
		mTimeCamMove = 0.f;
		++mNodeIdx %= mOrders.at(mOrderIdx).size();
	}
}

void SceneFlowChart::updateWithSkeleton()
{
	// update text
	if (getNumSkeletons() >= 2) {
		getNode<NodeDancer>()->title = "Dancers";
		getNode<NodeMotioner>()->title = "Motion Captures";
	}
	else {
		getNode<NodeDancer>()->title = "Dancer";
		getNode<NodeMotioner>()->title = "Motion Capture";
	}

	if (getNumSkeletons()) {
		// update for line
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
		//head.pan(180.f);
		head.pan(-90.f);
		const auto v0 = head.getGlobalPosition();
		const auto v1 = mNodeHead.getGlobalPosition();
		const auto q0 = head.getGlobalOrientation();
		const auto q1 = mNodeHead.getGlobalOrientation();
		const float f {0.99f}; // tps camera smoothing
		ofQuaternion q;
		q.slerp(f, q0, q1);
		mNodeHead.setGlobalPosition(v0.interpolated(v1, f));
		mNodeHead.setGlobalOrientation(q);
	}
}

void SceneFlowChart::updateMovingCam()
{
	auto& prop = mProperties.at(SCENE_MOVE);
	float t {ofClamp(mTimeCamMove, 0.f, prop.moveSpan) / prop.moveSpan};
	t = easeInOutCubic(t);
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
	auto currCam = mNodes.at(getCurrentNodeID())->getCamera();
	auto nextCam = mNodes.at(getNextNodeID())->getCamera();
	auto q0 = currCam.getGlobalOrientation();
	auto q1 = nextCam.getGlobalOrientation();
	ofQuaternion q;
	q.slerp(t, q0, q1);
	auto cam = mProperties.at(SCENE_MOVE).camera;
	cam->setFov(currCam.getFov() * (1.f - t) + nextCam.getFov() * t);
	cam->setGlobalPosition(currCam.getGlobalPosition().interpolated(nextCam.getGlobalPosition(), t));
	cam->setGlobalOrientation(q);
}

void SceneFlowChart::updateMainCam()
{
	mTimeCamRotation += ofGetLastFrameTime();
	const float r {::cosf(mTimeCamRotation * kMainCamSpeed) * 110.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	for (auto i : rep(mNodes.size())) {
		if (i == NODE_LIGHT) {
			mNodes.at(i)->t = 0.f;
		}
		else {
			mNodes.at(i)->t = 1.f;
		}
	}
}

void SceneFlowChart::updateMemoryCam()
{
	mTimeCamRotation += ofGetLastFrameTime();
	const float r {mTimeCamRotation * 100.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	for (auto i : rep(mNodes.size())) {
		if (i == NODE_LIGHT) {
			mNodes.at(i)->t = 0.f;
		}
		else {
			mNodes.at(i)->t = 1.f;
		}
	}
}

void SceneFlowChart::drawScene()
{
	auto cam = mProperties.at(mCurrentScene).camera;

	cam->begin();
	{
		ScopedMatrix m;
		if (mCurrentScene != SCENE_TPS) {
			ofTranslate(0.f, kYOffset, 0.f);
		}

		drawNodes();
		drawDancers();
		if (mCurrentScene != SCENE_MEMORY) {
			drawLines();
		}
	}
	debugDrawCameras();

	cam->end();
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
			auto& prop = mProperties.at(mCurrentScene);
			float f {::fmodf(mTimeCamMove, prop.lineSpan) / prop.lineSpan};
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
				ofSetColor(color::kMain, 150 * (1.f - easeInExpo(r)));
				ofCircle(ofVec3f::zero(), r * 100.f);
				ofEnableDepthTest();
			}
		}
	}
}

void SceneFlowChart::drawToolKit()
{
	ScopedStyle s;
	ofNoFill();

	ScopedMatrix m;

	auto n = getNode<NodeDisplay>();
	n->fbo.begin();
	ofBackground(ofColor::black);
	mCamToolKit.begin();

	ofSetRectMode(OF_RECTMODE_CENTER);
	{
		ScopedRotateX rx(90.f);
		const float d {1000.f};
		const int r {10};
		const float s {d / (float)r};
		ofSetColor(ofColor::white);
		ofRect(ofVec3f::zero(), d, d);
		for (auto i : rep(r)) {
			for (auto j : rep(r)) {
				ofLine(-d * 0.5f + i * s, -d * 0.5f + j * s, -d * 0.5f + (i + 1) * s, -d * 0.5f + j * s);
				ofLine(-d * 0.5f + i * s, -d * 0.5f + j * s, -d * 0.5f + i * s, -d * 0.5f + (j + 1) * s);
			}
		}
	}
	ofEnableDepthTest();
	for (auto i : rep(getNumSkeletons())) {
		auto skl = getSkeleton(i);
		for (auto& n : skl->getJoints()) {
			ofSetColor(ofColor::magenta);
			n.draw();
			if (n.getParent()) {
				ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
			}
			ofNode n2 = n;
			n2.setTransformMatrix(n.getGlobalTransformMatrix() * n2.getGlobalTransformMatrix());
			{
				ScopedMatrix m;
				ofMultMatrix(n2.getGlobalTransformMatrix());
				ofSetColor(ofColor::cyan);
				ofDrawBox(ofVec3f::zero(), 10.f);
			}
			ofSetColor(ofColor::white);
			ofLine(n.getGlobalPosition(), n2.getGlobalPosition());
		}
	}

	mCamToolKit.end();
	n->fbo.end();
}

void SceneFlowChart::debugDrawCameras()
{
	// debug draw cameras
	if (mCurrentScene == SCENE_DEBUG) {
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
			ofMultMatrix(mProperties.at(SCENE_MOVE).camera->getGlobalTransformMatrix());
			ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
			ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
		}
		// draw TPS camera
		{
			ofSetColor(ofColor::blue);
			ScopedStyle s;
			ScopedMatrix m;
			ofMultMatrix(mProperties.at(SCENE_TPS).camera->getGlobalTransformMatrix());
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
	ofEnableAlphaBlending();

	auto curr = mNodes.at(getCurrentNodeID());
	auto next = mNodes.at(getNextNodeID());

	// display node name
	if (mCurrentScene == SCENE_MOVE && next->t >= 1.f) {
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
	else if (mCurrentScene == SCENE_MAIN || mCurrentScene == SCENE_CIRCULATION) {
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
		const string strJP {curr->titleJP + "　→　" + next->titleJP};
		{
			ScopedTranslate t((ofGetWidth() - mFontJP.stringWidth(strJP)) * 0.5f, ofGetHeight() - 20.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
	else if (mCurrentScene == SCENE_TPS) {
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
	// swap buffer
	++mCurrentFbo %= 2;

	// set prev fbo to stage screen
	getNode<NodeStage>()->fbo = &mFbo[(mCurrentFbo + 1) % 2];

	mFbo[mCurrentFbo].begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	drawHUD();
	mFbo[mCurrentFbo].end();

	drawToolKit();

	{
		ScopedStyle s;
		ofSetColor(ofColor::white);
		ofDisableAlphaBlending();
		mFbo[mCurrentFbo].draw(ofVec3f::zero(), ofGetWidth(), ofGetHeight());
	}
}

void SceneFlowChart::keyPressed(int key)
{
	switch (key) {
	case '1': changeScene(SCENE_MOVE); break;
	case '2': changeScene(SCENE_MAIN); break;
	case '3': changeScene(SCENE_TPS); break;
	case '4': changeScene(SCENE_CIRCULATION); break;
	case '5': changeScene(SCENE_MEMORY); break;
	case '6': changeScene(SCENE_DEBUG); break;
	case ' ': mPaused ^= true; break;
	case '+':
	case '=':
		(++mOrderIdx) %= mOrders.size();
		mNodeIdx = 0;
		break;
	case '-':
	case '_':
		((--mOrderIdx) += mOrders.size()) %= mOrders.size();
		mNodeIdx = 0;
		break;
	}
}

void SceneFlowChart::changeScene(int index)
{
	mCurrentScene = index;
	switch (index) {
	case SCENE_MOVE:
		mNodeIdx = 0;
		mTimeCamMove = 0.f;
		break;
	case SCENE_MAIN:
		mTimeCamRotation = HALF_PI * (1.f / kMainCamSpeed);
		break;
	case SCENE_TPS:
		break;
	case SCENE_CIRCULATION:
		break;
	case SCENE_MEMORY:
		LineObj::enableAnimation = true;
		mTimeCamRotation = 0.f;
		break;
	case SCENE_DEBUG:
		break;
	default:
		break;
	}
	if (mCurrentScene != SCENE_MEMORY) {
		LineObj::enableAnimation = false;
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

void SceneFlowChart::windowResized(int w, int h)
{
	for (auto i :rep(2)) {
		mFbo[i].allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
	}
}

DP_SCORE_NAMESPACE_END