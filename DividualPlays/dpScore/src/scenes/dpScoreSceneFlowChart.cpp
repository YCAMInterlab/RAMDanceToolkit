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
#include "dpScoreFlowChartSceneMove.h"
#include "dpScoreFlowChartSceneDescription.h"
#include "dpScoreFlowChartSceneTPS.h"
#include "dpScoreFlowChartSceneCirculation.h"
#include "dpScoreFlowChartSceneMemory.h"
#include "dpScoreFlowChartSceneMasterHakoniwa.h"
#include "dpScoreFlowChartSceneDebug.h"
#include "dpScoreFlowChartRDTK.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreMatrixStack.h"
#include "dpScoreStyle.h"
#include "ofxException.h"
#include "ofxXmlSettings.h"
#include "dpScoreSceneHakoMovies.h"

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

    ofDisableArbTex();
	mFbos.clear();
	mFbos.assign(kNumFbos, ofFbo());
	for (auto& fbo : mFbos) {
		fbo.allocate(kWidth, kHeight, GL_RGBA32F_ARB);
	}

	mRDTK = makeShared<FlowChartRDTK>();

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
	//addNode<NodeAudience>();
}

void SceneFlowChart::setupNodeOrders()
{
	mNodeOrders.clear();
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
		order.push_back(getClassName<NodeDancer>());
		order.push_back(getClassName<NodeMotioner>());
		order.push_back(getClassName<NodeComputer>());
		order.push_back(getClassName<NodeHakoniwa>());
		//order.push_back(getClassName<NodeDisplay>());
		mNodeOrders.push_back(order);
	}
}

void SceneFlowChart::setupScenes()
{
	mScenes.clear();
	addScene<FlowChartSceneMove>()->setup(2.f, 2.f, 1.f, 48.f, false);
	addScene<FlowChartSceneCirculation>()->setup(0.f, 2.f, 2.f, 22.f, false);
	addScene<FlowChartSceneTPS>()->setup(0.f, 1.f, 1.f, 40.f, false);
	addScene<FlowChartSceneDescription>()->setup(4.f, 0.f, 1.f, 48.f, false);
	addScene<FlowChartSceneMasterHakoniwa>()->setup(8.f, 0.f, 1.f, 22.f, false);
	addScene<FlowChartSceneMemory>()->setup(0.f, 0.f, 0.f, 60.f, false);
	addScene<FlowChartSceneDebug>()->setup(2.f, 2.f, 1.f, 600.f, true);

	mSceneOrders.clear();
	mSceneOrders.push_back(getClassName<FlowChartSceneMove>());
	mSceneOrders.push_back(getClassName<FlowChartSceneCirculation>());
	mSceneOrders.push_back(getClassName<FlowChartSceneTPS>());
	mSceneOrders.push_back(getClassName<FlowChartSceneDescription>());
	mSceneOrders.push_back(getClassName<FlowChartSceneMasterHakoniwa>());
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

	updateGlobalTime();

	copySkeletons();

	getCurrentScene()->update(this);
}

void SceneFlowChart::updateGlobalTime()
{
	mElapsedTime += ofGetLastFrameTime();

	// change camera mode
	if (mElapsedTime >= getCurrentScene()->getTotalTime()) {
		++mSceneIdx;
		if (isCurrentScene<FlowChartSceneDebug>()) {
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

void SceneFlowChart::copySkeletons()
{
	mSkeletons.assign(getNumSkeletons(), Skeleton::create());
	for (auto i : rep(getNumSkeletons())) {
		*mSkeletons.at(i) = *getSkeleton(i);
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
		if (p.first == getClassName<NodeMasterHakoniwa>()) {
			continue;
		}
		// only render audiences when they have been focused
        //if (p.first == getClassName<NodeAudience>()) {
        //    if (isNextNode<NodeAudience>() or
        //                (isCurrentScene<NodeAudience>() and p.second->t < 1.f)) {
        //        p.second->draw();
        //    }
        //}
        //else {
		//	p.second->draw();
		//}
        p.second->draw();
	}
	// for blending
	getNode<NodeMasterHakoniwa>()->draw();
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
	if (isCurrentScene<FlowChartSceneMemory>() or
	    isCurrentScene<FlowChartSceneMasterHakoniwa>()) {
		return;
	}

	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();
	ofSetColor(color::kMain);

	auto _drawLines = [&](ofPtr<BaseNode> from, ofPtr<BaseNode> to)
			  {
				  for (auto i : rep(from->getNumAimingPositions())) {
					  for (auto j : rep(to->getNumAimingPositions())) {
						  auto p0 = from->getAimingPosition(i);
						  auto p1 = to->getAimingPosition(j);
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
			  };

	_drawLines(getCurrentNode(), getNextNode());
	if (auto child = getNextNode()->child.lock()) {
		_drawLines(getCurrentNode(), child);
	}
	if (auto parent = getNextNode()->parent.lock()) {
		_drawLines(parent, getNextNode());
	}
}

void SceneFlowChart::drawCircles()
{
	if (isCurrentScene<FlowChartSceneMemory>() or
	    isCurrentScene<FlowChartSceneMasterHakoniwa>()) {
		return;
	}

	ScopedStyle s;
	setStyle();
	ofDisableDepthTest();
	ofEnableAlphaBlending();
	ofSetCircleResolution(64);
	ofFill();

	auto _drawCircles = [&](ofPtr<BaseNode> to)
			    {
				    for (auto i : rep(to->getNumAimingPositions())) {
					    auto p = to->getAimingPosition(i);
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
			    };
	_drawCircles(getNextNode());
	if (auto child = getNextNode()->child.lock()) {
		_drawCircles(child);
	}
	ofEnableDepthTest();
}

void SceneFlowChart::drawToolKit()
{
	ScopedStyle s;
	auto display = getNode<NodeDisplay>();
	display->fbo.begin();
	ofBackground(ofColor::black);
	mCamToolKit.begin();
	if (mRDTK) {
		mRDTK->draw(mSkeletons);
	}
	mCamToolKit.end();
	display->fbo.end();
}

void SceneFlowChart::debugDrawCameras()
{
	if (!isCurrentScene<FlowChartSceneDebug>()) return;

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

	// render into fbo
	mFbos.at(mCurrentFbo).begin();
	ofBackground(0, 0, 0, 255);
	drawScene();
	drawHUD();
	mFbos.at(mCurrentFbo).end();

	drawToolKit();

	// render into screen
	{
		ScopedStyle s;
		ofSetColor(ofColor::white);
		ofDisableAlphaBlending();
		mFbos.at(mCurrentFbo).draw(ofVec3f::zero(), ofGetWidth(), ofGetHeight());
	}
}

void SceneFlowChart::keyPressed(int key)
{
	auto change = [&](int i)
		      {
			      OFX_BEGIN_EXCEPTION_HANDLING
	     changeScene(mSceneOrders.at(i));
			      OFX_END_EXCEPTION_HANDLING
		      };

	switch (key) {
	case '1': change(0); break;
	case '2': change(1); break;
	case '3': change(2); break;
	case '4': change(3); break;
	case '5': change(4); break;
	case '6': change(5); break;
	case '7': change(6); break;
	case '8': change(7); break;
	case ' ':
		mPaused ^= true;
		setPauseElapsedTimeCounter(mPaused);
		break;
	case '+':
	case '=':
		++mOrderIdx %= mNodeOrders.size();
		mNodeIdx = 0;
		break;
	case '-':
	case '_':
		(--mOrderIdx += mNodeOrders.size()) %= mNodeOrders.size();
		mNodeIdx = 0;
		break;
	}
}

void SceneFlowChart::changeScene(const string& name)
{
	for (auto i : rep(mSceneOrders.size())) {
		if (mSceneOrders.at(i) == name) {
			mSceneIdx = i;
			break;
		}
	}
	getCurrentScene()->reset(this);
	mElapsedTime = 0.f;
	mNodeIdx = 0;

	if (name == getClassName<FlowChartSceneDescription>()) {
		mOrderIdx = 1;
		getNode<NodeHakoniwa>()->child = getNode<NodeDisplay>();
		getNode<NodeDancer>()->parent = getNode<NodeDisplay>();
	}
	else {
		mOrderIdx = 0;
		getNode<NodeHakoniwa>()->child.reset();
		getNode<NodeDancer>()->parent.reset();
	}
}

void SceneFlowChart::incrementNode()
{
	++mNodeIdx %= mNodeOrders.at(mOrderIdx).size();

	if (!mOscInited) {
		return;
	}
	if (!isCurrentScene<FlowChartSceneMove>() and
	    !isCurrentScene<FlowChartSceneDescription>() and
	    !isCurrentScene<FlowChartSceneCirculation>()) {
		return;
	}

	auto s = getNextNodeName();
	ofStringReplace(s, "dp::score::Node", "");
	sendLightingOsc(s, getCurrentScene()->getMoveSpan());
}

void SceneFlowChart::sendLightingOsc(const string& s, float f)
{
	if (!mOscInited) return;
	ofxOscMessage m;
	m.setAddress(kOscAddrLighting);
	m.addStringArg(s);
	m.addFloatArg(f);
	mOscSender.sendMessage(m);
}

#pragma mark acceccors
#pragma mark ___________________________________________________________________

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

SceneFlowChart::SkeletonVec& SceneFlowChart::getCopiedSkeletons()
{
	return mSkeletons;
}

const string& SceneFlowChart::getCurrentSceneName() const
{
	return mSceneOrders.at(mSceneIdx);
}

ofPtr<FlowChartBaseScene> SceneFlowChart::getCurrentScene()
{
	return mScenes[mSceneOrders.at(mSceneIdx)];
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