#pragma once

#include "ramNodeLine.h"

class LineDrawing : public ramBaseScene
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
		
		float extend_from;
		float extend_to;
		
		float line_width;
		ofFloatColor color;
		
		ramNodeLine nodeLine;
		
		bool active;
		int id;

		void setupControlPanel()
		{
			ramControlPanel &gui = ramGetGUI();
			
#ifdef RAM_GUI_SYSTEM_OFXUI
			
			ofxUICanvas* panel = gui.getCurrentUIContext();
			
			line_width = 2;
			
			active = false;
			panel->addToggle("Line " + ofToString(id), &active, 15, 15, 0, 0);
			
			panel->addButton("From", &set_from, 10, 10, 0, 0);
			panel->addButton("Control0", &set_control0, 10, 10, 0, 80);
			panel->addButton("Control1", &set_control1, 10, 10, 0, 160);
			panel->addButton("To", &set_to, 10, 10, 0, 240);
			
			panel->addSlider("Line R", 0, 1, &color.r, 95, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("Line G", 0, 1, &color.g, 95, 10);
			panel->addSlider("Line B", 0, 1, &color.b, 95, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("line_width", 1, 10, &line_width, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("curve", -400, 400, &curve, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("spiral_radius", 0, 200, &spiral_radius, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("spiral_num_rotate", 0, 100, &spiral_num_rotate, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("noise_scale", 0, 200, &noise_scale, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("noise_freq", 0, 10, &noise_freq, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("extend from", 0, 1000, &extend_from, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("extend to", 0, 1000, &extend_to, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSpacer(gui.kLength, 2);
			
#endif
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
			
			if (extend_from > 0)
			{
				nodeLine.extendFrom(extend_from);
			}
			
			if (extend_to > 0)
			{
				nodeLine.extendTo(extend_to);
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
			ramActorManager &AM = ramActorManager::instance();
			const vector<string>& names = AM.getNodeArrayNames();
			
			nodeLine.from.index = ofRandom(23);
			nodeLine.from.name = names[rand() % names.size()];
			
			nodeLine.control0.index = ofRandom(23);
			nodeLine.control0.name = names[rand() % names.size()];
			
			nodeLine.to.index = ofRandom(23);
			nodeLine.to.name = names[rand() % names.size()];
			
			nodeLine.control1.index = ofRandom(23);
			nodeLine.control1.name = names[rand() % names.size()];
		}
	};
	
	enum
	{
		NUM_LINE = 3
	};
	
	LineContext lines[NUM_LINE];
	float random_change_time;
	float last_changed_time;
	
	LineDrawing()
	{
	}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();
		
		random_change_time = 60;
		gui.addSlider("random_change_time", 0.1, 60, &random_change_time);
		last_changed_time = ofGetElapsedTimef();
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].id = i;
			lines[i].setupControlPanel();
		}
		
		lines[0].active = true;
	}
	
	void setup()
	{
		ofAddListener(ofEvents().keyPressed, this, &LineDrawing::onKeyPressed);
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
	
	void update()
	{
		if (random_change_time < 60
			&& ofGetElapsedTimef() - last_changed_time > random_change_time)
		{
			last_changed_time = ofGetElapsedTimef();
			
			for (int i = 0; i < NUM_LINE; i++)
			{
				lines[i].randomize();
			}
		}
		
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

