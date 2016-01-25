//
//  FourPoints_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "FourPoints_ext.h"

void FourPoints_ext::setupControlPanel()
{
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	gui->addSlider("resolution", 1, 50, &twistResolution);
	gui->addSlider("width", 0.0, 10.0, &twistExtensionWidth);
	gui->addSlider("height", 0.0, 10.0, &twistExtensionHeight);
	
	twistResolution = 20;
	twistExtensionWidth = 1;
	twistExtensionHeight = 1;

	mex.setupControlPanel(this);
}

void FourPoints_ext::update()
{
	mex.update();

}

void FourPoints_ext::draw()
{
	if (ramGlobalShortcut::getNumNodeArray() < 1) return;

	ramBeginCamera();
	
	ofPushStyle();
	ofNoFill();
	ofSetColor(0);
	
	for (int i = 0;i < (mex.getNumPort() / 4);i++)
	{
		ofEnableAlphaBlending();
		ramNodeArray & actor = ramGlobalShortcut::getActorManager().getNodeArray(0);
		ofVec3f j1 = mex.getNodeAt(0 + i * 4);
		ofVec3f j2 = mex.getNodeAt(1 + i * 4);
		ofVec3f j3 = mex.getNodeAt(2 + i * 4);
		ofVec3f j4 = mex.getNodeAt(3 + i * 4);
		if(j1 != j2 && j1 != j3 && j1 != j4 && j2 != j3 && j2 != j4 && j3 != j4) {
			ofVec3f ap = (j1 + j2) / 2, bp = (j3 + j4) / 2;
			ofVec3f an = j2 - j1, bn = j4 - j3;
			float al = an.length(), bl = bn.length();
			ofQuaternion aq, bq;
			aq.makeRotate(ofVec3f(1, 0, 0), an.getNormalized());
			bq.makeRotate(ofVec3f(1, 0, 0), bn.getNormalized());
			ofSetColor(255, 192);
			for(int i = 0; i < twistResolution; i++) {
				float t = ofMap(i, 0, twistResolution - 1, 1 - twistExtensionHeight, 0 + twistExtensionHeight);
				ofPushMatrix();
				ofTranslate(ap.getInterpolated(bp, t));
				ofQuaternion q;
				q.slerp(t, aq, bq);
				ofMultMatrix(ofMatrix4x4(q));
				float radius = twistExtensionWidth * ofLerp(al, bl, t) / 2;
				ofLine(-radius, 0, radius, 0);
				ofPopMatrix();
			}
		}
	}
	
	ofPopStyle();
	
	mex.draw();
	ramEndCamera();
}