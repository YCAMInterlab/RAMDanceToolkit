// 
// HastyChase.h - RAMDanceToolkit
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

class HastyChase : public ramBaseScene
{
public:
	
	map<string, ramTimeShifter> time_shifters;
	float buffer_time;
	float rate;
	bool draw_line;
	bool show_box;
	bool fill_chaser;
	ofFloatColor joint_color;
	
	ofxUIToggle
		*drawLineToggle,
		*showBoxToggle,
		*fillChaserToggle;
	
	void setupControlPanel()
	{
		drawLineToggle = new ofxUIToggle("draw line", &draw_line, 20, 20);
		showBoxToggle = new ofxUIToggle("show box", &show_box, 20, 20);
		fillChaserToggle = new ofxUIToggle("fill chaser", &fill_chaser, 20, 20);
		
		ramGetGUI().addSlider("buffer_time", 1, 10000, &buffer_time);
		ramGetGUI().addSlider("rate", -2, 3, &rate);
		
		ramGetGUI().getCurrentUIContext()->addWidgetDown(drawLineToggle);
		ramGetGUI().getCurrentUIContext()->addWidgetDown(showBoxToggle);
		ramGetGUI().getCurrentUIContext()->addWidgetDown(fillChaserToggle);
		ramGetGUI().addColorSelector("chaser color", &joint_color);
	}
	
	void setup()
	{
		buffer_time = 3600;
		rate = 1.5;
		show_box = false;
		fill_chaser = false;
		joint_color = ramColor::BLUE_NORMAL;
	}
	
	void drawActor(const ramActor& actor)
	{
		ramTimeShifter &TS = time_shifters[actor.getName()];
		TS.setNumBufferFrame(buffer_time);
		TS.setRate(rate);

		ofPushStyle();
		if (fill_chaser)
		{
			ofFill();
		}
		else{
			ofNoFill();
		}
		
		const ramActor &chaser = TS.update(actor);
		
		ofSetColor(joint_color);
		ramDrawBasicActor(chaser);
		
		if (draw_line)
			ramDrawNodeCorresponds(actor, chaser);
		
		if (show_box)
		{
			
			ofSetColor(255, 127);
			ofNoFill();
			
			for (int i = 0; i < chaser.getNumNode(); i++)
			{
				const ramNode &node = chaser.getNode(i);
				ramBox(node, node.getVelocity().length() * 2);
			}
		}
		ofPopStyle();
	}
	
	void loadPreset(size_t preset_id=0)
	{
		buffer_time = 3600;
		rate = 1.5;
		draw_line = false;
		show_box = false;
		fill_chaser = false;
		
		if (preset_id == 1)
		{
			draw_line = true;
		}

		drawLineToggle->setValue(draw_line);
		drawLineToggle->stateChange();
		showBoxToggle->setValue(show_box);
		showBoxToggle->stateChange();
		fillChaserToggle->setValue(fill_chaser);
		fillChaserToggle->stateChange();
	}

	string getName() const { return "Hasty Chase"; }
};

