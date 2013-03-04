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
	
	void setupControlPanel()
	{
		
		buffer_time = 3600;
		rate = 1.5;
		
		ramGetGUI().addSlider("buffer_time", 1, 10000, &buffer_time);
		ramGetGUI().addSlider("rate", -2, 3, &rate);
		
		ramGetGUI().addToggle("draw_line", &draw_line);
		ramGetGUI().addToggle("show box", &show_box);
		ramGetGUI().addToggle("fill chaser", &fill_chaser);
		ramGetGUI().addColorSelector("chaser color", &joint_color);
	}
	
	void setup()
	{
		show_box = false;
		fill_chaser = false;
		joint_color = ramColor::BLUE_NORMAL;
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		
	}
	
	void drawHUD()
	{

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
	
	void drawRigid(const ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Hasty Chase"; }
};

