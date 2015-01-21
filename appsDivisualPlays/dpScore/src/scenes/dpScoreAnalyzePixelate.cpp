//
//  dpScoreAnalyzePixelate.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/15.
//
//

#include "dpScoreAnalyzePixelate.h"

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

void AnalyzePixelate::draw()
{
OFX_BEGIN_EXCEPTION_HANDLING
    ofPushStyle();
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