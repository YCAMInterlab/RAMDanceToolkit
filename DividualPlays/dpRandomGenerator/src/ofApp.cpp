#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    camWidth = 128;
    camHeight = 96;
    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for (int i = 0; i < devices.size(); i++) {
        cout << devices[i].id << ": " << devices[i].deviceName;
        if(devices[i].bAvailable) {
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    randomGenerator.setup(camWidth, camHeight);
    
    graph.clear();
    
    min = 1.0;
    max = -1.0f;
    
    headX = camWidth*0.5f;
    headY = camHeight*0.5f;
    
    soundStream.setup(this, 2, 0, 44100, 512, 4);
    audioBuffer.assign(512, 0.0f);
}

//--------------------------------------------------------------
void ofApp::update()
{
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew() && ofGetFrameNum() > 120) {
        randomGenerator.update(vidGrabber.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (ofGetFrameNum() < 120) return;
    
    ofSetColor(ofColor::white);
    const int w = vidGrabber.getWidth();
    const int h = vidGrabber.getHeight();
    const int stepX = ofGetWidth() / w;
    const int stepY = ofGetHeight() / h;
    const unsigned char* pixels = vidGrabber.getPixels();
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            const unsigned char r  = pixels[i * 3 + j * w * 3 + 0];
            const unsigned char g  = pixels[i * 3 + j * w * 3 + 1];
            const unsigned char b  = pixels[i * 3 + j * w * 3 + 2];
            ofSetColor(r, g, b);
            ofRect(i * stepX, j * stepY, stepX, stepY);
        }
    }
    
    const int nSamplePerFrame = 1;
    vector<float> tmpBuffer(nSamplePerFrame);
    for (int _n=0; _n<nSamplePerFrame; _n++) {
        
        const int x = headX;
        const int y = headY;
        const int n = 4;
        
        //headX++;
        //if (headX>=camWidth-4) headX = 0;
        //if (headY>=camHeight-4) headY = 0;
        
        unsigned long long r0 = 0;
        unsigned long long g0 = 0;
        unsigned long long b0 = 0;
        unsigned long long r1 = 0;
        unsigned long long g1 = 0;
        unsigned long long b1 = 0;
        for (int j=0; j<n; j++) {
            for (int i=0; i<n; i++) {
                r0  += pixels[(x+i) * 3 + (y+j) * w * 3 + 0];
                g0  += pixels[(x+i) * 3 + (y+j) * w * 3 + 1];
                b0  += pixels[(x+i) * 3 + (y+j) * w * 3 + 2];
                r1  += pixels[(x+i) * 3 + (y+j) * w * 3 + 0];
                g1  += pixels[(x+i+1) * 3 + (y+j+1) * w * 3 + 1];
                b1  += pixels[(x+i+1) * 3 + (y+j+1) * w * 3 + 2];
            }
        }
        r0 /= n;
        g0 /= n;
        b0 /= n;
        r1 /= n;
        g1 /= n;
        b1 /= n;
        
        unsigned long long _r0 = r0 << 0;
        unsigned long long _g0 = g0 << 8;
        unsigned long long _b0 = b0 << 16;
        unsigned long long _r1 = r1 << 24;
        unsigned long long _g1 = g1 << 32;
        unsigned long long _b1 = b1 << 40;
        
        const unsigned long long l = _r0 + _g0 + _b0 + _r1 + _g1 + b1;
        unsigned long long _max = 0;
        for (int i=0; i<6; i++) {
            unsigned long long llu = 0xFF;
            _max += (llu << (8 * i));
        }
        float f = l / (float)_max;
        tmpBuffer.at(_n) = f;
        f = f * 2.0f - 1.0f;
        
        graph.push_back(f);
        
        if (f < min) {
            min = f;
        }
        else if (f > max) {
            max = f;
        }
        
        while (graph.size()>ofGetWidth()) {
            graph.pop_front();
        }
    }
    
    const float step = 512.f / tmpBuffer.size();
    for (int i=0; i<tmpBuffer.size()-1; i++) {
        for (int j=0; j<step; j++) {
            float f = j / (float)step;
            audioBuffer.at(i*step+j) = tmpBuffer.at(i) * (1.0f - f) + tmpBuffer.at(i) * f;
        }
    }
    
    ofSetColor(ofColor::red);
    ofPushMatrix();
    ofTranslate(0.f, ofGetHeight()*0.5f);
    if (graph.empty() == false) {
        for (int i=0; i<graph.size()-1; i++) {
            float f0 = ofMap(graph.at(i+0), min, max, -1.f, 1.f);
            float f1 = ofMap(graph.at(i+1), min, max, -1.f, 1.f);
            const float h0 = ofGetHeight() * 0.5f * f0;
            const float h1 = ofGetHeight() * 0.5f * f1;
            ofLine(i+0, h0, i+1, h1);
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
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

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++) {
        output[i*nChannels    ] = audioBuffer.at(i);
        output[i*nChannels + 1] = audioBuffer.at(i);
    }
}
