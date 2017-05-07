
//  ramImGui.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/01.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#include "ramImGuiManager.h"

using namespace rdtk;

GuiManager* GuiManager::__instance = NULL;

GuiManager::GuiManager()
{
	gui.setup();
	prefGui = ofPtr<PreferencesGui>(new PreferencesGui());
	addScene(ofPtr<SceneGui>(prefGui));
}

GuiManager::~GuiManager()
{
}

void GuiManager::addActor(ofPtr<ActorGui> act)
{
	
}

void GuiManager::addScene(ofPtr<SceneGui> scn)
{
	sceneArr.push_back(scn);
}

void GuiManager::draw()
{
	gui.begin();
	CameraManager::instance().setEnableInteractiveCamera(!ImGui::GetWindowIsFocused());
	
	for (auto scn : sceneArr)
	{
		ImGui::PushID(scn->getName().c_str());
		scn->drawGuiBase();
		ImGui::PopID();
	}
	
	gui.end();
}
