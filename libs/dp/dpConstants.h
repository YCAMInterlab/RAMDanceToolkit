//
//  dpConstants.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/20.
//
//

#ifndef RAMDanceToolkit_dpConstants_h
#define RAMDanceToolkit_dpConstants_h

//#define DEBUG_MACBOOK_AIR

#if defined(DEBUG_MACBOOK_AIR)
static const int SINGLE_SCREEN_WIDTH = 1366/2;
static const int SINGLE_SCREEN_HEIGHT = 768;
#elif defined(DEBUG_MACBOOK_PRO)
static const int SINGLE_SCREEN_WIDTH = 1920/2;
static const int SINGLE_SCREEN_HEIGHT = 1080;
#else
static const int SINGLE_SCREEN_WIDTH = 1920;
static const int SINGLE_SCREEN_HEIGHT = 1080;
#endif

static const int APP_WIDTH = SINGLE_SCREEN_WIDTH * 2;
static const int APP_HEIGHT = SINGLE_SCREEN_HEIGHT;

static const int NUM_SCREEN_POSITIONS = 2;
static const ofVec2f SCREEN_POSITIONS[NUM_SCREEN_POSITIONS] = {
    ofVec2f(0.f, 0.f),
    ofVec2f(SINGLE_SCREEN_WIDTH, 0.f),
};

static ofRectangle dpGetFirstScreenViewPort(){
    return ofRectangle(0,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
}

static ofRectangle dpGetSecondScreenViewPort(){
    return ofRectangle(SINGLE_SCREEN_WIDTH,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
}

static ofPoint dpGetFirstScreenCenter(){
    return ofPoint(SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5,0.0);
}

static ofPoint dpGetSecondScreenCenter(){
    return ofPoint(SINGLE_SCREEN_WIDTH * 1.5,SINGLE_SCREEN_HEIGHT * 0.5,0.0);
}

struct dpColor
{
    static const ofColor MAIN_COLOR;
    static const ofColor PALE_PINK_LIGHT;
    static const ofColor PALE_PINK_HEAVY;
    static const ofColor DARK_PINK_LIGHT;
    static const ofColor DARK_PINK_HEAVY;
};

#endif
