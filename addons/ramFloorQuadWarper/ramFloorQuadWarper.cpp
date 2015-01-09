//
//  ramFloorQuadWarp.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#include "ramFloorQuadWarper.h"

ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

ramFloorQuadWarper* ramFloorQuadWarper::__instance = NULL;

void ramFloorQuadWarper::setupContolPanel(ramBaseScene *scene_, ofVec2f canvasPos){
    mGui = new ofxUICanvas();
    mGui->disableAppDrawCallback();
    mGui->disableMouseEventCallbacks();
    mGui->setUIColors(uiThemecb, uiThemeco, uiThemecoh,
                      uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
    mGui->addLabel("floorWarper");
    mGui->addButton("Save", false);
    mGui->addButton("Load", false);
    mGui->addSpacer();
    mGui->addButton("Reset", false);
    mGui->addToggle("Debug",&isDebug);
    
    mGui->setup();
    
    mGui->setPosition(canvasPos.x,
                      canvasPos.y);
    
    parentGui = ramGetGUI().getCurrentUIContext();
    parentGui->addWidget(mGui);
    
    mGui->autoSizeToFitWidgets();
    parentGui->autoSizeToFitWidgets();
    
    ofAddListener(mGui->newGUIEvent, this, &ramFloorQuadWarper::guiEvent);
    
}

void ramFloorQuadWarper::guiEvent(ofxUIEventArgs &e){
    ofxUIWidget* w = e.widget;
    
    if(w->getName() == "Debug"){
        if(isDebug)enableControls();
        else disableControls();
    }
    
    if(w->getName() == "Save"){
        save();
    }
    
    if(w->getName() == "Load"){
        load();
    }
    
    if(w->getName() == "Reset"){
        reset();
    }
}

void ramFloorQuadWarper::enableControls(){
    mWarper.enableKeyboardShortcuts();
    mWarper.enableMouseControls();
};

void ramFloorQuadWarper::disableControls(){
    mWarper.disableKeyboardShortcuts();
    mWarper.disableMouseControls();
};

void ramFloorQuadWarper::load(){
    mWarper.load("floorWarper.xml");
}

void ramFloorQuadWarper::save(){
    mWarper.save("floorWarper.xml");
};

void ramFloorQuadWarper::reset(){
    
    int offsetX = (FBO_WIDTH - FLOOR_WIDTH) * 0.5;
    int offsetY = (FBO_HEIGHT - FLOOR_HEIGHT) * 0.5;
    
    mWarper.setSourceRect(ofRectangle(offsetX,offsetY, FLOOR_WIDTH, FLOOR_HEIGHT));
    mWarper.setTopLeftCornerPosition(ofPoint(offsetX, offsetY));
    mWarper.setTopRightCornerPosition(ofPoint(FBO_WIDTH - offsetX, offsetY));
    mWarper.setBottomLeftCornerPosition(ofPoint(offsetX, FBO_HEIGHT - offsetY));
    mWarper.setBottomRightCornerPosition(ofPoint(FBO_WIDTH - offsetX, FBO_HEIGHT - offsetY));
    
}

void ramFloorQuadWarper::begin(){
    mFbo.begin();
    ofClear(0);
}

void ramFloorQuadWarper::end(){
    mFbo.end();
}

void ramFloorQuadWarper::setup(){
    mFbo.allocate(FBO_WIDTH, FBO_HEIGHT,GL_RGB);
    reset();
    mWarper.setup();
    load();
}

void ramFloorQuadWarper::draw(){
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ofDisableDepthTest();
    
    ofPushStyle();
    
    ofMatrix4x4 mat = getWarpedMatrix();
    
    ofPushMatrix();
    ofMultMatrix(mat);
    ofSetColor(255,255,255);
    mFbo.draw(0,0);
    ofPopMatrix();
    
    ofPopStyle();
    
    if(isDebug)drawDebug();
    
    glPopAttrib();
}

void ramFloorQuadWarper::drawDebug(){
    ofPushStyle();
    ofSetColor(ofColor::magenta);
    ofSetLineWidth(3);
    mWarper.drawQuadOutline();
    mWarper.drawCorners();
    mWarper.drawHighlightedCorner();
    mWarper.drawSelectedCorner();
    ofPopStyle();
};
