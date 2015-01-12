#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(70, 80, 70);
	ofSetFrameRate(30);

	receiver.setup(10000);
	dpSwitcher.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		dpSwitcher.receiveOscMessage(m);
	}

	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	dpSwitcher.update();
	dpCameraUnit.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	dpCameraUnit.draw();
	dpSwitcher.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') dpSwitcher.SelectHakoniwa(HAKO_STRUGGLE, 0);
	if (key == '2') dpSwitcher.SelectHakoniwa(HAKO_STRUGGLE, 1);
	if (key == '3') dpSwitcher.SelectHakoniwa(HAKO_STRUGGLE, 2);
	if (key == '4') dpSwitcher.SelectHakoniwa(HAKO_STRUGGLE, 3);

	if (key == 'q') dpSwitcher.SelectHakoniwa(HAKO_FROZENICE, 0);
	if (key == 'w') dpSwitcher.SelectHakoniwa(HAKO_FROZENICE, 1);
	if (key == 'e') dpSwitcher.SelectHakoniwa(HAKO_FROZENICE, 2);
	if (key == 'r') dpSwitcher.SelectHakoniwa(HAKO_FROZENICE, 3);

	if (key == 'a') dpSwitcher.SelectHakoniwa(HAKO_PLINK_LASER, 0);
	if (key == 's') dpSwitcher.SelectHakoniwa(HAKO_PLINK_LASER, 1);
	if (key == 'd') dpSwitcher.SelectHakoniwa(HAKO_PLINK_LASER, 2);
	if (key == 'f') dpSwitcher.SelectHakoniwa(HAKO_PLINK_LASER, 3);

	if (key == 'z') dpSwitcher.SelectHakoniwa(HAKO_PLINK_MAGNET, 0);
	if (key == 'x') dpSwitcher.SelectHakoniwa(HAKO_PLINK_MAGNET, 1);
	if (key == 'c') dpSwitcher.SelectHakoniwa(HAKO_PLINK_MAGNET, 2);
	if (key == 'v') dpSwitcher.SelectHakoniwa(HAKO_PLINK_MAGNET, 3);
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
