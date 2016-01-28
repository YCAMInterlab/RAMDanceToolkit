//
//  ThreePoints_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#include "ThreePoints_ext.h"


const ofColor ThreePoints_ext::cyanPrint = ofColor::fromHex(0x00abec);
const ofColor ThreePoints_ext::magentaPrint = ofColor::fromHex(0xec008c);
const ofColor ThreePoints_ext::yellowPrint = ofColor::fromHex(0xffee00);

void ThreePoints_ext::setupControlPanel()
{
#ifdef RAM_GUI_SYSTEM_OFXUI

	ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();

	showRects = false;
	showSpheres = false;
	invertSpheres = false;
	showCircleBisector = false;
	showCenterCircles = false;
	crossLength = 0;
	rectRadius = 0;
	pointSize = 3;
	maxInvertRadius = 2000;
	circleResolution = 30;

	ramGetGUI().addToggle("Show spheres", &showSpheres);
	ramGetGUI().addToggle("Show rects", &showRects);
	ramGetGUI().addToggle("Show circle", &showCircle);
	ramGetGUI().addToggle("Inverted spheres", &invertSpheres);
	ramGetGUI().addToggle("Show circle bisector", &showCircleBisector);
	ramGetGUI().addToggle("Show center circles", &showCenterCircles);
	panel->addSlider("Point size", 1, 10, &pointSize);
    panel->addSlider("Cross length", 1, 1000, &crossLength, 200.f, 20.f);
    panel->addSlider("Rect radius", 1, 1000, &rectRadius, 200.f, 20.f);
    panel->addSlider("Max invert radius", 1, 10000, &maxInvertRadius, 200.f, 20.f);
    panel->addSlider("Circle resolution", 3, 30, &circleResolution, 200.f, 20.f);
    
    mex.setupControlPanel(this);

#endif
}

void ThreePoints_ext::setup()
{
	_ofSetIcoSphereResolution(3);
}

void ThreePoints_ext::update()
{
    mex.update();
}

//--------------------------------------------------------------
void ThreePoints_ext::draw()
{
}

//--------------------------------------------------------------
void ThreePoints_ext::drawActor(const ramActor &actor)
{
	// maybe this is slow...? need a better way to do point size/depth testing.
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPointSize(pointSize);
	glDisable(GL_DEPTH_TEST);

	ofPushStyle();
	ofNoFill();
	ofSetColor(0);

	ofEnableAlphaBlending();
	ofSetCircleResolution(circleResolution);

	for (int i = 0; i < mex.getNumPort(); i++) {
        const ramNode &node = mex.getNodeAt(i);

		ofPushMatrix();
		node.beginTransform();
		ofSetColor(yellowPrint);

		if (showRects) {
			ofPushStyle();
			ofFill();
			ofSetColor(255, 128);
			ofRect(0, 0, 100, 30);
			ofPopStyle();
		}

		ofSetColor(255);
		node.endTransform();
		ofPopMatrix();

		if (node.hasParent()) {
			ramNode* parent = node.getParent();

			if (parent->hasParent()) {
				ramNode* grandparent = parent->getParent();
				ofVec3f a = node, b = *parent, c = *grandparent;
				ofVec3f normal = (a - b).cross(c - b);
				normal.normalize();
				ofSetColor(magentaPrint);
				ofLine(b - normal * crossLength, b + normal * crossLength);

				ofVec3f center;
				float radius;
				findCircle(a, b, c, center, normal, radius);
				ofPushMatrix();
				ofTranslate(center);
				rotateToNormal(normal);
				if (showSpheres) {
					ofSetColor(255, 32);
					ofIcoSphere(ofVec3f(), radius);
				}
				ofSetColor(yellowPrint);
				if (showCircle) {
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
				if (showCircleBisector) {
					ofLine(b, b + (center - b) * 2);
				}

				ofPushMatrix();
				ofTranslate(b);
				rotateToNormal(normal);
				ofSetColor(cyanPrint);
				ofRect(-rectRadius, -rectRadius, 2 * rectRadius, 2 * rectRadius);
				if (invertSpheres) {
					ofSetColor(255, 32);
					ofIcoSphere(ofVec3f(), maxInvertRadius / radius);
				}
				if (showCenterCircles) {
					ofSetColor(yellowPrint);
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
			}
		}
	}

	ofPopStyle();
	glPopAttrib();
}