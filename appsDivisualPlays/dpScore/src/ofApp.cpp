#include "ofApp.h"

static const int _circleNum = 2000;

static const int _clockNumX = 24;
static const int _clockNumY = 14;

static const int _gridStep = 10;

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    ofBackground(ofColor::black);
    mOscReceiver.setup(10000);
    
    mBuffer.clear();
    mCircleBuffer.clear();
    mClockBuffer.clear();
    
    mStep = 4;
    mMode = 0;
    
    mW = 1920;
    mH = 1080;
    mGridVertices.assign(mW * mH / _gridStep, ofVec3f::zero());
    mGridVbo.setVertexData(&mGridVertices.at(0), mGridVertices.size(), GL_DYNAMIC_DRAW);
    
    mCircleVertices.assign(_circleNum, ofVec3f::zero());
    mCircleColors.assign(_circleNum, ofFloatColor(0.f, 0.f, 0.f, 0.f));
    mCircleVbo.setVertexData(&mCircleVertices.at(0), _circleNum, GL_DYNAMIC_DRAW);
    mCircleVbo.setColorData(&mCircleColors.at(0), _circleNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    int n = 0;
    ofVec2f v;
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiver.getNextMessage(&m);
        if (m.getAddress() == "/dp/cameraUnit/pendulum/vector") {
            v.x += m.getArgAsFloat(1);
            v.y += m.getArgAsFloat(2);
            n++;
        }
    }
    if (n>0) {
        v /= (float)n;
        mBuffer.push_back(v);
        mCircleBuffer.push_back(v);
        mClockBuffer.push_back(v);
        mVec = v;
        
        while (mBuffer.size()>mW/mStep) {
            mBuffer.pop_front();
        }
        
        while (mCircleBuffer.size()>_circleNum) {
            mCircleBuffer.pop_front();
        }
        
        while (mClockBuffer.size()>_clockNumX * _clockNumY) {
            mClockBuffer.pop_front();
        }
    }
}

//--------------------------------------------------------------
static float align(float f)
{
    return ::floor(f) + 0.5f;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    switch (mMode) {
        case MODE_TWO_LINES:
            draw2LinesGraph();
            break;
            
        case MODE_CLOCKS:
            drawClocks();
            break;
            
        case MODE_GRID:
            drawGrid();
            break;
            
        case MODE_CIRCLE:
            drawCircle();
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw2LinesGraph()
{
    ofEnableAlphaBlending();
    ofPushMatrix();
    
    ofSetColor(ofColor::white, 255);
    
    ofDrawBitmapString("Dividual Plays System Score - A", 12.f, 16.f);
    
    ofPushMatrix();
    ofTranslate(0.f, align(20.f));
    ofLine(10.f, 0.f, mW-10.f, 0.f);
    const float step = 1900 / 300;
    for (int i=0; i<=300; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(align(10.f+i*step), 0.f, align(10.f+i*step), height);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, align(mH-10.f));
    ofLine(10.f, 0.f, mW-10.f, 0.f);
    for (int i=0; i<=300; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(align(10.f+i*step), 0.f, align(10.f+i*step), -height);
    }
    ofPopMatrix();
    
    const float stepY = (mH - (10.f + 20.f + 40.f)) / 100.f;
    for (int i=0; i<=100; i++) {
        int width = 5.f;
        if (i%10 == 0) width = 8;
        ofLine(0.f, align(40.f+i*stepY), width, align(40.f+i*stepY));
    }
    
    ofSetColor(ofColor::white, 200);
    ofTranslate(20.f, align(mH*0.5f+10.f));
    ofLine(0.f, 0.f, mW, 0.f);
    
    const float mult = mH / 40.f;
    if (mBuffer.size()>=2) {
        
        ofSetColor(ofColor::white, 255);
        for (int i=0; i<mBuffer.size()-1; i++) {
            ofVec2f v0 = mBuffer.at(i);
            ofVec2f v1 = mBuffer.at(i+1);
            v0 *= mult;
            v1 *= mult;
            ofLine(align(i*mStep), align(v0.x),
                   align((i+1)*mStep), align(v1.x));
            ofLine(align(i*mStep), align(v0.y),
                   align((i+1)*mStep), align(v1.y));
        }
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawClocks()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - B", 12.f, 16.f);
    
    ofSetColor(255, 100);
    ofNoFill();
    const float step = 10.f;
    
    for (int i=0; i<mW/step; i++) {
        ofLine(align(i*step), 20.f, align(i*step), align(mH));
    }
    
    for (int j=2; j<mH/step; j++) {
        ofLine(0.f, align(j*step), align(mW), align(j*step));
    }
    
    ofSetColor(255, 255);
    for (int j=0; j<_clockNumY; j++) {
        ofPushMatrix();
        ofTranslate(0.f, 30.f + j * mH/_clockNumY + 20.f, 0.f);
        
        for (int i=0; i<_clockNumX; i++) {
            ofPushMatrix();
            ofTranslate(i * mW/_clockNumX + 30.f, 0.f, 0.f);
            
            ofSetLineWidth(1.f);
            ofSetColor(255, 200);
            ofCircle(0.f, 0.f, 20.f);
            
            const int idx = i + j * _clockNumX;
            
            if (idx < mClockBuffer.size()) {
                ofVec2f v = mClockBuffer.at(idx);
                const float mult = mH / 200.f;
                
                ofSetLineWidth(1.5f);
                ofSetColor(255, 255);
                ofLine(ofVec2f::zero(), v * mult);
            }
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGrid()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - C", 12.f, 16.f);
    
    ofSetColor(255, 200);
    ofNoFill();
    for (int j=0; j<mH/_gridStep; j++) {
        for (int i=0; i<mW/_gridStep; i++) {
            mGridVertices.at(i + j * mW / _gridStep).set(align(i*_gridStep), align(j*_gridStep));
        }
    }
    mGridVbo.updateVertexData(&mGridVertices.at(0), mGridVertices.size());
    
    mGridVbo.draw(GL_POINTS, 0, mGridVertices.size());
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawCircle()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - D", 12.f, 16.f);
    
    ofPushMatrix();
    const float mult = mH / 20.f;
    ofVec2f v = mVec * mult;
    
    glPointSize(2.f);
    ofSetColor(255, 200);
    ofNoFill();
    ofRect(5.f, 25.f, mW-10.f, mH-30.f);
    
    ofLine(v.x+mW*0.5f, 40.f, v.x+mW*0.5f, mH-20.f);
    ofLine(20.f, v.y+mH*0.5f, mW-20.f, v.y+mH*0.5f);
    
    const float step = 1910 / 300;
    for (int i=2; i<=298; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(align(5.f+i*step), 25.f, align(5.f+i*step), 25.f+height);
        ofLine(align(5.f+i*step), mH - 5.f, align(5.f+i*step), mH - 5.f - height);
    }
    
    
    const float stepY = (mH - 30.f) / 100.f;
    for (int i=2; i<=98; i++) {
        int width = 5.f;
        if (i%10 == 0) width = 8;
        ofLine(5.f, align(25.f+i*stepY), 5.f+width, align(25.f+i*stepY));
        ofLine(mW - 5.f, align(25.f+i*stepY), mW - 5.f - width, align(25.f+i*stepY));
    }
    
    
    ofSetColor(255, 255);
    ofTranslate(mW * 0.5f, mH * 0.5f);
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        const float a = i / (float)mCircleBuffer.size();
        //const float a = 1.f;
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a);
    }
    
    mCircleVbo.updateVertexData(&mCircleVertices.at(0), mCircleVertices.size());
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_POINTS, 0, mCircleVertices.size());
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        const float a = i / (float)mCircleBuffer.size();
        //const float a = 0.5f;
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a * 0.5f);
    }
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_LINE_STRIP, 0, mCircleVertices.size());
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key=='f') {
        ofToggleFullscreen();
    }
    else if (key==' ') {
        ++mMode %= NUM_MODE;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}


