#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(70, 80, 70);
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
	dpCameraUnit.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	dpCameraUnit.draw();
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
	dpCameraUnit.inputUnit.mVideoPlayer.loadMovie(dragInfo.files[0]);
}
