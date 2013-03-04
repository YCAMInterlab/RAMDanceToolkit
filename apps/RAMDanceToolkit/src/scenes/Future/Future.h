#pragma once

#include "ParticleEngine.h"

class Future : public ramBaseScene
{
	
	ramFilterEach<ramGhost> ghostFilters;
	ramFilterEach<ramLowPassFilter> lowPassFilters;
	
	float speed, distance;
	
public:
	
	bool draw_line;
	
	Future() : distance(150), speed(27) {}
	
	struct Preset
	{
		ramGhost *self;
		float distance;
		float speed;
		
		Preset(ramGhost *self, float distance, float speed) : self(self), distance(distance), speed(speed) {}
		void operator()()
		{
			self->setSpeed(speed);
			self->setDistance(distance);
		}
	};

	void setupControlPanel()
	{
		
		gui().addToggle("Draw line from actor to ghost", &draw_line);
		
		ofAddListener(gui().addButton("Ghost"), this, &Future::onPresetGhost);
		ofAddListener(gui().addButton("Slow"), this, &Future::onPresetSlow);
		ofAddListener(gui().addButton("Normal"), this, &Future::onPresetNormal);
		ofAddListener(gui().addButton("Fast"), this, &Future::onPresetFast);
		
		gui().addSlider("Distance", 0.0, 255.0, &distance);
		gui().addSlider("Speed", 0.0, 255.0, &speed);
		
		ofAddListener(gui().getCurrentUIContext()->newGUIEvent, this, &Future::onValueChanged);
	}

	void draw()
	{
		const vector<ramNodeArray>& NAs = ghostFilters.update(getAllNodeArrays());
		const vector<ramNodeArray>& lowPassedNAs = ghostFilters.update(NAs);
		
		ramBeginCamera();
		
		for(int i=0; i<lowPassedNAs.size(); i++)
		{
			
			const ramNodeArray &NA = getNodeArray(i);
			const ramNodeArray &processedNA = lowPassedNAs[i];
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			ofPushStyle();
			ofNoFill();
			
			const ofColor gcolor =
			i==0 ? ramColor::RED_LIGHT :
			i==1 ? ramColor::YELLOW_DEEP : ramColor::BLUE_LIGHT;
			
			ofSetColor(gcolor);
			ramDrawNodes(processedNA);
			
			if (draw_line)
			{
				ofSetColor(gcolor);
				ramDrawNodeCorresponds(NA, processedNA);
			}
			
			ofPopStyle();
			glPopAttrib();
		}
		
		ramEndCamera();
	}
	
	void onPresetGhost(ofEventArgs &e)
	{
		speed = 1.5;
		distance = 240;
	}
	
	void onPresetSlow(ofEventArgs &e)
	{
		speed = 8.3;
		distance = 74.4;
	}
	
	void onPresetNormal(ofEventArgs &e)
	{
		speed = 9.4;
		distance = 150;
	}
	
	void onPresetFast(ofEventArgs &e)
	{
		speed = 38.9;
		distance = 211;
	}
	
	void updateFilters()
	{
		for(int i=0; i<ghostFilters.getNumFilters(); i++)
		{
			ramGhost &filter = ghostFilters.getFilter(i);
			filter.setDistance(distance);
			filter.setSpeed(speed);
		}
	}
	
	void onValueChanged(ofxUIEventArgs &e)
	{
		updateFilters();
	}
	
	const string getName() { return "Future"; }
};


