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

class Future : public ramBaseScene
{
	
	ramFilterEach<ramGhost> ghostFilters;
	ramFilterEach<ramLowPassFilter> lowPassFilters;
	
	float speed, distance;
	
public:
	
	bool draw_line;
	
	Future() : distance(80), speed(120) {}
	
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
		
		ramGetGUI().addToggle("Draw line from actor to ghost", &draw_line);
		
		ofAddListener(ramGetGUI().addButton("Speed: Ghost"), this, &Future::onPresetGhost);
		ofAddListener(ramGetGUI().addButton("Speed: Slow"), this, &Future::onPresetSlow);
		ofAddListener(ramGetGUI().addButton("Speed: Normal"), this, &Future::onPresetNormal);
		ofAddListener(ramGetGUI().addButton("Speed: Fast"), this, &Future::onPresetFast);
		
		ramGetGUI().addSlider("Distance", 0.0, 255.0, &distance);
		ramGetGUI().addSlider("Speed", 0.0, 255.0, &speed);
		
		ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &Future::onValueChanged);
	}

	void draw()
	{
		const vector<ramNodeArray>& NAs = ghostFilters.update(getAllNodeArrays());
		const vector<ramNodeArray>& lowPassedNAs = lowPassFilters.update(NAs);
		
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
			i==0 ? ofColor::green:
			i==1 ? ofColor::yellow: ofColor::red;
			
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
	
	void loadPreset(size_t preset_id=0)
	{
		for (int i=0; i<ghostFilters.getNumFilters(); i++)
		{
			ghostFilters.getFilter(i).setDistance(150);
			ghostFilters.getFilter(i).setSpeed(27);
		}
	}
	
    void onEnabled(){
        ramCameraManager::instance().getActiveCamera().setPosition(getRDTKSceneCameraPosition());
        ramCameraManager::instance().getActiveCamera().lookAt(getRDTKSceneCameraLookAt());
    }
	
	string getName() const { return "Future"; }
};


