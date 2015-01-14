//
//  ofxKsmrParallellinkDelta.cpp
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/07/18.
//
//

#include "ofxKsmrParallellinkDelta.h"

void ofxKsmrParallellinkDelta::setup(float Radius, float armLength,float arm2Length,float height,float plotRadius){

	plot_radius = plotRadius;
	radius		= Radius;
	arm_length	= armLength;
	arm2_length = arm2Length;

	for (int i = 0;i < 3;i++){
		actuator[i].setPosition(cos(ofDegToRad(90-i*120))*radius, height,
								sin(ofDegToRad(90-i*120))*radius);
		actuator[i].setOrientation(ofVec3f(0,i*120,0));

		arm[i].setPosition(ofVec3f(0,-armLength,0));
		arm[i].setParent(actuator[i]);
	}

	setPlot(ofVec3f(0,0,0));
}

void ofxKsmrParallellinkDelta::draw(){

	ofEnableDepthTest();
	for (int i = 0;i < 3;i++){
		ofSetColor(255);
		actuator[i].setScale(0.3);
		arm[i].setScale(0.3);
		actuator[i].draw();
		arm[i].draw();

		ofLine(actuator[i].getGlobalPosition(),
			   arm[i].getGlobalPosition());

		if (fabs(arm[i].getGlobalPosition().distance(plots[i]) - arm2_length/2) > 0.5) ofSetColor(255, 0, 0);
		else ofSetColor(255);
		ofLine(arm[i].getGlobalPosition(),
			   plots[i]);
	}

	ofSetColor(255);
	plot.setScale(0.5);
	plot.draw();
	ofDisableDepthTest();
	
}

void ofxKsmrParallellinkDelta::setPlot(ofVec3f pos){

	plot.setPosition(pos);

	for (int i = 0;i < 3;i++){
		plots[i].set(cos(ofDegToRad(90-i*120))*plot_radius, 0.0,
					 sin(ofDegToRad(90-i*120))*plot_radius);
		plots[i] += pos;

		float currentDist;
		const float degRes = 0.2;

		for (float deg = 0; deg < 90; deg+=degRes){

			actuator[i].setOrientation(ofVec3f(0,0,0));
			actuator[i].pan(i*120);
			actuator[i].tilt(-deg);

			currentDist = arm[i].getGlobalPosition().distance(plots[i]);

			if (currentDist > arm2_length/2){
				actuator[i].tilt(degRes);
				break;
			}

		}

	}

}