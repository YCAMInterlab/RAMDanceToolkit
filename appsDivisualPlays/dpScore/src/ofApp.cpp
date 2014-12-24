#include "ofApp.h"

static const int _circleNum = 2000;

static const int _clockNumX = 16;
static const int _clockNumY = 9;

static const int _scrollDiv = 10;

static const string _guiSettingsFile = "guiSettings.xml";

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    ofBackground(ofColor::black);
    mOscReceiver.setup(10000);
    
    mCircleBuffer.clear();
    mClockBuffer.clear();
    
    mStep = 4;
    mGridStep = 10;
    mMode = 0;
    mW = 1280;
    mH = 768;
    mSensorScale = 0.3f;
    
    mBuffer.assign(mW/mStep, ofVec2f::zero());
    
    mGridW = mW - 100.f;
    mGridH = mH;
    mGridBuffer.assign(mGridW/mGridStep, ofVec2f::zero());
    mGridVertices.assign(mGridW / mGridStep * mGridH /mGridStep, ofVec3f::zero());
    mGridVbo.setVertexData(&mGridVertices.at(0), mGridVertices.size(), GL_DYNAMIC_DRAW);
    
    mCircleVertices.assign(_circleNum, ofVec3f::zero());
    mCircleColors.assign(_circleNum, ofFloatColor(0.f, 0.f, 0.f, 0.f));
    mCircleVbo.setVertexData(&mCircleVertices.at(0), _circleNum, GL_DYNAMIC_DRAW);
    mCircleVbo.setColorData(&mCircleColors.at(0), _circleNum, GL_DYNAMIC_DRAW);
    
    mScrollW = mW;
    mScrollH = mH-20.f;
    mScrollBuffer.clear();
    mScrollBufferSize = mScrollW/_scrollDiv * mScrollH/_scrollDiv;
    
    mGui = new ofxUICanvas(20, 40, 300.f, 38.f);
    mGui->setColorBack(ofColor(255, 100));
    mGui->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
    mGui->loadSettings(_guiSettingsFile);
    mGui->disable();
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
        
        mScrollBuffer.push_back(vector<ofVec2f>());
        (mScrollBuffer.end()-1)->clear();
        if (m.getAddress() == "/dp/cameraUnit/pendulum/vector") {
            static bool skip = false;
            if (!skip) {
                ofVec2f _v;
                _v.x = m.getArgAsFloat(1);
                _v.y = m.getArgAsFloat(2);
                (mScrollBuffer.end()-1)->push_back(_v);
                v.x += m.getArgAsFloat(1);
                v.y += m.getArgAsFloat(2);
                n++;
            }
            skip ^= true;
        }
    }
    if (n>0) {
        v /= (float)n;
        mBuffer.push_back(v);
        mCircleBuffer.push_back(v);
        mGridBuffer.push_back(v);
        mClockBuffer.push_back(v);
        mVec = v;
        
        while (mBuffer.size()>mW/mStep-mStep*2) {
            mBuffer.pop_front();
        }
        
        while (mClockBuffer.size()>_clockNumX * _clockNumY) {
            mClockBuffer.pop_front();
        }
        
        while (mGridBuffer.size()>mW/mGridStep) {
            mGridBuffer.pop_front();
        }
        
        while (mCircleBuffer.size()>_circleNum) {
            mCircleBuffer.pop_front();
        }
        
        while (mScrollBuffer.size()>_scrollDiv) {
            mScrollBuffer.pop_front();
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
            
            //        case MODE_SCROLL:
            //            drawScroll();
            //            break;
            
        default:
            break;
    }
    
    if (mGui->isEnabled()) mGui->draw();
}

//--------------------------------------------------------------
void ofApp::draw2LinesGraph()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofPushMatrix();
    
    ofSetColor(ofColor::white, 255);
    
    ofDrawBitmapString("Dividual Plays System Score - A", 12.f, 16.f);
    
    ofPushMatrix();
    ofTranslate(0.f, align(20.f));
    ofLine(10.f, 0.f, mW-10.f, 0.f);
    const float step = mW / 210;
    for (int i=0; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(align(10.f+i*step), 0.f, align(10.f+i*step), height);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, align(mH-10.f));
    ofLine(10.f, 0.f, mW-10.f, 0.f);
    for (int i=0; i<=210; i++) {
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
    
    ofSetColor(ofColor::white, 150);
    ofTranslate(20.f, align(mH*0.5f+5.f));
    ofLine(0.f, 0.f, mW, 0.f);
    
    const float mult = mH / 40.f * mSensorScale;
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
    
    ofPushMatrix();
    ofTranslate(0.f, align(mH*0.5f+10.f));
    ofSetColor(255, 0, 0, 255);
    ofSetLineWidth(2.f);
    const ofVec2f v = mVec * mult;
    ofLine(ofGetWidth()-14.f, align(v.x), ofGetWidth(), align(v.x));
    ofLine(ofGetWidth()-14.f, align(v.y), ofGetWidth(), align(v.y));
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawClocks()
{
    ofSetCircleResolution(64);
    
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - B", 12.f, 16.f);
    
    ofNoFill();
    const float step = 10.f;
    ofSetColor(255, 50);
    
    for (int i=0; i<=mW/step; i++) {
        ofLine(align(i*step), 20.f, align(i*step), align(mH-30.f));
    }
    
    for (int j=2; j<mH/step-2; j++) {
        ofLine(0.f, align(j*step), align(mW), align(j*step));
    }
    
    const float circleStep = 80.f;
    ofSetColor(255, 255);
    for (int j=0; j<_clockNumY; j++) {
        ofPushMatrix();
        ofTranslate(0.f, 30.f + j * circleStep + 30.f, 0.f);
        
        for (int i=0; i<_clockNumX; i++) {
            ofPushMatrix();
            ofTranslate(30.f + i * circleStep + 10.f, 0.f, 0.f);
            
            ofSetLineWidth(1.f);
            ofSetColor(255, 150);
            ofCircle(0.f, 0.f, 30.f);
            
            const int idx = (_clockNumX * _clockNumY - 1) - (i + j * _clockNumX);
            
            (i == 0 && j == 0) ? ofSetColor(255, 0, 0, 255) : ofSetColor(255, 255);
            
            if (idx < mClockBuffer.size()) {
                ofVec2f v = mClockBuffer.at(idx);
                //v.normalize();
                const float mult = mH / 150.f * mSensorScale;
                
                ofSetLineWidth(1.5f);
                ofLine(ofVec2f::zero(), v * mult);
                //ofLine(ofVec2f::zero(), v * 30.f);
            }
            
            ofSetColor(255, 100);
            ofSetLineWidth(1.f);
            const float size = 5.f;
            ofLine(align(-size), 0.5f, align(size), 0.5f);
            ofLine(0.5f, align(-size), 0.5f, align(size));
            
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
    
    mCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()*0.5f, 150.f, 0.f);
    
    ofPushMatrix();
    ofTranslate(mGridW, 0.f);
    ofSetColor(255, 50);
    const float length = ofGetHeight()*2.f;
    ofLine(-length, 0.f, length, 0.f);
    ofLine(0.f, -length, 0.f, length);
    ofLine(0.f, 0.f, -length, 0.f, 0.f, length);
    
    const float size = 10.f;
    ofSetColor(ofColor::red, 100);
    ofLine(-size, 0.f, size, 0.f);
    ofSetColor(ofColor::green, 100);
    ofLine(0.f, -size, 0.f, size);
    ofSetColor(ofColor::blue, 100);
    ofLine(0.f, 0.f, -size, 0.f, 0.f, size);
    
    ofPopMatrix();
    
    ofSetColor(255, 255);
    ofNoFill();
    for (int j=0; j<mGridH/mGridStep; j++) {
        for (int i=0; i<mGridW/mGridStep; i++) {
            const int idx = j * mGridW/mGridStep + i;
            ofVec2f v = mGridBuffer.at(i);
            float f = ofMap(v.y, -25.f, 25.f, -1.f, 1.f);
            mGridVertices.at(idx).x = align(i*mGridStep);
            mGridVertices.at(idx).y = align(j*mGridStep) * f * mSensorScale;
            mGridVertices.at(idx).z = v.x * 15.f * mSensorScale;
        }
    }
    mGridVbo.updateVertexData(&mGridVertices.at(0), mGridVertices.size());
    
    mGridVbo.draw(GL_POINTS, 0, mGridVertices.size());
    ofPopMatrix();
    mCam.end();
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawCircle()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - D", 12.f, 16.f);
    
    ofSetLineWidth(1.f);
    ofPushMatrix();
    const float mult = mH / 40.f * mSensorScale;
    ofVec2f v = mVec * mult;
    
    glPointSize(2.f);
    ofSetColor(255, 200);
    ofNoFill();
    ofRect(5.f, 25.f, mW-10.f, mH-30.f);
    
    ofLine(v.x+mW*0.5f, 40.f, v.x+mW*0.5f, mH-20.f);
    ofLine(20.f, v.y+mH*0.5f, mW-20.f, v.y+mH*0.5f);
    
    ofSetLineWidth(2.f);
    ofSetColor(255, 0, 0, 255);
    //ofLine(v.x+mW*0.5f, 40.f, v.x+mW*0.5f, 50.f);
    //ofLine(v.x+mW*0.5f, mH-30.f, v.x+mW*0.5f, mH-20.f);
    //ofLine(20.f, v.y+mH*0.5f, 30.f, v.y+mH*0.5f);
    //ofLine(mW-30.f, v.y+mH*0.5f, mW-20.f, v.y+mH*0.5f);
    
    const float size = 10.f;
    ofLine(v.x+mW*0.5f, v.y+mH*0.5f-size, v.x+mW*0.5f, v.y+mH*0.5f+size);
    ofLine(v.x+mW*0.5f-size, v.y+mH*0.5f, v.x+mW*0.5f+size, v.y+mH*0.5f);
    
    ofSetColor(255, 200);
    ofSetLineWidth(1.f);
    const float step = 1910 / 300;
    for (int i=2; i<=210; i++) {
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
void ofApp::drawScroll()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - E", 12.f, 16.f);
    
    ofPushMatrix();
    ofTranslate(0.f, align(mH-mScrollH));
    
    const int w = mScrollW / _scrollDiv;
    const int h = mScrollH / _scrollDiv;
    
    for (int j=0; j<_scrollDiv; j++) {
        ofPushMatrix();
        ofTranslate(0.f, align(j*h));
        for (int i=0; i<_scrollDiv; i++) {
            ofPushMatrix();
            ofTranslate(align(i*w), 0.f);
            ofNoFill();
            ofSetColor(ofColor::white, 150);
            ofRect(0.f, 0.f, w, h);
            
            const int idx = (_scrollDiv * _scrollDiv -1) - j + i * _scrollDiv;
            
            if (mScrollBuffer.empty() == false && idx < mScrollBuffer.size()) {
                vector<ofVec2f>& vectors = mScrollBuffer.at(idx);
                glPointSize(2.f);
                glBegin(GL_POINTS);
                for (int k=0; k<vectors.size(); k++) {
                    ofVec2f v = vectors.at(k);
                    v.x = ofMap(v.x, -50.f, 50.f, 0.f, w);
                    v.y = ofMap(v.y, -50.f, 50.f, 0.f, h);
                    glVertex2f(v.x, v.y);
                }
                glEnd();
            }
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key=='f') {
        ofToggleFullscreen();
        ofShowCursor();
    }
    else if (key==' ') {
        ++mMode %= NUM_MODE;
    }
    else if (key=='g') {
        mGui->isEnabled() ? mGui->disable() : mGui->enable();
    }
    else if (key=='s') {
        mGui->saveSettings(_guiSettingsFile);
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


