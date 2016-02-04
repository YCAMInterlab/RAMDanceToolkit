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
    gui->addSlider("xCompress", 0.0, 2.0, &xCompress);

	mDrawMEX = false;
	mDrawMetaball = true;
	resolution = 22;
	radius_low = 0.02;
	radius_high = 0.13;
	mScale = 1.87;
	mSmooth = 28.0;
    xCompress = 1.0;

	mex.setupControlPanel(this);
	mex.load("motionExt_"+getName()+".xml");
}

void distanceMetaball::update(){
	metaball.setup(resolution);
	metaball.setRadius(radius_low,radius_high);

	for (int i = 0;i < mex.getNumPort();i++){
        ofVec3f tg = mex.getPositionAt(i)/800.0 * ofVec3f(xCompress, 1.0, 1.0) + ofVec3f(0.5,0.5,0.5);
        
		if (pts.size() <= i){
			pts.push_back(tg);
		}else{
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