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
		
		float line_width;
		ofFloatColor color;
		
		ramGraphView *graphView;
		
		ramNodeFinder from;
		ramNodeFinder to;
		
		float min, max;
		
		void setup(int id, ramGraphView *graphView)
		{
			this->id = id;
			this->graphView = graphView;
			min = 0;
			max = 100;
		}
		
		void setupControlPanel(ofxUICanvas* panel)
		{
			panel->addButton("From", &set_from, 15, 15, 0, 0);
			panel->addButton("To", &set_to, 15, 15, 0, 240);
			
			panel->addSlider("line_width", 1, 20, &line_width, 300, 10);
			
			panel->addSlider("R", 0, 1, &color.r, 300, 10);
			panel->addSlider("G", 0, 1, &color.g, 300, 10);
			panel->addSlider("B", 0, 1, &color.b, 300, 10);
			
			panel->addSlider("min", 0, 500, &min, 300, 10);
			panel->addSlider("max", 0, 500, &max, 300, 10);
		}
		
		void update()
		{
			if (set_from) from = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_to) to = ramActorManager::instance().getLastSelectedNodeIdentifer();
			
			string name = ofToString(id);
			
			ramNode from_node, to_node;
			if (from.get(from_node) && to.get(to_node))
			{
				float d = from_node.getGlobalPosition().distance(to_node.getGlobalPosition());
				
				graphView->setMinMax(name, min, max);
				
				if (!graphView->hasRecord(name))
				{
					graphView->setupRecord(name, 0, 300);
				}
				
				graphView->update(name, d);
			}
		}
		
		
	};
	
	enum
	{
		NUM_RECORD = 3
	};
	
	ramGraphView graphView;
	Record records[NUM_RECORD];
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		for (int i = 0; i < NUM_RECORD; i++)
		{
			records[i].setup(i, &graphView);
			records[i].setupControlPanel(panel);
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
		graphView.draw();
	}
	
	void drawActor(ramActor& actor)
	{
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	const string getName() { return "Graph 2D"; }
};

