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

class ThreePoints : public rdtk::BaseScene
{
public:
	
	ofxUIToggle *mToggles[rdtk::Actor::NUM_JOINTS];
	bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
	
	bool showRects, showCircle, showSpheres, invertSpheres, showCircleBisector, showCenterCircles;
	float pointSize, crossLength, rectRadius, maxInvertRadius, circleResolution;
	
	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();

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
		
		rdtk::GetGUI().addToggle("Show spheres", &showSpheres);
		rdtk::GetGUI().addToggle("Show rects", &showRects);
		rdtk::GetGUI().addToggle("Show circle", &showCircle);
		rdtk::GetGUI().addToggle("Inverted spheres", &invertSpheres);
		rdtk::GetGUI().addToggle("Show circle bisector", &showCircleBisector);
		rdtk::GetGUI().addToggle("Show center circles", &showCenterCircles);
		rdtk::GetGUI().addSlider("Point size", 1, 10, &pointSize);
		rdtk::GetGUI().addSlider("Cross length", 1, 1000, &crossLength);
		rdtk::GetGUI().addSlider("Rect radius", 1, 1000, &rectRadius);
		rdtk::GetGUI().addSlider("Max invert radius", 1, 10000, &maxInvertRadius);
		rdtk::GetGUI().addSlider("Circle resolution", 3, 30, &circleResolution);
		
		for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == rdtk::Actor::JOINT_LEFT_TOE || i == rdtk::Actor::JOINT_RIGHT_TOE);
			mToggles[i] = panel->addToggle(rdtk::Actor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
#endif
	}
	
	void drawImGui()
	{
		ImGui::Checkbox("Show spheres", &showSpheres);
		ImGui::Checkbox("Show rects", &showRects);
		ImGui::Checkbox("Show circles", &showCircle);
		ImGui::Checkbox("Inverted spheres", &invertSpheres);
		ImGui::Checkbox("Show circle bisector", &showCircleBisector);
		ImGui::Checkbox("Show center circles", &showCenterCircles);
		ImGui::DragFloat("Point size", &pointSize, 1, 1, 10);
		ImGui::DragFloat("Cross length", &crossLength, 1, 1, 1000);
		ImGui::DragFloat("Rect radius", &rectRadius, 1, 1, 1000);
		ImGui::DragFloat("Max invert radius", &maxInvertRadius, 10, 1, 10000);
		ImGui::DragFloat("Circle resolution", &circleResolution);
		
		ImGui::Spacing();
		ImGui::Columns(3, NULL, true);
		for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
		{
			ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
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
	void drawActor(const rdtk::Actor &actor)
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
			const rdtk::Node &node = actor.getNode(i);
			
			ofPushMatrix();
			node.beginTransform();
			ofSetColor(yellowPrint);
			
			if(showRects && mToggles[i]->getValue()) {
				ofPushStyle();
				ofFill();
				ofSetColor(255, 128);
				ofDrawRectangle(0, 0, 100, 30);
				ofPopStyle();
			}
			
			ofSetColor(255);
			node.endTransform();
			ofPopMatrix();
			
			if (node.hasParent())
			{
				rdtk::Node* parent = node.getParent();
				
				if(parent->hasParent() && mToggles[i]->getValue())
				{
					rdtk::Node* grandparent = parent->getParent();
					ofVec3f a = node, b = *parent, c = *grandparent;
					ofVec3f normal = (a - b).cross(c - b);
					normal.normalize();
					ofSetColor(magentaPrint);
					ofDrawLine(b - normal * crossLength, b + normal * crossLength);
					
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
						ofDrawCircle(0, 0, radius);
					}
					ofPopMatrix();
					if(showCircleBisector) {
						ofDrawLine(b, b + (center - b) * 2);
					}
					
					ofPushMatrix();
					ofTranslate(b);
					rotateToNormal(normal);
					ofSetColor(cyanPrint);
					ofDrawRectangle(-rectRadius, -rectRadius, 2*rectRadius, 2*rectRadius);
					if(invertSpheres) {
						ofSetColor(255, 32);
						ofIcoSphere(ofVec3f(), maxInvertRadius / radius);
					}
					if(showCenterCircles) {
						ofSetColor(yellowPrint);
						ofDrawCircle(0, 0, radius);
					}
					ofPopMatrix();
				}
			}
		}
		
		ofPopStyle();
		glPopAttrib();
	}
	
	//--------------------------------------------------------------
	void drawRigid(const rdtk::RigidBody &rigid)
	{
	}
	
	string getName() const { return "Three Points"; }
	
};
