#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(70, 80, 70);
	ofSetFrameRate(30);

	receiver.setup(12400);
}

//--------------------------------------------------------------
void ofApp::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
	}

	dpCameraUnit.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	dpCameraUnit.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'q') bDebug ^= true;
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
