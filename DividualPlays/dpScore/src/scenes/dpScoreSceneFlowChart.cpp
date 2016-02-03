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
#include "dpScoreSceneHakoMovies.h"
#include "dpScoreMatrixStack.h"
#include "dpScoreFlowChartSceneMove.h"
#include "dpScoreFlowChartSceneDescription.h"
#include "dpScoreFlowChartSceneTPS.h"
#include "dpScoreFlowChartSceneDebug.h"
#include "dpScoreFlowChartSceneCirculation.h"
#include "dpScoreFlowChartSceneMemory.h"
#include "dpScoreFlowChartSceneMasterHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneFlowChart::initialize()
{
	setFixPosition(false); // for skeleton

	mFont.loadFont(kFontPath, 112.f);
	mFontSmall.loadFont(kFontPath, 56.f);
	mFontJP.loadFont(kFontPathJP, 50.f, true, true);

	setupNodes();
	setupNodeOrders();
	setupScenes();

	mCamToolKit.setGlobalPosition(0.f, 200.f, 500.f);
	mCamToolKit.setFarClip(1000.f);

	mFbos.clear();
	mFbos.assign(kNumFbos, ofFbo());
	for (auto& fbo : mFbos) {
		fbo.allocate(kWidth, kHeight, GL_RGBA32F_ARB);
	}

	changeScene<FlowChartSceneMove>();

	OFX_BEGIN_EXCEPTION_HANDLING
	ofxXmlSettings xml;
	xml.load(kXmlSettingsPathFlowChart);
	xml.pushTag("lighting");
	auto host = xml.getAttribute("osc", "host", "127.0.0.1");
	auto port = xml.getAttribute("osc", "port", 10001);
	xml.popTag();
	mOscSender.setup(host, port);
	mOscInited = true;
	OFX_END_EXCEPTION_HANDLING
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

void SceneFlowChart::setupNodeOrders()
{
	mNodeOrders.clear();
	{
		vector<string> order;
		order.push_back(getClassName<NodeStage>());
		order.push_back(getClassName<NodeStage>());
		mNodeOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeDisplay>());
		order.push_back(getClassName<NodeDancer>());
		mNodeOrders.push_back(order);
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
		mNodeOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeMasterHakoniwa>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeHakoniwa>());
		mNodeOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeAudience>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeDisplay>());
		order.push_back(getClassName<NodeDancer>());
		mNodeOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeLight>());
		order.push_back(getClassName<NodeDancer>());
		mNodeOrders.push_back(order);
	}
	{
		vector<string> order;
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeSpeaker>());
		order.push_back(getClassName<NodeDancer>());
		mNodeOrders.push_back(order);
	}
}

void SceneFlowChart::setupScenes()
{
	mScenes.clear();
	addScene<FlowChartSceneMove>()->setup(2.f, 2.f, 1.f, 60.f, false);
	addScene<FlowChartSceneCirculation>()->setup(0.f, 2.f, 2.f, 30.f, false);
	addScene<FlowChartSceneTPS>()->setup(0.f, 1.f, 1.f, 60.f, false);
	addScene<FlowChartSceneDescription>()->setup(8.f, 0.f, 1.f, 90.f, false);
	addScene<FlowChartSceneMemory>()->setup(0.f, 0.f, 0.f, 60.f, false);
	addScene<FlowChartSceneDebug>()->setup(2.f, 2.f, 1.f, 600.f, true);

	mSceneOrders.clear();
	mSceneOrders.push_back(getClassName<FlowChartSceneMove>());
	mSceneOrders.push_back(getClassName<FlowChartSceneCirculation>());
	mSceneOrders.push_back(getClassName<FlowChartSceneTPS>());
	mSceneOrders.push_back(getClassName<FlowChartSceneDescription>());
	mSceneOrders.push_back(getClassName<FlowChartSceneMemory>());
	mSceneOrders.push_back(getClassName<FlowChartSceneDebug>());
}

void SceneFlowChart::shutDown()
{
	mNodes.clear();
	mNodeOrders.clear();
	mScenes.clear();
	mFbos.clear();
}

void SceneFlowChart::enter()
{
	changeScene<FlowChartSceneMove>();
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

	getCurrentScene()->update(this);
}

void SceneFlowChart::updateTime()
{
	mElapsedTime += ofGetLastFrameTime();

	// change camera mode
	if (mElapsedTime >= getCurrentScene()->getTotalTime()) {
		++mSceneIdx;
		if (getCurrentSceneName() == getClassName<FlowChartSceneCirculation>()) {
			ofxEventMessage m;
			m.setAddress(kEventAddrChangeScene);
			m.addStringArg(getClassName<SceneHakoMovies>());
			ofxNotifyEvent(m);
		}
		else {
			changeScene(getCurrentSceneName());
		}
		mElapsedTime = 0.f;
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
		auto& target = getCurrentScene()->getNodeCenter();
		auto head = mSkeletons.front()->getJoint(ofxMot::JOINT_NECK);
		//head.pan(180.f);
		head.pan(-90.f);
		const auto v0 = head.getGlobalPosition();
		const auto v1 = target.getGlobalPosition();
		const auto q0 = head.getGlobalOrientation();
		const auto q1 = target.getGlobalOrientation();
		const float f {0.99f}; // tps camera smoothing
		ofQuaternion q;
		q.slerp(f, q0, q1);
		target.setGlobalPosition(v0.interpolated(v1, f));
		target.setGlobalOrientation(q);
	}
}

void SceneFlowChart::drawScene()
{
	auto cam = getCurrentScene()->getCamera();

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
			if (getNextNodeName() == getClassName<NodeAudience>() or
			            (getCurrentNodeName() == getClassName<NodeAudience>()
			            and p.second->t < 1.f)) {
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
			if (!n.getParent()) {
				continue;
			}
			ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
		}
	}
}

void SceneFlowChart::drawLines()
{
	if (getCurrentSceneName() == getClassName<FlowChartSceneMemory>()) {
		return;
	}

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
			auto s = getCurrentScene();
			float f {::fmodf(s->getLineTime(), s->getLineSpan()) / s->getLineSpan()};
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
}

void SceneFlowChart::drawCircles()
{
	if (getCurrentSceneName() == getClassName<FlowChartSceneMemory>()) {
		return;
	}

	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();
	ofEnableAlphaBlending();
	ofSetCircleResolution(64);
	ofFill();
	auto nextNode = getNextNode();
	for (auto i : rep(nextNode->getNumAimingPositions())) {
		auto p = nextNode->getAimingPosition(i);
		auto s = getCurrentScene();
		ofVec3f screen = s->getCamera()->worldToScreen(p);
		float f {::fmodf(s->getLineTime(), s->getLineSpan()) / s->getLineSpan()};
		f = ofClamp(f, 0.f, 1.f);
		f = easeInOutQuad(f);
		const float len {0.5f};
		if (f >= len and screen.z < 1.f) {
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
	if (getCurrentSceneName() != getClassName<FlowChartSceneDebug>()) return;

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
		ofMultMatrix(getCurrentScene()->getCamera()->getGlobalTransformMatrix());
		ofDrawBox(ofVec3f(0.f, 0.f, 25.f), 20.f, 20.f, 50.f);
		ofDrawBox(ofVec3f(0.f, 0.f, -5.f), 10.f, 10.f, 10.f);
	}
	// draw TPS camera
	{
		ofSetColor(ofColor::blue);
		ScopedStyle s;
		ScopedMatrix m;
		ofMultMatrix(getCurrentScene()->getCamera()->getGlobalTransformMatrix());
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

	// display node name
	getCurrentScene()->drawText(this);

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
	case '1': changeScene(mSceneOrders.at(0)); break;
	case '2': changeScene(mSceneOrders.at(1)); break;
	case '3': changeScene(mSceneOrders.at(2)); break;
	case '4': changeScene(mSceneOrders.at(3)); break;
	case '5': changeScene(mSceneOrders.at(4)); break;
	case '6': changeScene(mSceneOrders.at(5)); break;
	case '7': changeScene(mSceneOrders.at(6)); break;
	case ' ':
		mPaused ^= true;
		setPauseElapsedTimeCounter(mPaused);
		break;
	case '+':
	case '=':
		(++mOrderIdx) %= mNodeOrders.size();
		mNodeIdx = 0;
		break;
	case '-':
	case '_':
		((--mOrderIdx) += mNodeOrders.size()) %= mNodeOrders.size();
		mNodeIdx = 0;
		break;
	}
}

map<string, ofPtr<BaseNode> >& SceneFlowChart::getNodes()
{
	return mNodes;
}

const map<string, ofPtr<BaseNode> >& SceneFlowChart::getNodes() const
{
	return mNodes;
}

const string& SceneFlowChart::getCurrentNodeName() const
{
	return mNodeOrders.at(mOrderIdx).at(mNodeIdx);
}

const string& SceneFlowChart::getNextNodeName() const
{
	return mNodeOrders.at(mOrderIdx).at((mNodeIdx + 1) % mNodeOrders.at(mOrderIdx).size());
}

ofPtr<BaseNode> SceneFlowChart::getCurrentNode()
{
	return mNodes[getCurrentNodeName()];
}

ofPtr<BaseNode> SceneFlowChart::getNextNode()
{
	return mNodes[getNextNodeName()];
}

void SceneFlowChart::incrementNode()
{
	++mNodeIdx %= mNodeOrders.at(mOrderIdx).size();

	if (!mOscInited) {
		return;
	}
	if (getCurrentSceneName() != getClassName<FlowChartSceneMove>() and
	    getCurrentSceneName() != getClassName<FlowChartSceneDescription>() and
	    getCurrentSceneName() != getClassName<FlowChartSceneCirculation>()) {
		return;
	}

	ofxOscMessage m;
	m.setAddress(kOscAddrLighting);
	auto s = getNextNodeName();
	ofStringReplace(s, "dp::score::Node", "");
	m.addStringArg(s);
	m.addFloatArg(getCurrentScene()->getMoveSpan());
	mOscSender.sendMessage(m);
};

const string& SceneFlowChart::getCurrentSceneName() const
{
	return mSceneOrders.at(mSceneIdx);
}

ofPtr<FlowChartBaseScene> SceneFlowChart::getCurrentScene()
{
	return mScenes[mSceneOrders.at(mSceneIdx)];
}

void SceneFlowChart::changeScene(const string& name)
{
	for (auto i : rep(mSceneOrders.size())) {
		if (mSceneOrders.at(i) == name) {
			mSceneIdx = i;
			break;
		}
	}
	getCurrentScene()->reset();
	mElapsedTime = 0.f;
	mNodeIdx = 0;

	auto sendOsc = [&](const string& s, float f) {
			       if (!mOscInited) return;
			       ofxOscMessage m;
			       m.setAddress(kOscAddrLighting);
			       m.addStringArg(s);
			       m.addFloatArg(f);
			       mOscSender.sendMessage(m);
		       };

	if (name == getClassName<FlowChartSceneCirculation>() or
	    name == getClassName<FlowChartSceneDescription>()) {
		getNode<NodeHakoniwa>()->setFocus(false);
	}
	else {
		getNode<NodeHakoniwa>()->setFocus(true);
	}

	if (name == getClassName<FlowChartSceneTPS>() or
	    name == getClassName<FlowChartSceneDebug>()) {
		sendOsc("Stage", 0.f);
	}
	else if (name == getClassName<FlowChartSceneMemory>()) {
		sendOsc("Off", 0.f);
	}

	if (name != getClassName<FlowChartSceneMemory>()) {
		LineObj::enableAnimation = false;
	}
}

ofTrueTypeFont& SceneFlowChart::getFont()
{
	return mFont;
}

ofTrueTypeFont& SceneFlowChart::getFontSmall()
{
	return mFontSmall;
}

ofxTrueTypeFontUC& SceneFlowChart::getFontJP()
{
	return mFontJP;
}

DP_SCORE_NAMESPACE_END