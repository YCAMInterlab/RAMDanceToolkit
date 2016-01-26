//
//  dp16_ramActorTranslator.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "dp16_ramActorTranslator.h"

void dp16_ramActorTranslator::setupControlPanel()
{
	ramGetGUI().addIntSlider("fix_pos", -1, 22, &fixer);
	ramGetGUI().addToggle("floorStay", &floorStay);
	ramGetGUI().addSlider("offset_x", -500, 500, &offset.x);
	ramGetGUI().addSlider("offset_y", -500, 500, &offset.y);
	ramGetGUI().addSlider("offset_z", -500, 500, &offset.z);
	ramGetGUI().addSeparator();
	ramGetGUI().addToggle("autoLayout", &autoLayout);
	ramGetGUI().addSlider("margine", 0, 1000, &margine);
	
	floorStay = true;
	autoLayout = true;
	margine = 160;
}

void dp16_ramActorTranslator::update()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	
	int numArray = am.getNumNodeArray();
	
	if (!autoLayout)
	{
		for (int i = 0;i < am.getNumNodeArray();i++)
		{
			ramNodeArray & act = am.getNodeArray(i);

			act.setFloor(floorStay);
			act.setFix(fixer);
			
			float floor_y = 0;
			for (int j = 0;j < am.getNodeArray(i).getNumNode();j++)
			{
				ramNode &nd = act.getNode(j);
				floor_y = MIN(floor_y, nd.getGlobalPosition().y);
			}
			
			floor_y = act.getNode(0).getGlobalPosition().y;
			
			act.setOffset(offset);
			
		}
	}
	else
	{
		for (int i = 0;i < am.getNumNodeArray();i++)
		{
			ramNodeArray & act = am.getNodeArray(i);

			act.setFloor(floorStay);
			act.setFix(fixer);
			
			float x = margine * i - (margine * (numArray - 1)) / 2.0;
			am.getNodeArray(i).setFix(fixer);
			am.getNodeArray(i).setOffset(offset + ofVec3f(x,0,0));
		}
	}
}

void dp16_ramActorTranslator::draw()
{
	
}

void dp16_ramActorTranslator::exit()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	for (int i = 0;i < am.getNumNodeArray();i++)
	{
		am.getNodeArray(i).setFix(-1);
		am.getNodeArray(i).setOffset(ofVec3f(0,0,0));
	}
}