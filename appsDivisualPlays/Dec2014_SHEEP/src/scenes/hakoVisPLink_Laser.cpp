//
//  hakoVisPLink_Laser.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/11.
//
//

#include "hakoVisPLink_Laser.h"

hakoVisPLink_Laser::hakoVisPLink_Laser(){

	receiver.addAddress("/dp/cameraUnit/");
	ramOscManager::instance().addReceiverTag(&receiver);

	lines.assign(300, liningUnit());
}

void hakoVisPLink_Laser::setupControlPanel(){

}

void hakoVisPLink_Laser::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/dp/cameraUnit/test/pixelate"){
			pix_w = m.getArgAsInt32(0);
			pix_h = m.getArgAsInt32(1);

			lines.clear();
			lines.assign(m.getArgAsInt32(0) * m.getArgAsInt32(1),liningUnit());

			for (int64_t i = 0;i < lines.size();i++){
				int64_t bt = m.getArgAsInt64(2 + i / 64);
				int64_t targetBit = int64_t(1) << (i % 64);
				lines[i].pixel = (bt & targetBit) > 0;
			}
		}
	}

	float rad = 400.0;
	float s,t;
	float tf = ofGetElapsedTimef()/5.0;
	for (int i = 0;i < lines.size();i++){
//		rad = ofNoise((i*52612.6124+tf)/2456.4231)*100;
		s = ofNoise((i/3.6124+tf)/5533.4231)*500;
		t = ofNoise((i/3.1542+tf)/3541.6245)*500;
		lines[i].pts.set(rad * cos(s) * cos(t),
						 rad * cos(s) * sin(t),
						 rad * sin(s));
	}

}

void hakoVisPLink_Laser::draw(){

	ofDisableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	if (lines.size() < 3) return;
	ramBeginCamera();
	ofPushMatrix();
//	ofRotate(ofGetElapsedTimef()*3);

	glPointSize(3.0);
	glBegin(GL_POINTS);
	ofSetColor(255, 50, 150);
	for (int i = 0;i < lines.size();i++){
		ofSetColor(ofColor(255,50,150,255));
		glVertex3d(lines[i].pts.x,lines[i].pts.y,lines[i].pts.z);
	}
	glEnd();

	ofSetColor(255);
	for (int i = 0;i < lines.size()-1;i++){
		if (lines[i].pixel)	lines[i].transp = 255;
		else				lines[i].transp -= 3;

		ofSetColor(100,lines[i].transp);
		for (int j = 0;j < 3;j++){
			ofLine(lines[i].pts,
				   lines[lines.size() * ofNoise((i+j)*34.134)].pts);
		}
	}
	ramEndCamera();

	ofPopMatrix();
	ofSetColor(255);
	ofEnableDepthTest();
}

void hakoVisPLink_Laser::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Laser::drawActor(const ramActor &actor){

}