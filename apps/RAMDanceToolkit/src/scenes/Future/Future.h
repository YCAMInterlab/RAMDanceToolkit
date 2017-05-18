// 
// Future.h - RAMDanceToolkit
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

#include "ParticleEngine.h"

class Future : public rdtk::BaseScene
{
	
	ramFilterEach<rdtk::Ghost> ghostFilters;
	ramFilterEach<rdtk::LowPassFilter> lowPassFilters;
	
	float speed, distance;
	
public:
	
    bool draw_line;
	
	Future() : distance(150), speed(27), draw_line(false) {}
	
	struct Preset
	{
		rdtk::Ghost *self;
		float distance;
		float speed;
		
		Preset(rdtk::Ghost *self, float distance, float speed) : self(self), distance(distance), speed(speed) {}
		void operator()()
		{
			self->setSpeed(speed);
			self->setDistance(distance);
		}
	};

	void setupControlPanel()
	{
		
		rdtk::GetGUI().addToggle("Draw line from actor to ghost", &draw_line);
		
		ofAddListener(rdtk::GetGUI().addButton("Speed: Ghost"), this, &Future::onPresetGhost);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Slow"), this, &Future::onPresetSlow);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Normal"), this, &Future::onPresetNormal);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Fast"), this, &Future::onPresetFast);
		
		rdtk::GetGUI().addSlider("Distance", 0.0, 255.0, &distance);
		rdtk::GetGUI().addSlider("Speed", 0.0, 255.0, &speed);
		
		ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &Future::onValueChanged);
	}
	
	void onValueChanged(ofxUIEventArgs &e)
	{
		updateFilters();
	}
	
	void drawImGui()
	{
		ofEventArgs dummy;
		if (ImGui::Checkbox("Draw line from actor to ghost", &draw_line)) updateFilters();
		if (ImGui::Button("speed: Ghost"))
		{
			onPresetGhost(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Slow"))
		{
			onPresetSlow(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Normal"))
		{
			onPresetNormal(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Fast"))
		{
			onPresetFast(dummy);
			updateFilters();
		}
		if (ImGui::DragFloat("Distance", &distance, 1, 0, 255)) updateFilters();
		if (ImGui::DragFloat("Speed", &speed, 1, 0, 255)) updateFilters();
		
	}

	void draw()
	{
		const vector<rdtk::NodeArray>& NAs = ghostFilters.update(getAllNodeArrays());
		const vector<rdtk::NodeArray>& lowPassedNAs = lowPassFilters.update(NAs);
		
		rdtk::BeginCamera();
		
		for(int i=0; i<lowPassedNAs.size(); i++)
		{
			
			const rdtk::NodeArray &NA = getNodeArray(i);
			const rdtk::NodeArray &processedNA = lowPassedNAs[i];
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			ofPushStyle();
			ofNoFill();
			
			const ofColor gcolor =
			i==0 ? rdtk::Color::RED_LIGHT :
			i==1 ? rdtk::Color::YELLOW_DEEP : rdtk::Color::BLUE_LIGHT;
			
			ofSetColor(gcolor);
			rdtk::DrawNodes(processedNA);
			
			if (draw_line)
			{
				ofSetColor(gcolor);
				rdtk::DrawNodeCorresponds(NA, processedNA);
			}
			
			ofPopStyle();
			glPopAttrib();
		}
		
		rdtk::EndCamera();
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
			rdtk::Ghost &filter = ghostFilters.getFilter(i);
			filter.setDistance(distance);
			filter.setSpeed(speed);
		}
	}
	
	void loadPreset(size_t preset_id=0)
	{
		for (int i=0; i<ghostFilters.getNumFilters(); i++)
		{
			ghostFilters.getFilter(i).setDistance(150);
			ghostFilters.getFilter(i).setSpeed(27);
		}
	}
	
	
	string getName() const { return "Future"; }
};


