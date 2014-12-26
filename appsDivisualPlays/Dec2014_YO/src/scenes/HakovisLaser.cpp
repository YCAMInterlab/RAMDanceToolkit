//
//  HakovisLaser.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 12/26/14.
//
//

#include "HakovisLaser.h"

HakovisLaser::Blob::Blob(int index, int nVerts) :
idx(index)
{
    
}

void HakovisLaser::Blob::draw()
{
    polyLine.draw();
}

HakovisLaser::HakovisLaser() :
mOscReceiverTag(NULL)
{
    mOscReceiverTag = new ramOscReceiveTag;
    ramOscManager::instance().addReceiverTag(mOscReceiverTag);
    mOscReceiverTag->addAddress("/dp/cameraUnit/prism/contour/blob");
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
    ramGetGUI().addSlider("Blob Scale", 0.f, 10.f, &mBlobScale);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent,
                  this,
                  &HakovisLaser::onPanelChanged);
}

void HakovisLaser::update()
{
    mBlobs.clear();
    while (mOscReceiverTag->hasWaitingMessages()) {
        ofxOscMessage m;
        mOscReceiverTag->getNextMessage(&m);
        //cout << m.getAddress() << endl;
        //for (int i=0; i<m.getNumArgs(); i++) {
        //    cout << m.getArgTypeName(i) << " ";
        //}
        //cout << endl;
        
        if (m.getAddress() == "/dp/cameraUnit/prism/contour/blob") {
            const int blobIdx = m.getArgAsInt32(0);
            const int nVerts = m.getArgAsInt32(1);
            
            cout << blobIdx << ", " << nVerts << endl;
            
            ofPtr<Blob> blob = ofPtr<Blob>(new Blob(blobIdx, nVerts));

            for (int i=0; i<nVerts; i+=2) {
                ofVec2f v;
                v.x = m.getArgAsFloat(i+0+2);
                v.y = m.getArgAsFloat(i+1+2);
                blob->polyLine.addVertex(v * mBlobScale);
            }
            
            mBlobs.push_back(blob);
        }
    }
}

void HakovisLaser::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofSetColor(ofColor::white);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
    for (int i=0; i<mBlobs.size(); i++) {
        mBlobs.at(i)->draw();
    }
    ofPopMatrix();
    
    ramBeginCamera();
    
    
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
