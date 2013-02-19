#pragma once

#include "ramMain.h"

#include "SphereMesh.h"
#include "GeometryHelpers.h"

class FourPoints : public ramBaseScene
{
public:
	
	ofxUIToggleMatrix *m4p1, *m4p2, *m4p3, *m4p4;
	
	bool showFourPointTwist, showFourPointSphere;
	float twistResolution, twistExtension, pointSize;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
    showFourPointSphere = false;
    showFourPointTwist = true;
    twistResolution = 20;
    twistExtension = 1;
		pointSize = 3;
		
		float dim = 20;
		float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
		float length = 320 - xInit;
		panel->addLabelToggle("Show four point sphere", &showFourPointSphere, length, dim);
		panel->addLabelToggle("Show four point twist", &showFourPointTwist, length, dim);
		panel->addSlider("Twist resolution", 2, 100, &twistResolution, length, dim);
		panel->addSlider("Twist extension", 1, 40, &twistExtension, length, dim);
		
		int matrixSize = 10;
		m4p1 = (ofxUIToggleMatrix*) panel->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.1"));
		m4p2 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.2"));
		m4p3 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.3"));
		m4p4 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.4"));
    m4p1->setAllowMultiple(false);
    m4p2->setAllowMultiple(false);
    m4p3->setAllowMultiple(false);
    m4p4->setAllowMultiple(false);
    m4p1->getToggles()[ramActor::JOINT_LEFT_HAND]->setValue(true);
    m4p2->getToggles()[ramActor::JOINT_RIGHT_HAND]->setValue(true);
    m4p3->getToggles()[ramActor::JOINT_LEFT_TOE]->setValue(true);
    m4p4->getToggles()[ramActor::JOINT_RIGHT_TOE]->setValue(true);
	}
	
	void setup()
	{		
		ofSetIcoSphereResolution(3);
	}
	
	void update()
	{
	}
	
	//--------------------------------------------------------------
	void draw()
	{	
	}
	
	//--------------------------------------------------------------
	int getChoice(ofxUIToggleMatrix* matrix) {
    vector<ofxUIToggle*> toggles = matrix->getToggles();
    for(int i = 0; i < toggles.size(); i++) {
			if(toggles[i]->getValue()) {
				return i;
			}
    }
    return 0;
	}
	
	//--------------------------------------------------------------
	void drawActor(ramActor &actor)
	{		
		// maybe this is slow...? need a better way to do point size/depth testing.
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPointSize(pointSize);
		glDisable(GL_DEPTH_TEST);
		
		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		
		ofEnableAlphaBlending();
		ofVec3f j1 = actor.getNode(getChoice(m4p1));
    ofVec3f j2 = actor.getNode(getChoice(m4p2));
    ofVec3f j3 = actor.getNode(getChoice(m4p3));
    ofVec3f j4 = actor.getNode(getChoice(m4p4));
    if(j1 != j2 && j1 != j3 && j1 != j4 && j2 != j3 && j2 != j4 && j3 != j4) {
			if(showFourPointSphere) {
				ofVec3f sphereCenter;
				float sphereRadius;
				findSphere(j1, j2, j3, j4, sphereCenter, sphereRadius);
				ofSetColor(255, 32);
				ofIcoSphere(sphereCenter, sphereRadius);
			}
			if(showFourPointTwist) {
				ofVec3f ap = (j1 + j2) / 2, bp = (j3 + j4) / 2;
				ofVec3f an = j2 - j1, bn = j4 - j3;
				float al = an.length(), bl = bn.length();
				ofQuaternion aq, bq;
				aq.makeRotate(ofVec3f(1, 0, 0), an.getNormalized());
				bq.makeRotate(ofVec3f(1, 0, 0), bn.getNormalized());
				ofSetColor(255, 192);
				for(int i = 0; i < twistResolution; i++) {
					float t = ofMap(i, 0, twistResolution - 1, 0, 1);
					ofPushMatrix();
					ofTranslate(ap.getInterpolated(bp, t));
					ofQuaternion q;
					q.slerp(t, aq, bq);
					ofMultMatrix(ofMatrix4x4(q));
					float radius = twistExtension * ofLerp(al, bl, t) / 2;
					ofLine(-radius, 0, radius, 0);
					ofPopMatrix();
				}
			}
    }
		
		ofPopStyle();
		glPopAttrib();
	}
	
	//--------------------------------------------------------------
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Four Points"; }
	
};
