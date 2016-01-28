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
#include "ofxException.h"
#include "ofxXmlSettings.h"

DP_SCORE_NAMESPACE_BEGIN

SceneFlowChart::Property::Property(float move, float idle, float line, float totalTime, bool easeyCam) :
	moveSpan(move),
	idleSpan(idle),
	lineSpan(line),
	totalTime(totalTime)
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

	mFont.loadFont(kFontPath, 112.f);
	mFontSmall.loadFont(kFontPath, 56.f);
	mFontJP.loadFont(kFontPathJP, 50.f, true, true);

	setupNodes();
	setupOrders();
	setupScenes();

	mCamToolKit.setGlobalPosition(0.f, 200.f, 500.f);
	mCamToolKit.setFarClip(1000.f);

	mFbos.clear();
	mFbos.assign(kNumFbos, ofFbo());
	for (auto& fbo : mFbos) {
		fbo.allocate(kWidth, kHeight, GL_RGBA32F_ARB);
	}

	changeScene(SCENE_MOVE);

	ofxXmlSettings xml;
	xml.load(kXmlSettingsPathFlowChart);
	xml.pushTag("lighting");
	auto host = xml.getAttribute("osc", "host", "127.0.0.1");
	auto port = xml.getAttribute("osc", "port", 10001);
	xml.popTag();

	mOscSender.setup(host, port);
}

void SceneFlowChart::setupNodes()
{
	// place nodes
	mNodes.clear();
	addNode<NodeMotioner>();
	addNode<NodeDancer>();
	addNode<NodeMasterHakoniwa>();
	addNode<NodeStage>();
	addNode<NodeHakoniwa>();
	addNode<NodeCameraUnit>();
	addNode<NodeDisplay>();
	addNode<NodeSpeaker>();
	addNode<NodeLight>();
	addNode<NodeComputer>();
	addNode<NodeAudience>();
}

void SceneFlowChart::setupOrders()
{
	mOrders.clear();
	{
		vector<string> order;
		order.push_back(getClassName<NodeStage>());
		order.push_back(getClassName<NodeStage>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeDisplay>());
		order.push_back(getClassName<NodeDancer>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeHakoniwa>());
		order.push_back(getClassName<NodeCameraUnit>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeDisplay>());
		order.push_back(getClassName<NodeDancer>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeMasterHakoniwa>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeHakoniwa>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeAudience>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeDisplay>());
		order.push_back(getClassName<NodeDancer>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeLight>());
		order.push_back(getClassName<NodeDancer>());
		mOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeSpeaker>());
		order.push_back(getClassName<NodeDancer>());
		mOrders.push_back(order);
	}
}

void SceneFlowChart::setupScenes()
{
	mProperties.clear();
	mProperties[SCENE_MOVE] = Property(2.f, 2.f, 1.f, 60.f, false);
	mProperties[SCENE_MAIN] = Property(2.f, 2.f, 1.f, 60.f, false);
	mProperties[SCENE_TPS] = Property(2.f, 2.f, 1.f, 30.f, false);
	mProperties[SCENE_CIRCULATION] = Property(2.f, 0.f, 2.f, 30.f, false);
	mProperties[SCENE_MEMORY] = Property(0.f, 0.f, 0.f, 60.f, false);
	mProperties[SCENE_DEBUG] = Property(2.f, 2.f, 1.f, 60.f, true);

	mNodeCenter.setGlobalPosition(ofVec3f::zero());

	mProperties[SCENE_MAIN].camera->setParent(mNodeCenter);
	mProperties[SCENE_MAIN].camera->setGlobalPosition(0.f, 200.f, 1200.f);

	mProperties[SCENE_TPS].camera->setParent(mNodeHead);
	mProperties[SCENE_TPS].camera->setGlobalPosition(0.f, 200.f, 200.f);

	mProperties[SCENE_CIRCULATION].camera->setGlobalPosition(-NodeStage::kWidth * 0.5f, 600.f, 800.f);
	mProperties[SCENE_CIRCULATION].camera->setOrientation(ofVec3f(-30.f, 0.f, 0.f));

	mProperties[SCENE_MEMORY].camera->setParent(mNodeCenter);
	mProperties[SCENE_MEMORY].camera->setGlobalPosition(0.f, 300.f, 1000.f);
}

void SceneFlowChart::shutDown()
{
	mNodes.clear();
	mOrders.clear();
	mProperties.clear();
	mFbos.clear();
}

void SceneFlowChart::enter()
{
	changeScene(SCENE_MOVE);
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
	else if (mCurrentScene == SCENE_MEMORY) {
		updateMemoryCam();
	}
	// do nothing
	else { // TPS and easy cam
		for (auto& p : mNodes) {
			p.second->t = 1.f;
		}
	}
}

void SceneFlowChart::updateTime()
{
	mElapsedTime += ofGetLastFrameTime();

	// change camera mode
	if (mElapsedTime >= mProperties[mCurrentScene].totalTime) {
		++mCurrentScene %= SCENE_DEBUG;
		changeScene(mCurrentScene);
		mElapsedTime = 0.f;
	}

	// camera moving and line animation
	auto& prop = mProperties[mCurrentScene];
	mTimeCamMove += ofGetLastFrameTime();
	if (mTimeCamMove >= prop.moveSpan + prop.idleSpan) {
		mTimeCamMove = 0.f;
		++mNodeIdx %= mOrders.at(mOrderIdx).size();

		if (mCurrentScene == SCENE_MOVE || mCurrentScene == SCENE_CIRCULATION || mCurrentScene == SCENE_MAIN) {
			ofxOscMessage m;
			m.setAddress("/dp/light/moving");
            auto s = getCurrentNodeName();
            ofStringReplace(s, "dp::score::Node", "");
			m.addStringArg(s);
			m.addFloatArg(mProperties[mCurrentScene].moveSpan);
			mOscSender.sendMessage(m);
		}
	}
}

void SceneFlowChart::updateWithSkeleton()
{
	mSkeletons.assign(getNumSkeletons(), Skeleton::create());
	for (auto i : rep(getNumSkeletons())) {
		*mSkeletons.at(i) = *getSkeleton(i);
	}

	// update text
	if (mSkeletons.size() >= 2) {
		getNode<NodeDancer>()->title = "Dancers";
		getNode<NodeMotioner>()->title = "Motion Captures";
	}
	else {
		getNode<NodeDancer>()->title = "Dancer";
		getNode<NodeMotioner>()->title = "Motion Capture";
	}

	if (mSkeletons.empty() == false) {
		// update for line
		auto motioner = mNodes[getClassName < NodeMotioner > ()];
		motioner->clearAimingOffsets();
		for (auto i : rep(mSkeletons.size())) {
			motioner->addAimingOffset(mSkeletons.at(i)->getJoint(ofxMot::JOINT_HIPS).getGlobalPosition());
		}
		auto dancer = mNodes[getClassName < NodeDancer > ()];
		dancer->clearAimingOffsets();
		for (auto i : rep(mSkeletons.size())) {
			dancer->addAimingOffset(mSkeletons.at(i)->getJoint(ofxMot::JOINT_HEAD).getGlobalPosition());
		}

		// TPS camera
		auto head = mSkeletons.front()->getJoint(ofxMot::JOINT_NECK);
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
	auto& prop = mProperties[SCENE_MOVE];
	float t {ofClamp(mTimeCamMove, 0.f, prop.moveSpan) / prop.moveSpan};
	t = easeInOutCubic(t);
	for (auto& p : mNodes) {
		if (p.first == getNextNodeName()) {
			p.second->t = t;
		}
		else if (p.first == getCurrentNodeName()) {
			p.second->t = 1.f - t;
		}
		else {
			p.second->t = 0.f;
		}
	}
	auto currCam = getCurrentNode()->getCamera();
	auto nextCam = getNextNode()->getCamera();
	auto p = currCam.getGlobalPosition().interpolated(nextCam.getGlobalPosition(), t);
	auto q0 = currCam.getGlobalOrientation();
	auto q1 = nextCam.getGlobalOrientation();
	ofQuaternion q;
	q.slerp(t, q0, q1);
	auto cam = mProperties[SCENE_MOVE].camera;
	cam->setFov(currCam.getFov() * (1.f - t) + nextCam.getFov() * t);
	cam->setGlobalPosition(p);
	cam->setGlobalOrientation(q);
}

void SceneFlowChart::updateMainCam()
{
	mTimeCamRotation += ofGetLastFrameTime();
	const float r {::cosf(mTimeCamRotation * kMainCamSpeed) * 110.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	for (auto& p : mNodes) {
		if (p.first == getClassName<NodeLight>()) {
			p.second->t = 0.f;
		}
		else {
			p.second->t = 1.f;
		}
	}
}

void SceneFlowChart::updateMemoryCam()
{
	mTimeCamRotation += ofGetLastFrameTime();

	if (mTimeCamRotation >= 10.f) {
		LineObj::enableAnimation = true;
	}

	const float r {-mTimeCamRotation * 5.f + 45.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	for (auto& p : mNodes) {
		if (p.first == getClassName<NodeLight>()) {
			p.second->t = 0.f;
		}
		else {
			p.second->t = 1.f;
		}
	}
}

void SceneFlowChart::drawScene()
{
	auto cam = mProperties[mCurrentScene].camera;

	cam->begin();
	{
		drawNodes();
		drawDancers();
		drawLines();
	}
	debugDrawCameras();

	cam->end();

	drawCircles();
}

void SceneFlowChart::drawNodes()
{
	for (auto& p : mNodes) {
		// only render audiences when they have been focused
		if (p.first == getClassName<NodeAudience>()) {
			if (getNextNodeName() == getClassName<NodeAudience>() ||
			    (getCurrentNodeName() == getClassName<NodeAudience>() && p.second->t < 1.f)) {
				p.second->draw();
			}
		}
		else {
			p.second->draw();
		}
	}
}

void SceneFlowChart::drawDancers()
{
	for (auto skl : mSkeletons) {
		for (auto& n : skl->getJoints()) {
			n.draw();
			if (!n.getParent()) continue;
			ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
		}
	}
}

void SceneFlowChart::drawLines()
{
	if (mCurrentScene == SCENE_MEMORY) return;

	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();
	ofSetColor(color::kMain);
	auto currNode = getCurrentNode();
	auto nextNode = getNextNode();
	for (auto i : rep(currNode->getNumAimingPositions())) {
		for (auto j : rep(nextNode->getNumAimingPositions())) {
			auto p0 = currNode->getAimingPosition(i);
			auto p1 = nextNode->getAimingPosition(j);
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
			auto& prop = mProperties[mCurrentScene];
			float f {::fmodf(mTimeCamMove, prop.lineSpan) / prop.lineSpan};
			f = ofClamp(f, 0.f, 1.f);
			f = easeInOutQuad(f);
			const float len {0.5f};
			const float fb {ofMap(f, len, 1.f, 0.f, 1.f, true)};
			const float fe {ofMap(f, 0.f, 1.f - len, 0.f, 1.f, true)};
			for (int i = (v.size() - 1) * fb; i < (v.size() - 1) * fe; i++) {
				ofLine(v.at(i), v.at(i + 1));
			}
		}
	}
	ofEnableDepthTest();
}

void SceneFlowChart::drawCircles()
{
	if (mCurrentScene == SCENE_MEMORY) return;

	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();
	ofEnableAlphaBlending();
	ofSetCircleResolution(64);
	ofFill();
	auto nextNode = getNextNode();
	for (auto i : rep(nextNode->getNumAimingPositions())) {
		auto p = nextNode->getAimingPosition(i);
		auto& prop = mProperties[mCurrentScene];
		ofVec3f screen = prop.camera->worldToScreen(p);
		float f {::fmodf(mTimeCamMove, prop.lineSpan) / prop.lineSpan};
		f = ofClamp(f, 0.f, 1.f);
		f = easeInOutQuad(f);
		const float len {0.5f};
		if (f >= len && screen.z < 1.f) {
			ScopedTranslate t(screen.x, screen.y, 0.f);
			ofSetColor(color::kMain);
			ofCircle(ofVec3f::zero(), 10.f);
			const float r {ofMap(f, len, 1.f, 0.f, 1.f)};
			ofSetColor(color::kMain, 150 * (1.f - easeInExpo(r)));
			ofCircle(ofVec3f::zero(), r * 200.f);
		}
	}
	ofEnableDepthTest();
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
	for (auto skl : mSkeletons) {
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
	if (mCurrentScene != SCENE_DEBUG) return;

	ScopedStyle s;
	ofNoFill();
	// draw cameras for each nodes
	for (auto& p : mNodes) {
		const auto c = p.second->getCamera();
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
		ofMultMatrix(mProperties[SCENE_MOVE].camera->getGlobalTransformMatrix());
		ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
		ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
	}
	// draw TPS camera
	{
		ofSetColor(ofColor::blue);
		ScopedStyle s;
		ScopedMatrix m;
		ofMultMatrix(mProperties[SCENE_TPS].camera->getGlobalTransformMatrix());
		ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
		ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
	}
	// draw center positions for each nodes
	{
		ScopedStyle s;
		ofDisableDepthTest();
		ofSetColor(ofColor::green);
		ofFill();
		for (auto& p : mNodes) {
			for (auto i : rep(p.second->getNumAimingPositions())) {
				ScopedTranslate t(p.second->getAimingPosition(i));
				billboard();
				ofCircle(ofVec3f::zero(), 3.f);
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

	auto curr = getCurrentNode();
	auto next = getNextNode();

	// display node name
	if (mCurrentScene == SCENE_MOVE && next->t >= 1.f) {
		const string str {next->title};
		{
			ScopedTranslate t(25.f, 170.f);
			ofSetColor(ofColor::white);
			mFont.drawString(str, 0.f, 0.f);
		}
		const string strJP {next->descriptionJP};
		{
			ScopedTranslate t((kWidth - mFontJP.stringWidth(strJP)) * 0.5f, kHeight - 40.f);
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
			ScopedTranslate t(25.f, 112.f);
			ofSetColor(color::kMain);
			mFontSmall.drawString("from\nto", 0.f, 0.f);
			ofSetColor(ofColor::white);
			mFontSmall.drawString(strFrom, mFontSmall.stringWidth("fromx"), 0.f);
			mFontSmall.drawString(strTo, mFontSmall.stringWidth("fromx"), 0.f);
		}
		const string strJP {curr->titleJP + "　→　" + next->titleJP};
		{
			ScopedTranslate t((kWidth - mFontJP.stringWidth(strJP)) * 0.5f, kHeight - 40.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
	else if (mCurrentScene == SCENE_TPS) {
		const string str {"Dancer's viewpoint"};
		{
			ScopedTranslate t(25.f, 170.f);
			ofSetColor(ofColor::white);
			mFont.drawString(str, 0.f, 0.f);
		}
		const string strJP {"ダンサー視点"};
		{
			ScopedTranslate t((kWidth - mFontJP.stringWidth(strJP)) * 0.5f, kHeight - 40.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
	else if (mCurrentScene == SCENE_MEMORY) {
		const string str {"Dancer's memory"};
		{
			ScopedTranslate t(25.f, 170.f);
			ofSetColor(ofColor::white);
			mFont.drawString(str, 0.f, 0.f);
		}
		const string strJP {"記憶"};
		{
			ScopedTranslate t((kWidth - mFontJP.stringWidth(strJP)) * 0.5f, kHeight - 40.f);
			ofSetColor(ofColor::black, 180);
			ofRect(mFontJP.getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			mFontJP.drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
#ifdef DEBUG
	ofSetColor(ofColor::gray);
	ofDrawBitmapString("order=" + ofToString(mOrderIdx), kWidth - 70.f, kHeight - 10.f);
#endif
}

void SceneFlowChart::draw()
{
	// swap buffer
	++mCurrentFbo %= kNumFbos;

	// set prev fbo to stage screen
	getNode<NodeStage>()->fbo = &mFbos.at((mCurrentFbo + 1) % kNumFbos);

	mFbos.at(mCurrentFbo).begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	drawHUD();
	mFbos.at(mCurrentFbo).end();

	drawToolKit();

	{
		ScopedStyle s;
		ofSetColor(ofColor::white);
		ofDisableAlphaBlending();
		mFbos.at(mCurrentFbo).draw(ofVec3f::zero(), ofGetWidth(), ofGetHeight());
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
	case ' ':
		mPaused ^= true;
		setPauseElapsedTimeCounter(mPaused);
		break;
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
	ofxOscMessage m;
	m.setAddress("/dp/light/moving");

	switch (index) {
	case SCENE_MOVE:
		mNodeIdx = 0;
		mTimeCamMove = 0.f;
		break;
	case SCENE_MAIN:
		mTimeCamRotation = HALF_PI * (1.f / kMainCamSpeed);
		break;
	case SCENE_TPS:
		m.addStringArg("Stage");
		m.addFloatArg(0.f);
		mOscSender.sendMessage(m);
		break;
	case SCENE_CIRCULATION:
		getNode<NodeHakoniwa>()->setFocus(false);
		break;
	case SCENE_MEMORY:
		mTimeCamRotation = 0.f;
		m.addStringArg("Off");
		m.addFloatArg(0.f);
		mOscSender.sendMessage(m);
		break;
	case SCENE_DEBUG:
		m.addStringArg("Stage");
		m.addFloatArg(0.f);
		mOscSender.sendMessage(m);
		break;
	default:
		break;
	}
	if (mCurrentScene != SCENE_MEMORY) {
		LineObj::enableAnimation = false;
	}
	if (mCurrentScene != SCENE_CIRCULATION) {
		getNode<NodeHakoniwa>()->setFocus(true);
	}
}

const string& SceneFlowChart::getCurrentNodeName() const
{
	return mOrders.at(mOrderIdx).at(mNodeIdx);
}

const string& SceneFlowChart::getNextNodeName() const
{
	return mOrders.at(mOrderIdx).at((mNodeIdx + 1) % mOrders.at(mOrderIdx).size());
}

ofPtr<BaseNode> SceneFlowChart::getCurrentNode()
{
	return mNodes[getCurrentNodeName()];
}

ofPtr<BaseNode> SceneFlowChart::getNextNode()
{
	return mNodes[getNextNodeName()];
}

DP_SCORE_NAMESPACE_END