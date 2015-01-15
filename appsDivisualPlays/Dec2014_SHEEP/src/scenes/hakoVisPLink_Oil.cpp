//
//  hakoVisPLink_Oil.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#include "hakoVisPLink_Oil.h"

hakoVisPLink_Oil::hakoVisPLink_Oil(){

	receiver.addAddress("/dp/cameraUnit/plink_Oil");
	ramOscManager::instance().addReceiverTag(&receiver);

}

void hakoVisPLink_Oil::setupControlPanel(){
	gui = ramGetGUI().getCurrentUIContext();

	vector<string> patterns;
	patterns.push_back("Pattern_A");
	patterns.push_back("Pattern_B");
	patterns.push_back("Pattern_C");

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
				}
			}
		}

	}

	for (int i = 0;i < chasers.size();i++){
		int targ = isExistByLabel(chasers[i].label);
		cout << targ << endl;
		if (targ > -1){
			chasers[i].pos = pts[targ] * 400;
		}

		chasers[i].update();
	}

}

void hakoVisPLink_Oil::draw(){

	for (int i = 0;i < pts.size();i++){
		ofPushMatrix();
		ofTranslate(pts[i] * 400);
		ofLine(-5, 0, 5, 0);
		ofLine(0, -5, 0, 5);
		ofDrawBitmapString(ofToString(labels[i]), 20,20);
		ofPopMatrix();
	}

	for (int i = 0;i < chasers.size();i++){
		ofPushMatrix();
		ofTranslate(chasers[i].pos);
		ofCircle(0, 0, 10);
		ofPopMatrix();
	}

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