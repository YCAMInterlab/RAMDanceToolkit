#pragma once

#include "ramNodeLine.h"

class Line : public ramBaseScene
{
	
public:
	
	ramNodeLine nodeLine;
	
	const string getName() { return "Line"; }
	
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
			cout << "random" << endl;
			
			nodeLine.from.index = ofRandom(23);
			nodeLine.control0.index = ofRandom(23);
			nodeLine.control1.index = ofRandom(23);
			nodeLine.to.index = ofRandom(23);
		}
	}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		
		panel->addButton("Line Width", &line_width, 1, 10);
		
		panel->addButton("Set From", &set_from, 30, 30);
		panel->addButton("Set Control 0", &set_control0, 30, 30);
		panel->addButton("Set Control 1", &set_control1, 30, 30);
		panel->addButton("Set To", &set_to, 30, 30);
		
		panel->addSlider("curve", -400, 400, &curve, 300, 20);

		panel->addSlider("spiral_radius", 0, 200, &spiral_radius, 300, 20);
		panel->addSlider("spiral_num_rotate", 0, 100, &spiral_num_rotate, 300, 20);

		panel->addSlider("noise_scale", 0, 200, &noise_scale, 300, 20);
		panel->addSlider("noise_freq", 0, 10, &noise_freq, 300, 20);
		
		panel->addSlider("extend", 0, 1000, &extend, 300, 20);

		ofAddListener(panel->newGUIEvent, this, &Line::guiEvent);
	}

	void setupControlPanel(ramControlPanel& gui)
	{
	}
	
	void update()
	{
		if (set_from) nodeLine.from = ramActorManager::instance().getLastSelectedNodeIdentifer();
		if (set_control0) nodeLine.control0 = ramActorManager::instance().getLastSelectedNodeIdentifer();
		if (set_control1) nodeLine.control1 = ramActorManager::instance().getLastSelectedNodeIdentifer();
		if (set_to) nodeLine.to = ramActorManager::instance().getLastSelectedNodeIdentifer();
	}
	
	void draw()
	{
		ramBeginCamera();
		
		ramNodeLine *NL = &nodeLine;
		
		nodeLine.curve(curve);
		
		if (extend > 0)
		{
			nodeLine.extend(extend);
		}

		nodeLine.resampling(0.3);
		
		if (spiral_radius > 0)
		{
			nodeLine.spiral(spiral_radius, spiral_num_rotate);
		}
		
		if (noise_scale > 0)
		{
			nodeLine.noise(noise_scale, noise_freq, ofGetElapsedTimef());
		}
		
		ofSetColor(255, 0, 0);
		
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
		
		ramEndCamera();
	}
	
	void guiEvent(ofxUIEventArgs &e)
	{
		string name = e.widget->getName();
		
	}
		
private:
	
};

