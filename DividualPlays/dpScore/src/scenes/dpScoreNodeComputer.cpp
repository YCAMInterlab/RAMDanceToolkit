//
//  dpScoreNodeComputer.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeComputer.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeComputer::NodeComputer()
{
	title = "RAM Dance Toolkit";
	titleJP = "RAM Dance Toolkit";
	descriptionJP = "「RAM Dance Toolkit」\nRAMシステムはダンサーの動きにあわせて\n仮想の環境を変化させます";
	descriptionJPFromHakoniwa = "「RAM Dance Toolkit」\n再度、箱庭内部の状態にあわせて\n仮想の環境を変化させます";

	const float diff {NodeStage::kDepth - Deck::getDepth() * kNumDecks};
	setGlobalPosition(-NodeStage::kWidth * 0.5f - Deck::getWidth() - 320.f,
	                  0.f,
	                  -NodeStage::kDepth * 0.5f + diff - 300.f);
	addAimingOffset(NodeComputer::getMacBookPosition(3) + ofVec3f(MacBook::getDepth() * 0.5f,
	                                                              MacBook::getDepth() * 0.5f,
	                                                              -MacBook::getWidth() * 0.5f));

	getCamera().setFov(60.f);
	auto p = getGlobalPosition();
	p.x -= Deck::getWidth() + 50.f;
	p.y += 180.f;
	p.z += Deck::getDepth() * kNumDecks * 0.5f;
	getCamera().setPosition(p);
	getCamera().setOrientation(ofVec3f(-20.f, -90.f, 0.f));

	mFbo.allocate(MacBook::getWidth() * 4.f, MacBook::getDepth() * 4.f);

	mDecks.clear();
	for (auto i : rep(kNumDecks)) {
		mDecks.push_back(Deck());
	}
	mMBPs.clear();
	for (auto i : rep(kNumMacBooks)) {
		mMBPs.push_back(MacBook());
	}
}

NodeComputer::~NodeComputer()
{

}

void NodeComputer::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	for (auto i : rep(kNumDecks)) {
		ScopedTranslate t(0.f, 0.f, Deck::getDepth() * i);
		mDecks.at(i).draw();
	}

	MacBook::DrawDisplayFunc drawDisplay[kNumMacBooks];
	memset(drawDisplay, 0, kNumMacBooks * sizeof(MacBook::DrawDisplayFunc));

	for (auto i : rep(kNumMacBooks)) {
		drawDisplay[i] = [](float w, float h) {
					 w -= 2.f;
					 h -= 2.f;
					 ScopedTranslate t(1.f, 1.f);
					 ScopedStyle style;
					 ofFill();
					 //ofNoFill();
					 ofSetColor(color::kMain);
					 const int n {10};
					 const float g {1.f};
					 const float s {w / (float)n - g};
					 for (auto j : rep(n)) {
						 const float t {getElapsedTime() * 1.f};
						 auto y = ofNoise(t, j) * h;
						 ofRect(j * (s + g), h, s, -y);
					 }
				 };
	}

	drawDisplay[1] = [](float w, float h) {
				 w -= 2.f;
				 h -= 2.f;
				 ScopedTranslate trans(1.f, 1.f);
				 ScopedStyle s;
				 ofSetColor(color::kMain);
				 const float t {getElapsedTime() * 1.5f};
				 auto x = ofSignedNoise(t, 12.95f) * w * 0.5f;
				 auto y = ofSignedNoise(t, 29.21f) * h * 0.5f;
				 ofLine(0.f, h * 0.5f + y, w, h * 0.5f + y);
				 ofLine(w * 0.5f + x, 0.f, w * 0.5f + x, h);
				 ofLine(0.f, h * 0.5f, w, h * 0.5f);
				 ofLine(w * 0.5f, 0.f, w * 0.5f, h);
			 };

	drawDisplay[2] = [](float w, float h) {
				 w -= 2.f;
				 h -= 2.f;
				 ScopedTranslate t(1.f, 1.f);
				 ScopedStyle style;
				 ofFill();
				 //ofNoFill();
				 ofSetColor(color::kMain);
				 const int n {10};
				 const float g {1.f};
				 const float s {h / (float)n - g};
				 for (auto j : rep(n)) {
					 const float t {getElapsedTime() * 1.f};
					 auto x = ofNoise(t, j) * w;
					 ofRect(w, j * (s + g), -x, s);
				 }
			 };

	drawDisplay[3] = [](float w, float h) {
				 w -= 2.f;
				 h -= 2.f;
				 ScopedTranslate trans(1.f, 1.f);
				 ScopedStyle s;
				 ofSetColor(color::kMain);
				 const float t {getElapsedTime() * 30.f};
				 ofPolyline p;
				 for (int i : rep((int)w)) {
					 p.addVertex((float)i, ofSignedNoise(i * 0.5f - t) * h * 0.5f + h * 0.5f, 0.f);
				 }
				 p.draw();
			 };
	/*
	   drawDisplay[3] = [](float w, float h) {
	    ScopedStyle s;
	    ofSetColor(color::kMain);
	    ofTranslate(w * 0.5f, h * 0.5f);
	    const float t {getElapsedTime() * 10.f};
	    ofPolyline p;
	    const int res{100};
	    const float step {360.f / (float)res};
	    for (auto i : rep(res)) {
	        const float deg {i * step};
	        const float r {ofSignedNoise(i * 0.2f - t) * h * 0.25f + h * 0.25f};
	        p.addVertex(::cosf(ofDegToRad(deg)) * r,
	                    ::sinf(ofDegToRad(deg)) * r,
	                    0.f);
	    }
	    p.draw();
	                     };
	 */
	drawDisplay[6] = drawDisplay[1];
	drawDisplay[5] = drawDisplay[2];
	drawDisplay[4] = drawDisplay[3];

	const float step {(Deck::getDepth() * (float)kNumDecks) / (float)kNumMacBooks};
	for (auto i : rep(kNumMacBooks)) {
		ScopedTranslate trans(Deck::getWidth() * 0.2f, Deck::getHeight(), step * 0.5f + i * step + MacBook::getWidth() * 0.5f);
		ScopedRotateY ry(90.f);
		mMBPs.at(i).angle = t * kMaxMacBookAngle;
		mMBPs.at(i).drawDisplay = drawDisplay[i];
		mMBPs.at(i).draw();
	}
}

ofVec3f NodeComputer::getMacBookPosition(int i)
{
	const float step {(Deck::getDepth() * (float)kNumDecks) / (float)kNumMacBooks};
	return ofVec3f(Deck::getWidth() * 0.2f, Deck::getHeight(), step * 0.5f + i * step + MacBook::getWidth() * 0.5f);
}

DP_SCORE_NAMESPACE_END