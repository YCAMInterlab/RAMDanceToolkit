#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	dpCon.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

	dpCon.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	dpCon.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == ' ') dpCon.callSection("LineKojiri");
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
