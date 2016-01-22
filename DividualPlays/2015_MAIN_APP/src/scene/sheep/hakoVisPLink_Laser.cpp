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

	pix_w = 0;
	pix_h = 0;
	step = 5;
}

void hakoVisPLink_Laser::setupControlPanel(){
	gui = ramGetGUI().getCurrentUIContext();

	vector<string> patterns;
	patterns.push_back("Pattern_A");
	patterns.push_back("Pattern_B");
	patterns.push_back("Pattern_C");

    gui->addRadio("Patterns", patterns)->activateToggle("Pattern_B");
	gui->addSpacer();
	gui->addLabel("Adjustment",OFX_UI_FONT_LARGE);
	gui->addSlider("Scale", 0.0, 1.0, &mScale)->setValue(0.5);
	gui->addSlider("LineWidth", 0.0, 1.0, &mLineScale)->setValue(0.1);
	gui->addToggle("ManualCam", &mManualCam);
	gui->addToggle("FaceBlink", &mFaceBlink);

	ShCam.setMode(SHEEPCAM_SIGMOID_WANDER);
}

void hakoVisPLink_Laser::update(){

	ShCam.update();
	
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

	for (int i = 0;i < lines.size()-1;i++){
		if (lines[i].pixel){
			if (lines[i].stand){
				lines[i].transp = ofRandom(80);
				lines[i].stand = false;
			}
		}
		else{
			lines[i].stand = true;
			lines[i].transp /= 1.15;
		}

		if (lines[i].transp_smooth < lines[i].transp){
			lines[i].pt_smooth += (lines[i].pts - lines[i].pt_smooth) / 1.5;
			lines[i].transp_smooth += (lines[i].transp - lines[i].transp_smooth) / 1.8;
		}else{
			lines[i].pt_smooth += (lines[i].pts - lines[i].pt_smooth) / 62.0;
			lines[i].transp_smooth += (lines[i].transp - lines[i].transp_smooth) / 63.0;
		}
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
			if (tv.y > 10.0){
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

	ShCam.camera.begin();
	ofEnableDepthTest();

	ofPushMatrix();
	ofRotateY(ofGetElapsedTimef()*2);

	ofSetColor(0);
	msh.draw(OF_MESH_FILL);
//	glBegin(GL_TRIANGLES);
//	for (int i = 0;i < msh.getNumIndices();i+=3){
//
//
//		for (int j = 0;j < 3;j++){
//			ofColor c = ofColor(255,50,150);
//			c *= MIN(1.0,msh.getVertex(msh.getIndex(i+j)).y);
//
//			ofSetColor(c);
//			glVertex3d(msh.getVertex(msh.getIndex(i+j)).x,
//					   msh.getVertex(msh.getIndex(i+j)).y,
//					   msh.getVertex(msh.getIndex(i+j)).z);
//		}
//
//	}
//	glEnd();

	ofTranslate(0, 1);
	ofSetColor(255);
	ofSetLineWidth(3.0);
	msh.draw(OF_MESH_WIREFRAME);

	if (ofGetFrameNum() % step == 0){
		step = ofRandom(15,500);
		flickStop = (ofRandomuf() < 0.4);
	}

	if (!flickStop) flick_counter+=2;

	for (int i = 0;i < msh.getNumIndices()-2;i+=3){
		if (msh.getVertex(msh.getIndex(i+0)).y > 30 ||
			msh.getVertex(msh.getIndex(i+1)).y > 30 ||
			msh.getVertex(msh.getIndex(i+2)).y > 30){

			float rd = pow(ofRandomuf(),2.0f);
			ofSetColor(10);

			if (ofNoise(i*4243.324 + flick_counter / 30.0) > 0.5) ofSetColor(255);
			if (ofNoise(i*4243.324 + flick_counter / 30.0) > 0.8) ofSetColor(dpColor::MAIN_COLOR);

			glBegin(GL_TRIANGLES);
			for (int j = 0;j < 3;j++){
				glVertex3d(msh.getVertex(msh.getIndex(i+j)).x,
						   msh.getVertex(msh.getIndex(i+j)).y,
						   msh.getVertex(msh.getIndex(i+j)).z);
			}
			glEnd();
		}
	}


	ofSetColor(255, 50, 150);
	ofSetLineWidth(1.5);

	ofPushMatrix();
	ofNoFill();
	ofTranslate(0, 200, 0);
	ofRotateX(90);
	ofSetLineWidth(12.0);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	del.draw();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	ofSetLineWidth(1.0);
	ofFill();
	ofPopMatrix();

	ofSetLineWidth(1.0);

	ofPopMatrix();

	ShCam.camera.end();

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