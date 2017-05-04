//
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
	bool cl = ImGui::CollapsingHeader("Actors");
	ImGui::SameLine();ImGui::Checkbox("Toggle", &cl);
	if (cl)
	{
		ImGui::Button("Load Recorded File");
//		ImGui::Button(")
		
	}
	if (ImGui::CollapsingHeader("Presets"))
	{
		
	}
	if (ImGui::CollapsingHeader("Preferences"))
	{
		
	}
	
	for (auto scn : sceneArr)
	{
		if (ImGui::CollapsingHeader(scn->getName().c_str()))
		{
			scn->drawGuiBase();
		}
	}
	
	gui.end();
}
