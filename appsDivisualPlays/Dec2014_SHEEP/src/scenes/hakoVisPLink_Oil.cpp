//
//  hakoVisPLink_Oil.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#include "hakoVisPLink_Oil.h"

hakoVisPLink_Oil::hakoVisPLink_Oil(){

	receiver.addAddress("/dp/cameraUnit/Plink_Oil");
	ramOscManager::instance().addReceiverTag(&receiver);

}

void hakoVisPLink_Oil::setupControlPanel(){
	gui = ramGetGUI().getCurrentUIContext();

	vector<string> patterns;
	patterns.push_back("Pattern_A");
	patterns.push_back("Pattern_B");
	patterns.push_back("Pattern_C");

	gui->addToggle("DebugDraw", &mDebugDraw);

}

void hakoVisPLink_Oil::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if (m.getAddress() == "/dp/cameraUnit/plink_Oil/contour/boundingRect"){
			int nBlob = m.getArgAsInt32(0);
			pts.assign(nBlob, ofVec3f(0,0,0));
			labels.assign(nBlob, 0);

			for (int i = 0;i < nBlob;i++){
				labels[i] = m.getArgAsInt32(1+i*5);
				pts[i].set(m.getArgAsFloat(1+i*5+1) + m.getArgAsFloat(1+i*5+3)/2.0,
						   m.getArgAsFloat(1+i*5+2) + m.getArgAsFloat(1+i*5+4)/2.0);
			}

			for (int i = 0;i < labels.size();i++){
				if (!isExistByChaser(labels[i])){
					chasers.push_back(labelChaser(labels[i]));
					chasers.back().pos_smooth = pts[i] * SINGLE_SCREEN_HEIGHT + ofVec2f(-SINGLE_SCREEN_HEIGHT,SINGLE_SCREEN_HEIGHT);
				}
			}
		}

	}

	for (int i = 0;i < chasers.size();i++){
		int targ = isExistByLabel(chasers[i].label);
		if (targ > -1 && !chasers[i].dead){
			chasers[i].pos = pts[targ] * SINGLE_SCREEN_HEIGHT;
		}else{
			chasers[i].dead = true;
			chasers[i].vec += ofVec3f(1.0,-1.0);
			chasers[i].pos += chasers[i].vec;
		}

		chasers[i].update();
	}

	for (int i = 0;i < chasers.size();i++){
		if (chasers[i].vec.lengthSquared() > 100){
			chasers.erase(chasers.begin()+i);
		}
	}

}

void hakoVisPLink_Oil::draw(){

	if (mDebugDraw){
		for (int i = 0;i < pts.size();i++){
			ofPushMatrix();
			ofTranslate(pts[i] * SINGLE_SCREEN_HEIGHT);
			ofLine(-5, 0, 5, 0);
			ofLine(0, -5, 0, 5);
			ofDrawBitmapString(ofToString(labels[i]), 20,20);
			ofPopMatrix();
		}
	}

	for (int i = 0;i < chasers.size();i++){
		ofPushMatrix();

		ofTranslate(chasers[i].pos_smooth);
//		ofRotateZ(45);

		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(chasers[i].col);
//		ofRect(0, 0, pow(chasers[i].seed,4.0f)*50, SINGLE_SCREEN_HEIGHT);
		ofSetLineWidth(3.0);
		ofLine(-5000, 0, 5000, 0);
		ofLine(0, -5000, 0, 5000);
		ofSetLineWidth(3.0);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofPopMatrix();
	}

	ofSetColor(255);

}

void hakoVisPLink_Oil::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Oil::draw_PatternA(){

}

void hakoVisPLink_Oil::draw_PatternB(){

}

bool hakoVisPLink_Oil::isExistByChaser(int label){
	for (int i = 0;i < chasers.size();i++){
		if (chasers[i].label == label) return true;
	}
	return false;
}

int hakoVisPLink_Oil::isExistByLabel(int label){
	for (int i = 0;i < labels.size();i++){
		if (labels[i] == label) return i;
	}
	return -1;
}