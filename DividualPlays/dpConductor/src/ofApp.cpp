#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	dpCon = ofPtr<dpConductor>(new dpConductor);
	gui.conPtr = dpCon;

	gui.setup();
	dpCon->setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	dpCon->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(50);
	
	dpCon->draw();
	
	ofPushMatrix();
	ofTranslate(ofGetMouseX(), ofGetMouseY());
	ofLine(-5, 0, 5, 0);
	ofLine(0, -5, 0, 5);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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
