#include "HakoniwaTheta.h"

void HakoniwaTheta::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    mDrawPreview	= true;
    mDrawDump		= true;
    mDrawhidden     = false;

    gui->addToggle("preview", &mDrawPreview);
    gui->addToggle("Dump", &mDrawDump);
    gui->addToggle("hidden", &mDrawhidden);
    gui->addIntSlider("video_no", 1, 5, &video_no);
    
    ofAddListener(gui->newGUIEvent, this, &HakoniwaTheta::onPanelChanged);
    
    motionExtractor.setupControlPanel(this);
    
}

void HakoniwaTheta::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0);
    ofEnableAlphaBlending();
    
    sphere.set(750, 50);
    sphere.setPosition(dpGetFirstScreenCenter());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    video_no = 1;
    vidPlay.loadMovie("1.MP4");
    vidPlay.setVolume(0);
    vidPlay.setLoopState(OF_LOOP_NORMAL);
    vidPlay.play();
    sphere.mapTexCoordsFromTexture(vidPlay.getTextureReference());

}

void HakoniwaTheta::update(){

    motionExtractor.update();
    vidPlay.update();

    port_no = motionExtractor.getNumPort()-1;
    int numActor = getNumNodeArray();
    
    if( mDrawhidden == true){
        radiusChanged(0);
    }else{
        radiusChanged(750);
    }

}

void HakoniwaTheta::draw(){

    ramSetViewPort(dpGetFirstScreenViewPort());
    vidPlay.getTextureReference().bind();
    
    ofQuaternion qForSphere_osc;
    qForSphere_osc = motionExtractor.getRotationAt(port_no);
    sphere.setOrientation(qForSphere_osc);
    
    sphere.draw();
    ofPopMatrix();
    
    if (mDrawPreview)	motionExtractor.draw();

}


void HakoniwaTheta::onPanelChanged(ofxUIEventArgs& e){

    const string name = e.widget->getName();

    if (name == "video_no") {
        VideoChanged(video_no);
    }
//    else if (name == "Fullscreen"){
//        sphere.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0); // 位置
//    }

}

void HakoniwaTheta::radiusChanged(int radius){

    sphere.set(radius, 50);
//    sphere.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0); // 位置
    sphere.setPosition(dpGetFirstScreenCenter()); // 位置
    

}

void HakoniwaTheta::VideoChanged(int no){

    ostringstream ss;
    ss << no << ".MP4";
    vidPlay.loadMovie(ss.str());
    vidPlay.setVolume(0);
    vidPlay.setLoopState(OF_LOOP_NORMAL);
    vidPlay.play();
    sphere.mapTexCoordsFromTexture(vidPlay.getTextureReference());

}