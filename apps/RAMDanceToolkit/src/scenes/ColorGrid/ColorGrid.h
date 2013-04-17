// 
// ColorGrid.h - RAMDanceToolkit
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

class ColorGrid : public ramBaseScene
{
public:
	
	ofImage img;
	bool useRgb;
	float bufferSize;
	
	void setupControlPanel()
	{
		useRgb = true;
		bufferSize = 1024;
		
		ramGetGUI().addToggle("Use RGB/HSB", &useRgb);
		ramGetGUI().addSlider("Buffer size", 128, 2048, &bufferSize);
	}
	
	void setup()
	{		
	}
	
	void update()
	{
		bufferSize = (int) bufferSize;
		if(bufferSize != img.getWidth())
		{
			img.allocate((int) bufferSize, ramActor::NUM_JOINTS, OF_IMAGE_COLOR);
			for(int y = 0; y < ramActor::NUM_JOINTS; y++) {
				for(int x = 0; x < bufferSize; x++) {
					img.setColor(x, y, ofColor(0));
				}
			}
		}
		img.update();
	}
	
	//--------------------------------------------------------------
	void drawHUD()
	{	
		ofSetColor(255);
		ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
		img.draw(0, 0, ofGetViewportWidth(), ofGetViewportHeight());
	}
	
	//--------------------------------------------------------------
	void drawActor(const ramActor &actor)
	{
		for(int y = 0; y < img.getHeight(); y++) {
			for(int x = 0; x < img.getWidth() - 1; x++) {
				img.setColor(x, y, img.getColor(x + 1, y));
			}
		}
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			if(i < ramActor::NUM_JOINTS) {
				const ramNode &node = actor.getNode(i);
				ofColor cur;
				if(useRgb) {
					ofVec3f base = ofVec3f(1, 0, 0) * node.getOrientationQuat();
					cur = ofColor(ofMap(base.x, -1, 1, 0, 255, true),
												ofMap(base.y, -1, 1, 0, 255, true),
												ofMap(base.z, -1, 1, 0, 255, true));
				} else {
					ofVec3f euler = node.getOrientationEuler();
					ofVec3f base = ofVec3f(1, 0, 0) * node.getOrientationQuat();				
					cur = ofColor::fromHsb(ofMap(euler.z, -180, 180, 0, 255, true),
																 ofMap(base.x, -1, 1, 0, 255, true),
																 ofMap(base.y, -1, 1, 0, 255, true));
				}
				img.setColor(img.getWidth() - 1, i, cur);
//				
//				ofPushMatrix();
//				node.beginTransform();
//				ofFill();
//				ofBox((i==ramActor::JOINT_HEAD) ? 6 : 3);
//				ofSetColor(cur);
//				node.endTransform();
//				ofPopMatrix();
//				
//				if (node.hasParent())
//				{
//					ramNode* parent = node.getParent();
//					ofLine(node, *parent);
//				}
			}
		}
	}
	
	//--------------------------------------------------------------
	void drawRigid(const ramRigidBody &rigid)
	{
	}
	
	string getName() const { return "Color Grid"; }
	
};
