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
	ramGetGUI().addSlider("offset_x", -500, 500, &offset.x);
	ramGetGUI().addSlider("offset_y", -500, 500, &offset.y);
	ramGetGUI().addSlider("offset_z", -500, 500, &offset.z);
}

void dp16_ramActorTranslator::update()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	
	for (int i = 0;i < am.getNumNodeArray();i++)
	{
		am.getNodeArray(i).setFix(fixer);
		am.getNodeArray(i).setOffset(offset);
	}
}

void dp16_ramActorTranslator::draw()
{
	
}