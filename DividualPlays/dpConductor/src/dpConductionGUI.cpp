//
//  dpConductionGUI.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#include "dpConductionGUI.h"

void dpConductionGUI::setup()
{
	scenes_for_test.push_back("Line_ext");
	scenes_for_test.push_back("BigBox_ext");
	
	gui.setFontSize(OFX_UI_FONT_LARGE, 48);
	gui.setup();
	
	currentScene = gui.addDropDownList("Scene", scenes_for_test);
	gui.addToggle("Exclusive", &bExclusive);
	gui.addButton("Enable", false);
	gui.addButton("Disable", false);
	gui.addSpacer();
	
	ofAddListener(gui.newGUIEvent, this, &dpConductionGUI::event);
	
	currentScene->setAutoClose(true);
	
	
}

void dpConductionGUI::event(ofxUIEventArgs &e)
{
	ofxUIWidget* w = e.widget;
	
	if ((w == currentScene) && (currentScene->getSelectedNames().size() > 0))
	{
		currentScene->setLabelText(currentScene->getSelectedNames()[0]);
	}
	
	if (w->getName() == "Enable"  && w->getState() == OFX_UI_STATE_DOWN)
	{
		conPtr->sceneCon->setScene(currentScene->getSelectedNames()[0], true, true, true);
		conPtr->sceneCon->setScene(currentScene->getSelectedNames()[0], false, true, true);
	}
	if (w->getName() == "Disable" && w->getState() == OFX_UI_STATE_DOWN)
	{
		conPtr->sceneCon->disableScene(currentScene->getSelectedNames()[0], true);
		conPtr->sceneCon->disableScene(currentScene->getSelectedNames()[0], false);
	}
}