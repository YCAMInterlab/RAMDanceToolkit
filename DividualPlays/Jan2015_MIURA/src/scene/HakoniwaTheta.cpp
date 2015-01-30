#include "HakoniwaTheta.h"

void HakoniwaTheta::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    mDrawDump		= true;
    mDrawhidden     = false;

    gui->addToggle("Dump", &mDrawDump);
    gui->addToggle("Hidden", &mDrawhidden);
    gui->addIntSlider("Video No", 1, 5, &mVideoNo);
    
    ofAddListener(gui->newGUIEvent, this, &HakoniwaTheta::onPanelChanged);
    
    motionExtractor.setupControlPanel(this);
    motionExtractor.load("motionExt_HakoniwaTheta.xml");
    
}

void HakoniwaTheta::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0);
    ofEnableAlphaBlending();
    
    sphere.set(750, 50);
    sphere.setPosition(dpGetFirstScreenCenter());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    mVideoNo = 1;
    vidPlay.loadMovie("1.MP4");
    vidPlay.setVolume(0);
    vidPlay.setLoopState(OF_LOOP_NORMAL);
    vidPlay.play();
    sphere.mapTexCoordsFromTexture(vidPlay.getTextureReference());

}

void HakoniwaTheta::update(){

    motionExtractor.update();
    vidPlay.update();

    portNo = motionExtractor.getNumPort()-1;
    int numActor = getNumNodeArray();
    
    if( mDrawhidden == true){
        radiusChanged(0);
    }else{
        radiusChanged(750);
    }

}

void HakoniwaTheta::draw(){

    ramSetViewPort(dpGetFirstScreenViewPort());
    
    if (ofGetFrameNum() % 600 == 0){
        mVideoNo = ofRandom(1 , 6);
        VideoChanged(mVideoNo);
    }
    
    vidPlay.getTextureReference().bind();
    
    ofQuaternion qForSphere_osc;
    qForSphere_osc = motionExtractor.getRotationAt(portNo);
    sphere.setOrientation(qForSphere_osc);
    
    sphere.draw();
    
}

void HakoniwaTheta::onPanelChanged(ofxUIEventArgs& e){

    const string name = e.widget->getName();
    if (name == "Video No") {
        VideoChanged(mVideoNo);
    }

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

void HakoniwaTheta::drawDump(){
    
    int motionExtCnt = 0;
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        ofPushMatrix();
        ofTranslate(700, i*75);
        
        
        ofNoFill();
        ofRect(0, 0, 200, 75);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + motionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + motionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(motionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, motionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        ofPopMatrix();
    }
}