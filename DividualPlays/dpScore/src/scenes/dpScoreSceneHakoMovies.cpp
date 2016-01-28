//
//  dpScoreSceneHakoMovies.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#include "dpScoreSceneHakoMovies.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreSceneFlowChart.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneHakoMovies::initialize()
{
	auto nama = ofPtr<ofVideoPlayer>(new ofVideoPlayer());
	auto vis = ofPtr<ofVideoPlayer>(new ofVideoPlayer());

	nama->loadMovie("movies/hakonama.mp4");
	vis->loadMovie("movies/hakovis.mp4");

	mMovies.clear();
	mMovies.push_back(nama);
	mMovies.push_back(vis);
}

void SceneHakoMovies::shutDown()
{

}

void SceneHakoMovies::enter()
{
	for (auto p : mMovies) {
		p->play();
	}

	mAlphaNama = 1.f;
	mAlphaVis = 0.f;
	mDisplayType = DISPLAY_NAMA;
	mElapsedTime = 0.f;
}

void SceneHakoMovies::exit()
{
	for (auto p : mMovies) {
		p->stop();
	}
}

void SceneHakoMovies::update(ofxEventMessage& m)
{
	if (mLastFrame == ofGetFrameNum()) return;
	mLastFrame = ofGetFrameNum();

	mElapsedTime += ofGetLastFrameTime();
	if (mElapsedTime >= kFadeDur) {
		mElapsedTime = 0.f;
		//++mDisplayType %=  NUM_DISPLAY_TYPES;
        ++mDisplayType;
        if (mDisplayType == NUM_DISPLAY_TYPES) {
            mDisplayType = DISPLAY_MIX;
            ofxEventMessage m;
            m.setAddress(kEventAddrChangeScene);
            m.addStringArg(getClassName<SceneFlowChart>());
            ofxNotifyEvent(m);
        }
	}

	for (auto p : mMovies) {
		p->update();
	}

	switch (mDisplayType) {
	case DISPLAY_NAMA:
		mAlphaNama += ofGetLastFrameTime() * kFadeSpeed;
		mAlphaNama = ofClamp(mAlphaNama, 0.f, 1.f);
		mAlphaVis -= ofGetLastFrameTime() * kFadeSpeed;
		mAlphaVis = ofClamp(mAlphaVis, 0.f, 1.f);
		break;
	case DISPLAY_VIS:
		mAlphaNama -= ofGetLastFrameTime() * kFadeSpeed;
		mAlphaNama = ofClamp(mAlphaNama, 0.f, 1.f);
		mAlphaVis += ofGetLastFrameTime() * kFadeSpeed;
		mAlphaVis = ofClamp(mAlphaVis, 0.f, 1.f);
		break;
	case DISPLAY_MIX:
		mAlphaNama += ofGetLastFrameTime() * kFadeSpeed;
		mAlphaNama = ofClamp(mAlphaNama, 0.f, 1.f);
		mAlphaVis += ofGetLastFrameTime() * kFadeSpeed;
		mAlphaVis = ofClamp(mAlphaVis, 0.f, 1.f);
		break;
	default:
		break;
	}
}

void SceneHakoMovies::draw()
{
	ScopedStyle style;
	ofDisableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	{
		ofSetColor(ofColor::white, 255 * mAlphaNama);
		auto m = mMovies.at(MOVIE_NAMA);
		const float s = ofGetHeight() / (float)m->getHeight();
		ScopedTranslate t((ofGetWidth() - m->getWidth() * s) * 0.5f, 0.f);
		m->draw(ofVec3f::zero(), m->getWidth() * s, m->getHeight() * s);
	}

	{
		ofSetColor(ofColor::white, 255 * mAlphaVis);
		auto m = mMovies.at(MOVIE_VIS);
		const float s = ofGetHeight() / (float)m->getHeight();
		ScopedTranslate t((ofGetWidth() - m->getWidth() * s) * 0.5f, 0.f);
		m->draw(ofVec3f::zero(), m->getWidth() * s, m->getHeight() * s);
	}
}

void SceneHakoMovies::keyPressed(int key)
{
	switch (key) {
	case '1':
		mDisplayType = DISPLAY_NAMA;
		break;
	case '2':
		mDisplayType = DISPLAY_VIS;
		break;
	case '3':
		mDisplayType = DISPLAY_MIX;
		break;
	default:
		break;
	}
}

DP_SCORE_NAMESPACE_END