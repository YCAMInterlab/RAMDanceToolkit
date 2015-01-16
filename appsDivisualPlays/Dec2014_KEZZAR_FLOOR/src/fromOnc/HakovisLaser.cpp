//
//  HakovisLaser.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 12/26/14.
//
//

#include "HakovisLaser.h"
#include "Easing.h"

static float _height = 20.f;

HakovisLaser::Blob::Blob(int index, int nVerts) :
idx(index)
{
    
}

void HakovisLaser::Blob::update()
{
    for (int i=0; i<verts.size(); i++) {
        center += verts.at(i);
    }
    center /= verts.size();
    
    for (int i=0; i<verts.size(); i++) {
        verts.at(i) -= center;
        const ofVec2f& v = verts.at(i);
        if (i==0){
            min = max = v;
        }
        else {
            if (v.x < min.x) min.x = v.x;
            if (v.y < min.y) min.y = v.y;
            if (v.x > max.x) max.x = v.x;
            if (v.y > max.y) max.y = v.y;
        }
    }
}

void HakovisLaser::Blob::draw(float a)
{
    int n = 10;
    float base = 2.f;
    
    a = ::easeInSine(a);
    
    float square = (max.x - min.x) * (max.y - min.y);
    square = ofMap(square, 0, 50000, 0.2f, 1.f);
    
    ofPushMatrix();
    ofTranslate(center.x, 0.f, center.y);
    
    glLineWidth(2.f);
    for (int i=0; i<n; i++) {
        ofPushMatrix();
        float _f = (i+1)/(float)n;
        ofColor c;
        c.setHsb(_f*255.f, (a*0.7f)*255.f, 255.f);
        ofSetColor(c, 2555);
        ofTranslate(0.f, (n*_height - (i+1)*_height) * square, 0.f);
        ofScale(i/(float)(n+1)*base, 1.f, i/(float)(n+1)*base);
        glBegin(GL_LINE_STRIP);
        for (int i=0; i<verts.size(); i++) {
            glVertex3f(verts.at(i).x, 0.f, verts.at(i).y);
        }
        glEnd();
        ofPopMatrix();
    }
    ofPopMatrix();
}

HakovisLaser::HakovisLaser() :
mOscReceiverTag(NULL)
{
    mOscReceiverTag = new ramOscReceiveTag;
    ramOscManager::instance().addReceiverTag(mOscReceiverTag);
    mOscReceiverTag->addAddress("/dp/cameraUnit/stage/contour/blob");
    mBlobScale = 4.f;
    mX = mY = 0.f;
    mBlobs.clear();
}

HakovisLaser::~HakovisLaser()
{
    if (mOscReceiverTag) {
        delete mOscReceiverTag;
        mOscReceiverTag = NULL;
    }
}

void HakovisLaser::setupControlPanel()
{
    ramGetGUI().addLabel("HakovisLaser");
    ramGetGUI().addSlider("Blob Scale", 0.f, 1600.f, &mBlobScale);
    ramGetGUI().addSlider("X", 0.f, 500.f, &mX);
    ramGetGUI().addSlider("Y", 0.f, 500.f, &mY);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent,
                  this,
                  &HakovisLaser::onPanelChanged);
}

void HakovisLaser::update()
{
    vector<ofPtr<Blob> > blobs;
    blobs.clear();
    while (mOscReceiverTag->hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiverTag->getNextMessage(&m);
        //cout << m.getAddress() << endl;
        //for (int i=0; i<m.getNumArgs(); i++) {
        //    cout << m.getArgTypeName(i) << " ";
        //}
        //cout << endl;
        
        if (m.getAddress() == "/dp/cameraUnit/stage/contour/blob") {
            
            int numBlobs = m.getArgAsInt32(0);
            
            int sumArg= 1;
            
            int blobIdx = 0;
            
            for(int i = 0; i < numBlobs; i++){
                
                int nVerts = m.getArgAsInt32(sumArg);
                
                ofPtr<Blob> blob = ofPtr<Blob>(new Blob(blobIdx, nVerts));
                
                for(int j = sumArg + 1; j < sumArg + nVerts; j+=2){
                    float x = m.getArgAsFloat(j);
                    float y = m.getArgAsFloat(j + 1);
                    
                    ofPoint v;
                    v.x = x;
                    v.y = y;
                    blob->verts.push_back(v * mBlobScale);
                }
                
                blobIdx++;
                
                blob->update();
                blobs.push_back(blob);
                
                sumArg += nVerts * 2 + 1;
            }
        }
    }
    
    if (blobs.empty() == false)
        mBlobs.push_back(blobs);
    
    while (mBlobs.size() > 60) {
        mBlobs.pop_front();
    }
}

void HakovisLaser::draw()
{
    ofPushStyle();
    ofEnableDepthTest();
    ofSetColor(ofColor::white);
    ramSetViewPort(dpGetFirstScreenViewPort());
    ramBeginCamera();
    
    ofPushMatrix();
    //ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
    ofTranslate(mX-250.f, 0.f, mY-250.f);
    for (int i=0; i<mBlobs.size(); i++) {
        for (int j=0; j<mBlobs.at(i).size(); j++) {
            float a = (i+1)/(float)mBlobs.size();
            mBlobs.at(i).at(j)->draw(a);
        }
    }
    ofPopMatrix();
    
    ramEndCamera();
    
    ofPopStyle();
}

void HakovisLaser::exit()
{
    
}

void HakovisLaser::onPanelChanged(ofxUIEventArgs& e)
{
    
}

void HakovisLaser::drawActor(const ramActor &actor)
{
    
}
