//
//  hakoVisPLink_Prism.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#include "hakoVisPLink_Prism.h"

hakoVisPLink_Prism::hakoVisPLink_Prism(){

	grid.setGridScale(75.0);
	receiver.addAddress("/dp/cameraUnit/Plink_Prism");
	ramOscManager::instance().addReceiverTag(&receiver);

}

void hakoVisPLink_Prism::setupControlPanel(){

}

void hakoVisPLink_Prism::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if (m.getAddress() == "/dp/cameraUnit/Plink_Prism/contour/boundingRect"){
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
					chasers.push_back(hexChaser());
					chasers.back().setGrid(&grid, labels[i]);
					chasers.back().setDefault(pts[i].x*30,
											  pts[i].y*20);

				}
			}
		}
		
	}


	for (int i = 0;i < chasers.size();i++){
		int targ = isExistByLabel(chasers[i].label);
		if (targ > -1 && !chasers[i].dead){
			chasers[i].setNextPosition(pts[i].x*30, pts[i].y*20);
		}else{
			chasers[i].dead = true;
		}

		chasers[i].update();
	}

	for (int i = 0;i < chasers.size();i++){
		if (chasers[i].dead) chasers.erase(chasers.begin()+i);
	}
}

void hakoVisPLink_Prism::draw(){
	ofDisableDepthTest();
	ofSetColor(120);
	for (int i = 0;i < 30;i++){
		for (int j = 0;j < 20;j++){

			if (i % 2 == 1){
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i-1, j-1));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i+1, j-1));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i-1, j+0));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i+1, j+0));
			}else{
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i-1, j+0));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i+1, j+0));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i-1, j+1));
				ofLine(grid.getPosition(i, j),
					   grid.getPosition(i+1, j+1));
			}
			ofLine(grid.getPosition(i, j),
				   grid.getPosition(i+0, j+1));
			ofLine(grid.getPosition(i, j),
				   grid.getPosition(i+0, j-1));

		}
	}

	for (int i = 0;i < chasers.size();i++){
		ofSetColor(255);
		ofPushMatrix();

		ofSetLineWidth(2.0);
		ofTranslate(chasers[i].pos_smooth);
		ofLine(-10, 0, 10, 0);
		ofLine(0, -10, 0, 10);

		ofSetLineWidth(3.0);
		ofPopMatrix();
		glBegin(GL_LINE_STRIP);
		for (int j = 0;j < chasers[i].pos_log.size();j++){
			ofSetColor(dpColor::MAIN_COLOR);
			glVertex2d(chasers[i].pos_log[j].x,
					   chasers[i].pos_log[j].y);
		}
		glEnd();
	}
	ofSetLineWidth(1.0);
	ofEnableDepthTest();
}

void hakoVisPLink_Prism::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Prism::draw_PatternA(){

}

void hakoVisPLink_Prism::draw_PatternB(){

}



bool hakoVisPLink_Prism::isExistByChaser(int label){
	for (int i = 0;i < chasers.size();i++){
		if (chasers[i].label == label) return true;
	}
	return false;
}

int hakoVisPLink_Prism::isExistByLabel(int label){
	for (int i = 0;i < labels.size();i++){
		if (labels[i] == label) return i;
	}
	return -1;
}