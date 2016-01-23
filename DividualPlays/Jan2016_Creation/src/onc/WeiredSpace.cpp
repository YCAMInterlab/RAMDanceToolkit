//
//  WeiredSpace.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "WeiredSpace.h"

WeiredSpace::WeiredSpace() :
displaceXY(false),
displaceZ(false)
{
    const int num = 120;
    const float s = 5.0;
    
    checker = vector<Rect>(num * num);
    
    for (int i=0; i<num; i++) {
        for (int j=0; j<num; j++) {
            Rect& r = checker.at(j + i * num);
            r.size = s;
            if ((i%2==0 && j%2==0) || (i%2==1 && j%2==1)) {
                r.color.set(0);
            }
            else {
                r.color.set(255);
            }
            r.center.x = i * s - s * num * 0.5;
            r.center.y = j * s - s * num * 0.5;
            
            r.spd.x = ofRandom(-1.0, 1.0);
            r.spd.y = ofRandom(-1.0, 1.0);
            r.spd.z = ofRandom(-1.0, 1.0);
            
            r.displacement.x = ofRandom(1.0, 4.0);
            r.displacement.y = ofRandom(1.0, 4.0);
            r.displacement.z = ofRandom(1.0, 4.0);
        }
    }
}

WeiredSpace::~WeiredSpace()
{
    
}

void WeiredSpace::setupControlPanel()
{
    ramGetGUI().addToggle("Displace XY", &displaceXY);
    ramGetGUI().addToggle("Displace Z", &displaceZ);
    ramGetGUI().addButton("Center cam");
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &WeiredSpace::onPanelChanged);
}

void WeiredSpace::update()
{
    for (int i=0; i<checker.size(); i++) {
        Rect& r = checker.at(i);
        if (displaceZ) {
            r.movement.z = ::cos(ofGetElapsedTimef() * r.spd.z) * r.displacement.z;
        }
        if (displaceXY) {
            r.movement.x = ::cos(ofGetElapsedTimef() * r.spd.x) * r.displacement.x;
            r.movement.y = ::cos(ofGetElapsedTimef() * r.spd.y) * r.displacement.y;
        }
    }
    
}

void WeiredSpace::draw()
{
    glEnable(GL_DEPTH_TEST);
    
    ramBeginCamera();
    
    ofPushMatrix();
    ofTranslate(0.f, 300.f, 300.0);
    drawChecker();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, 300.f, -300.0);
    drawChecker();
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotate(90.0, 0.0, 1.0, 0.0);
    ofTranslate(0.f, 300.f, -300.0);
    drawChecker();
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotate(90.0, 0.0, 1.0, 0.0);
    ofTranslate(0.f, 300.f, 300.0);
    drawChecker();
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotate(90.0, 1.0, 0.0, 0.0);
    ofTranslate(0.f, 0.f, 0.0);
    drawChecker();
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotate(90.0, 1.0, 0.0, 0.0);
    ofTranslate(0.f, 0.f, -600.0);
    drawChecker();
    ofPopMatrix();
    
    ramEndCamera();
}

void WeiredSpace::drawChecker()
{
    for (int i=0; i<checker.size(); i++) {
        Rect& r = checker.at(i);
        ofSetColor(r.color);
        ofRect(r.center + r.movement, r.size, r.size);
    }
}

void WeiredSpace::onPanelChanged(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
    if (name == "Center cam") {
        ramCameraManager::instance().getActiveCamera().setPosition(ofVec3f(0.0f, 300.f, 0.0f));
    }
    else if (name == "Displace XY") {
        ofxUIToggle* t = (ofxUIToggle *)e.widget;
        if (t->getValue() == false) {
            for (int i=0; i<checker.size(); i++) {
                Rect& r = checker.at(i);
                r.movement.x = 0.0;
                r.movement.y = 0.0;
            }
        }
    }
    else if (name == "Displace Z") {
        ofxUIToggle* t = (ofxUIToggle *)e.widget;
        if (t->getValue() == false) {
            for (int i=0; i<checker.size(); i++) {
                Rect& r = checker.at(i);
                r.movement.z = 0.0;
            }
        }
    }
}

void WeiredSpace::drawActor(const ramActor &actor)
{
    //    ramCameraManager::instance().getActiveCamera().setPosition(actor.getNode(ramActor::JOINT_HEAD));
    //    ramCameraManager::instance().getActiveCamera().lookAt(actor.getNode(ramActor::JOINT_HEAD).getZAxis());
    
    //    ramCameraManager::instance().getActiveCamera().setPosition(ofVec3f(0.f, 300.f, 0.f));
    //
    //    const float x = ::cos(ofGetElapsedTimef());
    //    const float y = ::sin(ofGetElapsedTimef());
    //
    //    ofVec3f lookat(x, 0.0, y);
    //    lookat.normalize();
    //
    //    ramCameraManager::instance().getActiveCamera().lookAt(lookat);
}






