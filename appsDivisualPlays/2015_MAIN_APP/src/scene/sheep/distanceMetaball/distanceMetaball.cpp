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
	gui->addToggle("FaceFlip", &mFlipFace);
	gui->addToggle("pointDraw", &mPoints);
	gui->addToggle("Test",&mTest);
	
	mFlipFace = true;
	mPoints = true;
	mDrawMEX = false;
	mDrawMetaball = true;
	resolution = 9;
	radius_low = 0.05;
	radius_high = 0.35;
	mScale = 3.0;
	mSmooth = 35.0;

	mex.setupControlPanel(this);
	mex.load("motionExt_"+getName()+".xml");
}

void distanceMetaball::update(){
	metaball.setup(resolution);
	metaball.setRadius(radius_low,radius_high);

	for (int i = 0;i < mex.getNumPort();i++){
		if (pts.size() <= i){
			pts.push_back(mex.getPositionAt(i,true)/400.0 + ofVec3f(0.5,0.5,0.5));
		}else{
			ofVec3f tg = mex.getPositionAt(i,true)/400.0 + ofVec3f(0.5,0.5,0.5);
			pts[i] += (tg - pts[i]) / mSmooth;
		}
	}

	while (pts.size() > mex.getNumPort()){
		pts.pop_back();
	}
	
	if (mTest){
		pts.clear();
		for (int i = 0;i < 10;i++){
			if (ofGetKeyPressed('a')){
				pts.push_back(ofVec3f(ofNoise(i*34.325+ofGetElapsedTimef()/34.13)*2 - 0.5,
									  ofNoise(i*54.54+ofGetElapsedTimef()/34.13)*2 - 0.5,
									  ofNoise(i*65.724+ofGetElapsedTimef()/34.13)*2 - 0.5));
			}else{
				pts.push_back(ofVec3f(ofNoise(i*34.325)*2 - 0.5,
									  ofNoise(i*54.54)*2 - 0.5,
									  ofNoise(i*65.724)*2 - 0.5));

			}
		}
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
	ofRotateY(ofGetElapsedTimef()*2.0);

	glScaled(mScale, mScale, mScale);
	glScaled(400.0,400.0,400.0);
	ofTranslate(-0.5, -0.6, -0.5);

	if (mDrawMetaball){
		
		glBegin(GL_TRIANGLES);
		for (int i = 0;i < m.getNumVertices();i+=3){
			
			if ((ofNoise(i*243.4235+ofGetElapsedTimef()/10.0) < 0.3) &&
				mFlipFace){
				ofSetColor(dpColor::MAIN_COLOR);
			}else{
				ofSetColor(0, 0, 0);
			}
			
			for (int j = 0;j < 3;j++){
				glVertex3d(m.getVertex(i+j).x,
						   m.getVertex(i+j).y,
						   m.getVertex(i+j).z);
			}
		}
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		ofSetLineWidth(4.0);
		glBegin(GL_TRIANGLES);
		ofSetColor(255);
		for (int i = 0;i < m.getNumVertices();i+=3){
			
			for (int j = 0;j < 3;j++){
				glVertex3d(m.getVertex(i+j).x,
						   m.getVertex(i+j).y,
						   m.getVertex(i+j).z);
			}

		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		ofSetColor(dpColor::MAIN_COLOR);
		if (mPoints){
			for (int i = 0;i < m.getNumVertices();i++){
				ofPushMatrix();
				ofTranslate(m.getVertex(i));
				float sc = 1.0 / 1200.0 * 5;
				glScaled(sc,sc,sc);
				glutSolidIcosahedron();
				ofPopMatrix();
			}			
		}
		
		ofSetColor(dpColor::DARK_PINK_HEAVY);
		
		for (int i = 0;i < pts.size();i++){
			for (int j = 0;j < pts.size();j++){
//					ofLine(pts[i], pts[j]);
			}
		}
		
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