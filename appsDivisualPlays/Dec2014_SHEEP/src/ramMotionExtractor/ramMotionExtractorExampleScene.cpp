//
//  ramMotionExtractorExampleScene.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/05.
//
//

#include "ramMotionExtractorExampleScene.h"

ramMotionExtractorExampleScene::ramMotionExtractorExampleScene(){

	mDrawLines		= false;
	mDrawTriangle	= false;
	mDrawPreview	= true;
	mDrawDump		= true;

}

void ramMotionExtractorExampleScene::update(){

	/*=== update ===*/
	motionExtractor.update();

}

void ramMotionExtractorExampleScene::draw(){

	ramBeginCamera();

	/*=== Preview selected nodes ===*/
	if (mDrawPreview)	motionExtractor.draw();

	/*=== Example drawing with motionExtractor ===*/
	if (mDrawLines)		example_drawLines();
	if (mDrawTriangle)	example_drawTriangles();

	ramEndCamera();

	if (mDrawDump)		example_drawDump();

}

void ramMotionExtractorExampleScene::example_drawDump(){

	ofPushMatrix();
	ofTranslate(700, 10);

	for (int i = 0;i < motionExtractor.getNumPort();i++){
		ofPushMatrix();
		ofTranslate(0, i*90);

		ofVec3f vec = motionExtractor.getVelocityAt(i);
		float speed = motionExtractor.getVelocitySpeedAt(i);

		ofNoFill();
		ofRect(0, 0, 200, 80);
		ofFill();

		string info = "";
		info += "Port  :" + ofToString(i) + "\n";
		info += "Actor :" + motionExtractor.getActorNameAt(i) + "\n";
		info += "Joint :" + motionExtractor.getJointNameAt(i) + "\n";
		info += "Speed :" + ofToString(motionExtractor.getVelocitySpeedAt(i)) + "\n";

		ofSetColor(50);
		ofRect(10, 45, motionExtractor.getVelocitySpeedAt(i)*10.0, 15);

		ofSetColor(255);
		ofDrawBitmapString(info, 10, 15);


		ofPopMatrix();
	}

	ofPopMatrix();

}

void ramMotionExtractorExampleScene::example_drawLines(){

	for (int i = 0;i < motionExtractor.getNumPort() - 1;i+=2){

		ofVec3f vec_a = motionExtractor.getPositionAt(i);
		ofVec3f vec_b = motionExtractor.getPositionAt(i);

		ofLine(vec_a, vec_b);
	}

}

void ramMotionExtractorExampleScene::example_drawTriangles(){

	ofVec3f vec_a = motionExtractor.getPositionAt(0);
	ofVec3f vec_b = motionExtractor.getPositionAt(1);
	ofVec3f vec_c = motionExtractor.getPositionAt(2);

	ofSetLineWidth(3.0);
	ofNoFill();
	ofTriangle(vec_a, vec_b, vec_c);
	ofFill();
	ofSetLineWidth(1.0);

}

void ramMotionExtractorExampleScene::setupControlPanel(){

	ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();

	gui->addToggle("Preview"	, &mDrawPreview);
	gui->addToggle("Dump"		, &mDrawDump);

	gui->addSpacer();
	gui->addToggle("Lines"		, &mDrawLines);
	gui->addToggle("Triangle"	, &mDrawTriangle);


	/*=== register scene's pointer ===*/

	motionExtractor.setupControlPanel(this);
	
}