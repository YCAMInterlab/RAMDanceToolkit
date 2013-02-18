#pragma once

#include "ramNodeLine.h"

class Line : public ramBaseScene
{
	
public:
	
	const string getName() { return "Line"; }
	
	
	struct LineContext
	{
		bool set_from;
		bool set_control0;
		bool set_control1;
		bool set_to;
		
		float curve;
		
		float spiral_radius;
		float spiral_num_rotate;
		
		float noise_scale;
		float noise_freq;
		
		float extend;
		
		float line_width;
		ofFloatColor color;
		
		ramNodeLine nodeLine;
		
		bool active;
		int id;

		void setupControlPanel(ofxUICanvas* panel)
		{
			ramControlPanel &gui = ramGetGUI();
			
			line_width = 2;
			
			active = false;
			panel->addToggle("Line " + ofToString(id), &active, 30, 30, 0, 0);
			
			panel->addButton("From", &set_from, 15, 15, 0, 0);
			panel->addButton("Control0", &set_control0, 15, 15, 0, 80);
			panel->addButton("Control1", &set_control1, 15, 15, 0, 160);
			panel->addButton("To", &set_to, 15, 15, 0, 240);
			
			panel->addSlider("R", 0, 1, &color.r, 300, 10);
			panel->addSlider("G", 0, 1, &color.g, 300, 10);
			panel->addSlider("B", 0, 1, &color.b, 300, 10);
			
			panel->addSlider("line_width", 1, 20, &line_width, 300, 10);
			
			panel->addSlider("curve", -400, 400, &curve, 300, 10);
			
			panel->addSlider("spiral_radius", 0, 200, &spiral_radius, 300, 10);
			panel->addSlider("spiral_num_rotate", 0, 100, &spiral_num_rotate, 300, 10);
			
			panel->addSlider("noise_scale", 0, 200, &noise_scale, 300, 10);
			panel->addSlider("noise_freq", 0, 10, &noise_freq, 300, 10);
			
			panel->addSlider("extend", 0, 1000, &extend, 300, 10);
			
			panel->addSpacer(gui.kLength, 2);
		}
		
		void update()
		{
			if (!active) return;
			
			if (set_from) nodeLine.from = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control0) nodeLine.control0 = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control1) nodeLine.control1 = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_to) nodeLine.to = ramActorManager::instance().getLastSelectedNodeIdentifer();
		}
		
		void draw()
		{
			if (!active) return;
			
			nodeLine.curve(curve);
			nodeLine.resampling(0.3);
			
			if (extend > 0)
			{
				nodeLine.extend(extend);
			}
			
			if (spiral_radius > 0)
			{
				nodeLine.spiral(spiral_radius, spiral_num_rotate);
			}
			
			if (noise_scale > 0)
			{
				nodeLine.noise(noise_scale, noise_freq, ofGetElapsedTimef());
			}
			
			ofSetColor(color);
			
			ofSetLineWidth(line_width);
			nodeLine.draw();
			
			ofSetColor(255, 127);
			
			ramNode node;
			if (nodeLine.from.get(node))
			{
				ofDrawBitmapString("FROM", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control0.get(node))
			{
				ofDrawBitmapString("CP0", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control1.get(node))
			{
				ofDrawBitmapString("CP1", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.to.get(node))
			{
				ofDrawBitmapString("TO", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
		}
		
		void randomize()
		{
			nodeLine.from.index = ofRandom(23);
			nodeLine.control0.index = ofRandom(23);
			nodeLine.control1.index = ofRandom(23);
			nodeLine.to.index = ofRandom(23);
		}
	};
	
	enum
	{
		NUM_LINE = 2
	};
	
	LineContext lines[NUM_LINE];
	
	Line()
	{
	}
		
	void setup()
	{
		ofAddListener(ofEvents().keyPressed, this, &Line::onKeyPressed);
	}
	
	void onKeyPressed(ofKeyEventArgs &e)
	{
		if (e.key == 'r')
		{
			for (int i = 0; i < NUM_LINE; i++)
			{
				lines[i].randomize();
			}
		}
	}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].id = i;
			lines[i].setupControlPanel(panel);
		}
		
		lines[0].active = true;
	}

	void update()
	{
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].update();
		}
	}
	
	void draw()
	{
		ramBeginCamera();
		
		ofPushStyle();
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].draw();
		}
		ofPopStyle();
		
		ramEndCamera();
	}
	
private:
	
};

