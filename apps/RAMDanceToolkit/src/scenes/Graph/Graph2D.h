// 
// Graph2D.h - RAMDanceToolkit
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

class Graph2D : public ramBaseScene
{
public:
	
	class Record
	{
	public:
		
		int id;
		
		bool set_from;
		bool set_to;
		
		ramGraphView *graphView;
		
		ramNodeFinder from;
		ramNodeFinder to;
		
		float min, max;
		ramNode from_node, to_node;
		
		ofColor color;
		
		bool use_height;
		
		void setup(int id, ramGraphView *graphView)
		{
			this->id = id;
			use_height = false;
			this->graphView = graphView;
			min = 0;
			max = 100;
		}
		
		void setupControlPanel(ofxUICanvas* panel)
		{
			panel->addButton("From", &set_from, 15, 15, 0, 0);
			panel->addButton("To", &set_to, 15, 15, 0, 240);
			
			panel->addSlider("min", 0, 500, &min, 300, 10);
			panel->addSlider("max", 0, 500, &max, 300, 10);
			
			panel->addToggle("use_height", &use_height, 15, 15);
		}
		
		void update()
		{
			if (set_from) from = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_to) to = ramActorManager::instance().getLastSelectedNodeIdentifer();
			
			string name = ofToString(id);
			
			if (from.get(from_node) && to.get(to_node))
			{
				if (use_height)
				{
					float d = to_node.getGlobalPosition().y;
					
					graphView->setMinMax(name, min, max);
					
					if (!graphView->hasRecord(name))
					{
						graphView->setupRecord(name, 0, 300);
					}
					
					graphView->update(name, d);
				}
				else
				{
					float d = from_node.getGlobalPosition().distance(to_node.getGlobalPosition());
					
					graphView->setMinMax(name, min, max);
					
					if (!graphView->hasRecord(name))
					{
						graphView->setupRecord(name, 0, 300);
					}
					
					graphView->update(name, d);
				}
				
				graphView->getColor(name, color);
			}
		}
		
		void draw()
		{
			ofSetColor(color);
			
			if (use_height)
			{
				ofVec3f yy = to_node.getGlobalPosition();
				yy.y = 0;
				ofLine(yy, to_node.getGlobalPosition());
			}
			else
			{
				ofLine(from_node.getGlobalPosition(), to_node.getGlobalPosition());
			}
		}
		
	};
	
	enum
	{
		NUM_RECORD = 3
	};
	
	float line_width;
	ramGraphView graphView;
	Record records[NUM_RECORD];
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		panel->addSlider("line_width", 1, 20, &line_width, 300, 10);
		
		for (int i = 0; i < NUM_RECORD; i++)
		{
			records[i].setup(i, &graphView);
			records[i].setupControlPanel(panel);
			panel->addSpacer(gui.kLength, 2);
		}
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		float m = 100;
		graphView.setRect(ofRectangle(m, m, ofGetViewportWidth() - m * 2, ofGetViewportHeight() - m * 2));
		
		for (int i = 0; i < NUM_RECORD; i++)
		{
			records[i].update();
		}
	}
	
	void draw()
	{
		ofPushStyle();
		ofSetLineWidth(line_width);
		
		ramBeginCamera();
		for (int i = 0; i < 2; i++)
		{
			records[i].draw();
		}
		ramEndCamera();
		ofPopStyle();
	}
	
	void drawHUD()
	{
		ofPushStyle();
		ofSetLineWidth(line_width);

		graphView.draw();
		
		ofPopStyle();
	}
	
	void drawActor(ramActor& actor)
	{
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	string getName() const { return "Graph 2D"; }
};

