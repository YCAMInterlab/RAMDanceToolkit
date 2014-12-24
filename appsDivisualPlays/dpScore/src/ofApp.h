#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void guiEvent(ofxUIEventArgs &e);
    
private:
    void draw2LinesGraph();
    void drawClocks();
    void drawGrid();
    void drawCircle();
    void drawScroll();
    
    enum Mode {
        MODE_TWO_LINES,
        MODE_CLOCKS,
        MODE_GRID,
        MODE_CIRCLE,
        //MODE_SCROLL,
        NUM_MODE,
    };
    
    ofxUICanvas *mGui;
    ofEasyCam mCam;
    
    float mSensorScale;
    int mMode;
    int mW;
    int mH;
    
    ofxOscReceiver mOscReceiver;
    deque<ofVec2f> mBuffer;
    ofVec2f mVec;
    int mStep;
    
    deque<ofVec2f> mClockBuffer;
    
    vector<ofVec3f> mGridVertices;
    ofVbo mGridVbo;
    deque<ofVec2f> mGridBuffer;
    int mGridStep;
    int mGridW;
    int mGridH;
    
    deque<ofVec2f> mCircleBuffer;
    vector<ofVec3f> mCircleVertices;
    vector<ofFloatColor> mCircleColors;
    ofVbo mCircleVbo;
    
    deque<vector<ofVec2f> > mScrollBuffer;
    float mScrollW;
    float mScrollH;
    int mScrollBufferSize;
    
};
