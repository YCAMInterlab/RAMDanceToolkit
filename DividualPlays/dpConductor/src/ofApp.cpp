#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	scenes_for_test.push_back("Line_ext");
	scenes_for_test.push_back("BigBox_ext");
	
	gui.setFontSize(OFX_UI_FONT_LARGE, 48);
	gui.setup();
	currentScene = gui.addDropDownList("Scene", scenes_for_test);
	gui.addButton("Enable", false);
	gui.addButton("Disable", false);
	gui.addSpacer();
	
	ofAddListener(gui.newGUIEvent, this, &ofApp::uiEvent);
	
	currentScene->setAutoClose(true);
	dpCon.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	dpCon.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(50);
	
	dpCon.draw();
	
	ofPushMatrix();
	ofTranslate(ofGetMouseX(), ofGetMouseY());
	ofLine(-5, 0, 5, 0);
	ofLine(0, -5, 0, 5);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') dpCon.callSection("testSection");
}

void ofApp::uiEvent(ofxUIEventArgs &e)
{
	ofxUIWidget* w = e.widget;
	
	if ((w == currentScene) && (currentScene->getSelectedNames().size() > 0))
	{
		currentScene->setLabelText(currentScene->getSelectedNames()[0]);
	}
	
	if (w->getName() == "Enable"  && w->getState() == OFX_UI_STATE_DOWN)
	{
		dpCon.sceneCon->setScene(currentScene->getSelectedNames()[0], true, true, true);
		dpCon.sceneCon->setScene(currentScene->getSelectedNames()[0], false, true, true);
	}
	if (w->getName() == "Disable" && w->getState() == OFX_UI_STATE_DOWN)
	{
		dpCon.sceneCon->disableScene(currentScene->getSelectedNames()[0], true);
		dpCon.sceneCon->disableScene(currentScene->getSelectedNames()[0], false);
	}
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
