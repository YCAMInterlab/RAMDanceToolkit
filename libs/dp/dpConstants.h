//
//  dpConstants.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/20.
//
//

#ifndef RAMDanceToolkit_dpConstants_h
#define RAMDanceToolkit_dpConstants_h

static const int SINGLE_SCREEN_WIDTH = 1920;
static const int SINGLE_SCREEN_HEIGHT = 1080;
static const int APP_WIDTH = SINGLE_SCREEN_WIDTH * 2;
static const int APP_HEIGHT = SINGLE_SCREEN_HEIGHT;

static ofRectangle dpGetFirstScreenViewPort(){
    return ofRectangle(0,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
}

static ofRectangle dpGetSecondScreenViewPort(){
    return ofRectangle(SINGLE_SCREEN_WIDTH,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
}

static ofPoint dpGetFisrtScreenCenter(){
    return ofPoint(SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5,0.0);
}

static ofPoint dpGetSecondScreenCenter(){
    return ofPoint(SINGLE_SCREEN_WIDTH * 1.5,SINGLE_SCREEN_HEIGHT * 0.5,0.0);
}

#endif
