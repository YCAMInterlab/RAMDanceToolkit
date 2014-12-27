#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(30, 30, 30);
	ser.listDevices();
	ser.setup("cu.usbserial-A5002vny",9600);
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){

//	if (!ofGetKeyPressed('a')){
//		unsigned char buf[8] = {ofRandom(49,58), ',',
//								ofRandom(49,58), ';',
//								ofRandom(49,58), ',',
//								ofRandom(49,58), '\r'};
//		ser.writeBytes(buf, 8);
//	}

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
