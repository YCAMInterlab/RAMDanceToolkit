//
//  distanceMetaball.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#include "distanceMetaball.h"

distanceMetaball::distanceMetaball(){

}

void distanceMetaball::setupControlPanel(){

	ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();

	gui->addRangeSlider("Radius", 0.0, 1.0, &radius_low, &radius_high);
	gui->addSlider("Resolution", 0.0, 30, &resolution);

	gui->addSlider("Scale", 0.0, 10.0, &mScale);
	gui->addSlider("Smooth", 1.0, 200.0, &mSmooth);
	gui->addToggle("drawMEX", &mDrawMEX);
	gui->addToggle("drawMetaball", &mDrawMetaball);

	mDrawMEX = false;
	mDrawMetaball = true;
	resolution = 9;
	radius_low = 0.05;
	radius_high = 0.35;
	mScale = 1.0;
	mSmooth = 35.0;

	mex.setupControlPanel(this);
	mex.load("motionExt_"+getName()+".xml");
}

void distanceMetaball::update(){
	metaball.setup(resolution);
	metaball.setRadius(radius_low,radius_high);

	for (int i = 0;i < mex.getNumPort();i++){
		if (pts.size() <= i){
			pts.push_back(mex.getPositionAt(i)/400.0 + ofVec3f(0.5,0.5,0.5));
		}else{
			ofVec3f tg = mex.getPositionAt(i)/400.0 + ofVec3f(0.5,0.5,0.5);
			pts[i] += (tg - pts[i]) / mSmooth;
		}
	}

	while (pts.size() > mex.getNumPort()){
		pts.pop_back();
	}

	metaball.setCenters(pts);
	metaball.update();
	mex.update();

}

void distanceMetaball::draw(){

	ramBeginCamera();

	if (mDrawMEX) mex.draw();
	ofMesh m = metaball.getMesh();

	ofPushMatrix();

	glScaled(mScale, mScale, mScale);
	glScaled(400.0,400.0,400.0);
	ofTranslate(-0.5, -0.5, -0.5);

	if (mDrawMetaball){
		ofSetColor(0);
		m.draw(OF_MESH_FILL);
		ofSetColor(255);
		ofSetLineWidth(3.0);
		m.draw(OF_MESH_WIREFRAME);
		ofSetLineWidth(1.0);
	}

	ofPopMatrix();
	ramEndCamera();
}

void distanceMetaball::onPanelChanged(ofxUIEventArgs &e){

}

void distanceMetaball::drawActor(const ramActor &actor){

}

void distanceMetaball::onEnabled(){

}

void distanceMetaball::onDisabled(){

}