#include "ofApp.h"
dpVideoGrabberManager *mManager;
//--------------------------------------------------------------
void ofApp::setup(){
    mManager = &dpVideoGrabberManager::instance();
    mManager->setupDevice(0);
    
    mServer.setName("dpVideoSyphonServer");
}

//--------------------------------------------------------------
void ofApp::update(){
    mManager->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    mManager->draw(0,0);
    mServer.publishScreen();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
