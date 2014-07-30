// 
// ThreePoints.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ramMain.h"

#include "SphereMesh.h"
#include "ramGeometry.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

class ThreePoints : public ramBaseScene
{
public:
	
	ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	bool showRects, showCircle, showSpheres, invertSpheres, showCircleBisector, showCenterCircles;
	float pointSize, crossLength, rectRadius, maxInvertRadius, circleResolution;
	
	void setupControlPanel()
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
		ramGetGUI().addSlider("Point size", 1, 10, &pointSize);
		ramGetGUI().addSlider("Cross length", 1, 1000, &crossLength);
		ramGetGUI().addSlider("Rect radius", 1, 1000, &rectRadius);
		ramGetGUI().addSlider("Max invert radius", 1, 10000, &maxInvertRadius);
		ramGetGUI().addSlider("Circle resolution", 3, 30, &circleResolution);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_LEFT_TOE || i == ramActor::JOINT_RIGHT_TOE);
			mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
#endif
	}
	
	void setup()
	{		
		_ofSetIcoSphereResolution(3);
	}
	
	void update()
	{
	}
	
	//--------------------------------------------------------------
	void draw()
	{	
	}
	
	//--------------------------------------------------------------
	void drawActor(const ramActor &actor)
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
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			
			ofPushMatrix();
			node.beginTransform();
			ofSetColor(yellowPrint);
			
			if(showRects && mToggles[i]->getValue()) {
				ofPushStyle();
				ofFill();
				ofSetColor(255, 128);
				ofRect(0, 0, 100, 30);
				ofPopStyle();
			}
			
			ofSetColor(255);
			node.endTransform();
			ofPopMatrix();
			
			if (node.hasParent())
			{
				ramNode* parent = node.getParent();
				
				if(parent->hasParent() && mToggles[i]->getValue())
				{
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
					if(showSpheres) {
						ofSetColor(255, 32);
						ofIcoSphere(ofVec3f(), radius);
					}
					ofSetColor(yellowPrint);
					if(showCircle) {
						ofCircle(0, 0, radius);
					}
					ofPopMatrix();
					if(showCircleBisector) {
						ofLine(b, b + (center - b) * 2);
					}
					
					ofPushMatrix();
					ofTranslate(b);
					rotateToNormal(normal);
					ofSetColor(cyanPrint);
					ofRect(-rectRadius, -rectRadius, 2*rectRadius, 2*rectRadius);
					if(invertSpheres) {
						ofSetColor(255, 32);
						ofIcoSphere(ofVec3f(), maxInvertRadius / radius);
					}
					if(showCenterCircles) {
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
	
	//--------------------------------------------------------------
	void drawRigid(const ramRigidBody &rigid)
	{
	}
	
	string getName() const { return "Three Points"; }
	
};
