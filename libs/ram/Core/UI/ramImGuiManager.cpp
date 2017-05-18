
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
	//Load external font
//	ImGuiIO& io = ImGui::GetIO();
//	
//	if (io.Fonts->AddFontFromFileTTF(ofToDataPath("ARIALUNI.TTF").c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesChinese()))
//	{
//		unsigned char * pixels;
//		int width, height;
//		io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
//	}

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
