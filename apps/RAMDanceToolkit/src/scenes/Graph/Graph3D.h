// 
// Graph3D.h - RAMDanceToolkit
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

#include "ramUpsideDown.h"

class Graph3D : public ramBaseScene
{
public:
	
	Graph3D() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel("No parameters", OFX_UI_FONT_MEDIUM));
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		
	}
	
	void drawHUD()
	{
		if (getNumNodeArray() == 0) return;
		
		ramNodeArray &arr = getNodeArray(0);
		
		{
			float ww = ofGetWidth() / 6;
			float hh = ofGetHeight() / 4;
			
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 6; x++)
				{
					int index = y * 6 + x;
					if (index >= 23) continue;
					
					ramNode &node = arr.getNode(index);
					
					ofPushView();
					ofCamera cam;
					
					cam.setPosition(0, 0, 500);
					cam.begin(ofRectangle(x * ww, y * hh, ww, hh));
					
					ofMatrix4x4 m = node.getGlobalOrientation();
					m.postMultTranslate(ofVec3f(ww / 2, hh / 2, 0));
					
					glPushMatrix();
					
					glMultMatrixf(m.getPtr());
					
					ofDrawAxis(100);
					
					ofNoFill();
					ofSetColor(255, 30);
					ofBox(100);
					
					glPopMatrix();
					
					string str = ramActor::getJointName(index);
					ofDrawBitmapString(str, 0, -200);
					
					cam.end();
					
					ofPopView();
				}
			}
		}
	}
	
	void drawActor(ramActor& actor)
	{
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	string getName() const { return "Graph 3D"; }
};

