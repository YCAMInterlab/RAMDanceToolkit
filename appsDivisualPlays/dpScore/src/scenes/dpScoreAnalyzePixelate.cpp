//
//  dpScoreAnalyzePixelate.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/15.
//
//

#include "dpScoreAnalyzePixelate.h"
#include "dpScoreMasterHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

AnalyzePixelate::AnalyzePixelate()
{
    mPixels.assign((int)Color::Num, vector<bool>());
    mPixelsInited.assign((int)Color::Num, false);
}

void AnalyzePixelate::updateColor(Color color, ofxEventMessage& m)
{
    vector<bool>* pixels{nullptr};
    switch (color) {
        case Color::R:
            pixels = &mPixels.at((int)Color::R);
            mPixelsInited.at((int)Color::R) = true;
            break;
        case Color::G:
            pixels = &mPixels.at((int)Color::G);
            mPixelsInited.at((int)Color::G) = true;
            break;
        case Color::B:
            pixels = &mPixels.at((int)Color::B);
            mPixelsInited.at((int)Color::B) = true;
            break;
        default:
            break;
    }
    if (!pixels) {
        ofxThrowException(ofxException, "incorrect color type");
    }
    
    mPixelW = m.getArgAsInt32(0);
    mPixelH = m.getArgAsInt32(1);
    
    pixels->clear();
    pixels->assign(m.getArgAsInt32(0) * m.getArgAsInt32(1), false);
    
    for (int64_t i=0; i<pixels->size(); i++) {
        int64_t bt = m.getArgAsInt64(2 + i / 64);
        int64_t targetBit = int64_t(1) << (i % 64);
        (*pixels)[i] = (bt & targetBit) > 0;
    }
}

void AnalyzePixelate::update()
{
    if (mDoSomething) {
        getMH().doSomething(0);
        mDoSomething = false;
    }
    
    const float t{ofGetElapsedTimef()};
    
    const int prevR{mNumR};
    const int prevG{mNumG};
    const int prevB{mNumB};
    
    mNumR = mNumG = mNumB = 0;
    
    for (int j=0; j<mPixelH; j++) {
        for (int i=0; i<mPixelW; i++) {
            const int idx{i + j * mPixelW};
            const bool r{mPixels.at((int)Color::R).at(idx)};
            const bool g{mPixels.at((int)Color::G).at(idx)};
            const bool b{mPixels.at((int)Color::B).at(idx)};
            if (r) mNumR++;
            if (g) mNumG++;
            if (b) mNumB++;
        }
    }
    
    mDiffR.f += ::abs(mNumR - prevR) * 0.01f;
    mDiffG.f += ::abs(mNumG - prevG) * 0.01f;
    mDiffB.f += ::abs(mNumB - prevB) * 0.01f;
    
    mTotalDiff.f = mDiffR.f + mDiffG.f + mDiffB.f;
    
    if (t < mPrevSetSceneTime + mMinSetSceneTime) return;
    
    if (mMaster) {
        if (mTotalDiff.f >= mLimit) {
            mWin0  = (mTotalDiff.i & 0b00000001) >> 0;
            mWin1  = (mTotalDiff.i & 0b00000010) >> 1;
            
            const int r  = (mDiffR.i & 0b00111100) >> 2;
            const int g  = (mDiffG.i & 0b00111100) >> 2;
            const int b  = (mDiffB.i & 0b00111100) >> 2;
            mWhich = r + g + b;
            if (getMH().getUniqueScenes().empty() == false) {
                const int scene{(int)(mWhich % getMH().getUniqueScenes().size())};
                const int score{(int)(mWhich % getMH().getNumUniqueScores())};
                getMH().setUniqueScene(scene, mWin0, mWin1);
                getMH().setUniqueScore(score);
                mPrevSetSceneTime = t;
            }
            mDiffR.f = mDiffG.f = mDiffB.f = 0.f;
        }
    }
    else {
        if (mTotalDiff.f >= mDoSomethingLimit) {
            mDoSomething = true;
            mDiffR.f = mDiffG.f = mDiffB.f = 0.f;
        }
    }
}

void AnalyzePixelate::draw()
{
OFX_BEGIN_EXCEPTION_HANDLING    
    const float t{ofGetElapsedTimef()};
    
    ofPushStyle();
    ofPushMatrix();
    alignedTranslate(0.f, MH::kTextSpacing);
    checkData();
    const int step{10};
    const int spacing{2};
    ofFill();
    for (int j=0; j<mPixelH; j++) {
        for (int i=0; i<mPixelW; i++) {
            const int idx{i + j * mPixelW};
            const bool r{mPixels.at((int)Color::R).at(idx)};
            const bool g{mPixels.at((int)Color::G).at(idx)};
            const bool b{mPixels.at((int)Color::B).at(idx)};
            if (r | g | b)
                ofSetColor(r * 125 + 150,
                           g * 125 + 150,
                           b * 125 + 150,
                           100);
            else
                ofSetColor(ofColor::white, 20);
            
            alignedRect(i * step + i * spacing, j * step + j * spacing, step, step);
        }
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(MH::kTextColor);
    ofDrawBitmapString("[pixelate]", ofPoint::zero());
    alignedTranslate(200.f, MH::kTextSpacing);
    alignedTranslate(0.f, MH::kTextSpacing);
    stringstream ss;
    ss << fixed << setprecision(1)
    << "master    : " << mMaster << endl
    << "scene span: " << t - mPrevSetSceneTime << endl
    << "total diff: " << mTotalDiff.f << endl
    << "RGB       : " << mNumR << ", " << mNumG << ", " << mNumB << endl
    << "diff      : " << mDiffR.f << ", " << mDiffG.f << ", " << mDiffB.f << endl
    << "gen       : " << mWhich << ", " << mWin0 << ", " << mWin1;
    ofDrawBitmapString(ss.str(), ofPoint::zero());
    alignedTranslate(0.f, MH::kTextSpacing * 7.f);
    mDoSomething ? ofSetColor(color::kMain) : ofSetColor(MH::kTextColor);
    ss.str("");
    ss << boolalpha
    << "do something: " << mTotalDiff.f << " / " << mDoSomethingLimit;
    ofDrawBitmapString(ss.str(), ofPoint::zero());
    ofPopMatrix();
    
    ofPopStyle();
    
OFX_END_EXCEPTION_HANDLING
}

void AnalyzePixelate::checkData()
{
    for (auto b : mPixelsInited) {
        if (!b) return;
    }
    
    if (mPixels.empty())
        ofxThrowException(ofxException, "empty pixels");
    if (!mPixelW || !mPixelH)
        ofxThrowExceptionf(ofxException, "incorrect pixel dimension %d %d",
                           mPixelW,
                           mPixelH);
    if (mPixels.at((int)Color::R).size() !=  mPixels.at((int)Color::B).size())
        ofxThrowExceptionf(ofxException, "pixel size dosen't same %d %d",
                           mPixels.at((int)Color::R).size(),
                           mPixels.at((int)Color::B).size());
    
    if (mPixels.at((int)Color::R).size() !=  mPixels.at((int)Color::G).size())
        ofxThrowExceptionf(ofxException, "pixel size dosen't same %d %d",
                           mPixels.at((int)Color::R).size(),
                           mPixels.at((int)Color::G).size());
}

DP_SCORE_NAMESPACE_END