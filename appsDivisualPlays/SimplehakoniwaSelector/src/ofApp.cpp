#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	sender.setup("192.168.20.5", 12400);
	sender.setup("localhost",12400);

	hakoniwas.push_back("dpVisPLink_Laser");
	hakoniwas.push_back("dpVisServoPendulum");
	hakoniwas.push_back("dpVisMagPendulum");
	hakoniwas.push_back("dpVisSandStorm");
	hakoniwas.push_back("dpVisTheta");
	hakoniwas.push_back("dpVisStruggle");
	hakoniwas.push_back("dpVisStage");
	hakoniwas.push_back("dpHGearMove");
	hakoniwas.push_back("dpHWorm");
	hakoniwas.push_back("dpVisTornado");
	hakoniwas.push_back("dpHfrozenIce");
	hakoniwas.push_back("dpVisPLink_Prism");
	hakoniwas.push_back("dpVisPLink_Oil");

	hakoniwas.push_back("TestSceneA");
	hakoniwas.push_back("TestSceneB");
	hakoniwas.push_back("TestSceneC");
	hakoniwas.push_back("TestSceneD");
	hakoniwas.push_back("Blank");

	selector_hakoniwa.setup();
	selector_hakoniwa.addLabel("TargetHakoniwa",OFX_UI_FONT_LARGE);
	selector_hakoniwa.addTextInput("Target", "")->setAutoClear(false);
	selector_hakoniwa.addSpacer();
	selector_hakoniwa.addRadio("Hakoniwa", hakoniwas);
	selector_hakoniwa.autoSizeToFitWidgets();
	selector_hakoniwa.setPosition(10, 30);

	selector_display.setup();
	selector_display.addToggle("Enable", &mEnable);
	selector_display.addToggle("DISP_A", &displays[0]);
//	selector_display.addToggle("DISP_B", &displays[1]);
	selector_display.addToggle("DISP_B", &displays[2]);
//	selector_display.addToggle("DISP_D", &displays[3]);
	selector_display.addButton("Send", false);
	selector_display.addSpacer();
	selector_display.addButton("AllClear", false);
	selector_display.addLabel("IPAddress",OFX_UI_FONT_LARGE);
//	selector_display.addToggle("Auto Shuffle", &mAutoShuffle);
	addressInput = selector_display.addTextInput("IPAddress", "192.168.20.5");
	addressInput->setAutoClear(false);

	selector_display.autoSizeToFitWidgets();
	selector_display.setPosition(220, 30);

	ofAddListener(selector_display.newGUIEvent, this,
				  &ofApp::guiEvent);
	ofAddListener(selector_hakoniwa.newGUIEvent, this,
				  &ofApp::guiEvent);

	ofCircle(0, 0, 0);
}

void ofApp::guiEvent(ofxUIEventArgs &ev){
	ofxUIWidget* w = ev.widget;

	if (w->getName() == "AllClear" && w->getState() == OFX_UI_STATE_DOWN){
		for (int i = 0;i < hakoniwas.size();i++){
			((ofxUITextInput*)(selector_hakoniwa.getWidget("Target")))->setTextString(hakoniwas[i]);
			mEnable = false;
			displays[0] = false;
			displays[1] = false;
			displays[2] = false;
			displays[3] = false;
			sendMessage();
		}
	}

	if (w->getName() == "Send" && w->getState() == OFX_UI_STATE_DOWN){
		sendMessage();
	}

	for (int i = 0;i < hakoniwas.size();i++){
		if (w->getName() == hakoniwas[i]){
			((ofxUITextInput*)(selector_hakoniwa.getWidget("Target")))->setTextString(hakoniwas[i]);
		}
	}

	if (w->getName() == "Target"){
		ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa.getWidget("Hakoniwa"));
		hkn->activateToggle("");
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	if (mAutoShuffle && (ofGetFrameNum() % 30 == 0)){
		ofxUIRadio* slt = (ofxUIRadio*)(selector_display.getWidget("Slot"));
		ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa.getWidget("Hakoniwa"));

		mEnable = (ofRandomuf() < 0.5);
		for (int i = 0;i < 4;i++)
			displays[i] = ofRandomuf() < 0.5;

		hkn->activateToggle(hakoniwas[int(ofRandom(4))+5]);
		((ofxUITextInput*)(selector_hakoniwa.getWidget("Target")))->setTextString(hkn->getActiveName());

		sendMessage();
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofxUIRadio* slt = (ofxUIRadio*)(selector_display.getWidget("Slot"));
	ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa.getWidget("Hakoniwa"));

//	if (key == 'i') slt->activateToggle("A");
//	else if (key == 'o') slt->activateToggle("B");
//	else if (key == 'k') slt->activateToggle("C");
//	else if (key == 'l') slt->activateToggle("D");
//
//	if (key == '1') hkn->activateToggle("testA");
//	else if (key == '2') hkn->activateToggle("testB");
//	else if (key == '3') hkn->activateToggle("testC");
//	else if (key == '4') hkn->activateToggle("testD");

	if (key == ' '){
		sendMessage();
	}
}

void ofApp::sendMessage(){
	ofxUIRadio* hkn = (ofxUIRadio*)(selector_hakoniwa.getWidget("Hakoniwa"));

	string h = ((ofxUITextInput*)(selector_hakoniwa.getWidget("Target")))->getTextString();
	string ad = addressInput->getTextString();

	int hh;
	for (int i = 0;i < hakoniwas.size();i++){
		if (h == hakoniwas[i]) hh = i;
	}

	ofxOscMessage m;
	m.setAddress("/ram/set_scene");
	m.addStringArg(h);
	m.addIntArg(mEnable);
	m.addIntArg(displays[0]);
	m.addIntArg(displays[0]);
	m.addIntArg(displays[2]);
	m.addIntArg(displays[2]);

	sender.setup(ad, 12400);
	sender.sendMessage(m);
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
