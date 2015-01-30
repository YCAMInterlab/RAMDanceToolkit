#include "HakoniwaTheta.h"

void HakoniwaTheta::setupControlPanel(){
    
    ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
    mDrawDump		= true;
    mDrawhidden     = false;

    gui->addToggle("Dump", &mDrawDump);
    gui->addToggle("Hidden", &mDrawhidden);
    gui->addIntSlider("Video No", 1, 3, &mVideoNo);
    gui->addSlider("Degree",0.0,360.0,&mDegreeOffset);
    
    mDegreeOffset = 180.0;
    
    ofAddListener(gui->newGUIEvent, this, &HakoniwaTheta::onPanelChanged);
    
    motionExtractor.setupControlPanel(this);
    motionExtractor.load("motionExt_HakoniwaTheta.xml");
    
}

void HakoniwaTheta::setup(){
    
    ofEnableAlphaBlending();
    
    sphere.set(700, 50);
    sphere.setPosition(dpGetFirstScreenCenter());
    sphere.setPosition(0, 0, 0);
    
    mVideoNo = 1;
    alpha = 255;

    for(int i = 1; i < 4; i++){
        ostringstream ss;
        ss <<  "./HakoniwaTheta/" << i << ".mov";
        vidPlay[i].loadMovie(ss.str());
        vidsetup[i] = false;
        mVidFirstPlay[i] = false;

    }
}

void HakoniwaTheta::update(){

    motionExtractor.update();
    
    for(int i = 1; i < 4; i++){
        if(vidPlay[i].isLoaded() && vidsetup[i] == false){
            vidPlay[i].play();
            vidPlay[i].setVolume(0);
            vidPlay[i].setLoopState(OF_LOOP_NORMAL);
            vidsetup[i] = true;
            cout << "play " << i << endl;
        }
    }
    
    vidPlay[mVideoNo].update();

    portNo = motionExtractor.getNumPort()-1;
    int numActor = getNumNodeArray();
    
    if( mDrawhidden == true){
        radiusChanged(0);
    }else{
        radiusChanged(700);
    }
    
    alpha += alphaSpeed;
    
    if(alpha > 255)alpha = 255;

}

void HakoniwaTheta::draw(){

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    ramSetViewPort(dpGetFirstScreenViewPort());
    
    ramBeginCamera();
    ofRotateZ(mDegreeOffset);

    if (ofGetFrameNum() % 3600 == 0){
        
        cout << "mVidFirstPlay[mVideoNo]  " << mVidFirstPlay[mVideoNo]  << endl;
        if(vidsetup[mVideoNo] == true && mVidFirstPlay[mVideoNo] == false){
            alpha = 0;
            mVidFirstPlay[mVideoNo] = true;
        }
        
        if(mVideoNo == 3){
            mVideoNo = 1;
        }else{
            mVideoNo++;
        }

//        mVideoNo = ofRandom(1 , 4);
        VideoChanged(mVideoNo);
    }
    
    sphere.mapTexCoordsFromTexture(vidPlay[mVideoNo].getTextureReference());
    vidPlay[mVideoNo].getTextureReference().bind();
    
    ofQuaternion qForSphere_osc;
    qForSphere_osc = motionExtractor.getRotationAt(portNo);
    sphere.setOrientation(qForSphere_osc);

    ofSetColor(255,255,255,alpha);
    sphere.draw();
    vidPlay[mVideoNo].getTextureReference().unbind();
    
    ramEndCamera();
    
    glDisable(GL_CULL_FACE);
}

void HakoniwaTheta::onPanelChanged(ofxUIEventArgs& e){

    const string name = e.widget->getName();
    if (name == "Video No") {
        VideoChanged(mVideoNo);
    }

}

void HakoniwaTheta::radiusChanged(int radius){

    sphere.set(radius, 50);
    sphere.setPosition(0, 0, 0);

}

void HakoniwaTheta::VideoChanged(int no){

    sphere.mapTexCoordsFromTexture(vidPlay[no].getTextureReference());

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

void HakoniwaTheta::onDisabled(){
    
    cout << "ondisebled " << endl;
//    for(int i = 1; i<6; i++){
//        
//        ostringstream ss;
//        ss <<  "./HakoniwaTheta/" << i << ".MP4";
//        vidPlay[i].stop();
//    }
    
}

void HakoniwaTheta::onEnabled(){
    
    cout << "onenabled " << endl;
//    for(int i = 1; i<6; i++){
//        
//        ostringstream ss;
//        ss <<  "./HakoniwaTheta/" << i << ".MP4";
//        if(vidPlay[i].isLoaded()){
//            vidPlay[i].play();
//        }
//    }
    
}
