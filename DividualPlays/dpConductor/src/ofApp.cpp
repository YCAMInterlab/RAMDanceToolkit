#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	dpCon = ofPtr<dpConductor>(new dpConductor);
	gui.conPtr = dpCon;


	dpCon->setup();
	gui.setup();
	
	receiver.setup(12400);
}

//--------------------------------------------------------------
void ofApp::update()
{
	dpCon->update();
	
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if (m.getAddress() == "/ram/set_scene") dpCon->receiveMasterHakoniwa(m);
		if (m.getAddress() == "/ram/set_scene") gui.setScene(m);
		if (m.getAddress() == "/que") dpCon->callSection(m.getArgAsString(0));
		if (m.getAddress() == "/ram/uiList")	gui.setDUI(m);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(50);
	
	dpCon->draw();
	
	ofPushMatrix();
	ofTranslate(ofGetMouseX(), ofGetMouseY());
	ofSetLineWidth(2.0);
	ofLine(-15, 0, 15, 0);
	ofLine(0, -15, 0, 15);
	ofSetLineWidth(1.0);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'l') dpCon->callSection("LineKojiri");
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
