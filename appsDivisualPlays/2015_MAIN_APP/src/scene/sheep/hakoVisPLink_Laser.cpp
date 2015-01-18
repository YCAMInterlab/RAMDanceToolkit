//
//  hakoVisPLink_Laser.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/11.
//
//

#include "hakoVisPLink_Laser.h"

hakoVisPLink_Laser::hakoVisPLink_Laser(){

	receiver.addAddress("/dp/cameraUnit/Plink_Laser");
	ramOscManager::instance().addReceiverTag(&receiver);

	lines.assign(300, liningUnit());
}

void hakoVisPLink_Laser::setupControlPanel(){
	gui = ramGetGUI().getCurrentUIContext();

	vector<string> patterns;
	patterns.push_back("Pattern_A");
	patterns.push_back("Pattern_B");
	patterns.push_back("Pattern_C");

    gui->addRadio("Patterns", patterns)->activateToggle("Pattern_B");
}

void hakoVisPLink_Laser::update(){
	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/dp/cameraUnit/Plink_Laser/pixelate"){
			pix_w = m.getArgAsInt32(0);
			pix_h = m.getArgAsInt32(1);
			if (lines.size() != pix_w * pix_h){
				lines.clear();
				lines.assign(m.getArgAsInt32(0) * m.getArgAsInt32(1),liningUnit());
			}

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
		s = ofNoise((i/3.6124+tf)/5533.4231)*500;
		t = ofNoise((i/3.1542+tf)/3541.6245)*500;
		lines[i].pts.set(rad * cos(s) * cos(t),
						 rad * cos(s) * sin(t),
						 rad * sin(s));
	}

}

void hakoVisPLink_Laser::draw(){

	ofxUIRadio* pat = (ofxUIRadio*)(gui->getWidget("Patterns"));
	if (pat->getActiveName() == "Pattern_A") draw_PatternA();
	if (pat->getActiveName() == "Pattern_B") draw_PatternB();

}

void hakoVisPLink_Laser::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Laser::drawActor(const ramActor &actor){

}

void hakoVisPLink_Laser::draw_PatternB(){
	camera.setFov(75.0);
	camera.setPosition(0.0, 200.0, 500.0);
	camera.lookAt(ofVec3f(0.0,0.0,0.0));

	for (int i = 0;i < lines.size()-1;i++){
		if (lines[i].pixel){
			if (lines[i].stand){
				lines[i].transp = ofRandom(80);
				lines[i].stand = false;
			}
		}
		else{
			lines[i].stand = true;
			lines[i].transp /= 2.0;
		}
		lines[i].pt_smooth += (lines[i].pts - lines[i].pt_smooth) / 2.0;
		lines[i].transp_smooth += (lines[i].transp - lines[i].transp_smooth) / 3.0;
	}

	ofMesh msh;

	del.reset();
	int counter = 0;
	for (int j = 0;j < pix_h;j++){
		for (int i = 0;i < pix_w;i++){
			ofVec3f tv =ofVec3f((i-pix_w/2)*60,
								lines[j*pix_w+i].transp_smooth+
								lines[j*pix_w+i].pixel * ofRandom(0),
								(j-pix_h/2)*60);
			msh.addVertex(tv);
			if (tv.y > 0){
				counter++;
				del.addPoint(tv.x,tv.z,100+tv.y/10.0);
			}

			if ((i < (pix_w - 1)) &&
				(j < (pix_h - 1))){

				msh.addIndex(j*pix_w+i);
				msh.addIndex(j*pix_w+i+1);
				msh.addIndex((j+1)*pix_w+i);

				msh.addIndex(j*pix_w+i+1);
				msh.addIndex((j+1)*pix_w+i);
				msh.addIndex((j+1)*pix_w+i+1);

			}
		}
	}

	if (counter > 4) del.triangulate();

	camera.begin();
	ofEnableDepthTest();

	ofPushMatrix();
	ofRotateY(ofGetElapsedTimef()*5);

	ofSetColor(0);
	msh.draw(OF_MESH_FILL);

	ofTranslate(0, 1);
	ofSetColor(255);
	msh.draw(OF_MESH_WIREFRAME);


	ofSetColor(255, 50, 150);
	ofSetLineWidth(1.5);

	ofPushMatrix();
	ofNoFill();
	ofTranslate(0, 200, 0);
	ofRotateX(90);
	del.draw();
	ofFill();
	ofPopMatrix();

	ofSetLineWidth(1.0);

	ofPopMatrix();

	camera.end();

	ofSetLineWidth(1.0);
	ofSetColor(255);
	ofPopMatrix();

}

void hakoVisPLink_Laser::draw_PatternA(){
	ofDisableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	if (lines.size() < 3) return;
	ramBeginCamera();
	ofPushMatrix();
	ofRotateY(ofGetElapsedTimef()*5);

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
	ofPopMatrix();
	ramEndCamera();

	ofSetColor(255);
	ofEnableDepthTest();
}