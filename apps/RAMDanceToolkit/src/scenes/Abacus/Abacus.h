// 
// Abacus.h - RAMDanceToolkit
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

class Abacus : public rdtk::BaseScene
{
	
	const int box_size;
	const float margin;
	const float width;
	
public:
	
	Abacus() : box_size(20), margin(5.0), width(500.0) {}
	
	void setup()
	{
		
	}
	
	void draw()
	{
		rdtk::BeginCamera();
		
		glEnable(GL_DEPTH_TEST);
		
		int numNodeArrays = getNumNodeArray();
		
		for(int i=0; i<numNodeArrays; i++)
		{
			drawAbacus(getNodeArray(i), i, numNodeArrays);
		}
		
		rdtk::EndCamera();
	}
	
	string getName() const { return "Abacus"; }
	
	
	
#pragma mark - 
	
	
	
	void drawAbacus(rdtk::NodeArray& nodeArray, int index, int numNodeArrays)
	{
		const int numNodes = nodeArray.getNumNode();
		
		ofColor c1 = rdtk::Color::BLUE_DEEP;
		ofColor c2 = rdtk::Color::GREEN_DEEP;
		
		glTranslatef(0.0, -(margin+box_size) * numNodes / 2, 0.0);
		
		ofPushStyle();
		for(int i=0; i<numNodes; i++)
		{
			glPushMatrix();
			rdtk::Node& node = nodeArray.getNode(i);
			ofVec3f pos = node.getGlobalPosition();
			
			pos.y = i*margin + i*box_size;
			pos.z = 0;
			
			ofSetColor(i%2==0 ? c1 : c2);
			ofSetLineWidth(2);
			ofDrawLine(ofVec3f(-width, pos.y, 0), ofVec3f(width, pos.y, 0));
			
			glTranslatef(width*2/(numNodeArrays+1)*(index+1) - width, 0, 0);
			ofDrawBox(pos, box_size);
			
			glPopMatrix();
		}
		ofPopStyle();
	}
};
