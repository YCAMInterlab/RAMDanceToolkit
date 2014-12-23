#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    ofBackground(ofColor::black);
    mOscReceiver.setup(10000);
    
    mBuffer.clear();
    
    mStep = 4;
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
        
        while (mBuffer.size()>ofGetWidth()/mStep) {
            mBuffer.pop_front();
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
    ofEnableAlphaBlending();
    const float mgn = 60.f;
    ofPushMatrix();
    ofTranslate(0.f, ofGetHeight()*0.5f);
    ofSetColor(ofColor::white, 200);
    const float mult = ofGetHeight() / 160.f;
    if (mBuffer.size()>=2) {
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
void ofApp::keyPressed(int key)
{
    if (key=='f') {
        ofToggleFullscreen();
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


