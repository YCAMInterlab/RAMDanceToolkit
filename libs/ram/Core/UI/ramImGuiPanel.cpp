//
//  ramImGuiPanel.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/04.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#include "ramImGuiPanel.h"

using namespace rdtk;

void SceneGui::drawGuiBase()
{
	if (gui_floating)
	{
		ImGui::Begin(getName().c_str(), &gui_floating, ImGuiWindowFlags_AlwaysAutoResize);
		
		if (ImGui::IsWindowHovered())
			CameraManager::instance().setEnableInteractiveCamera(false);
		
		drawImGui();
		ImGui::End();
	}
	else
	{
		if (ImGui::CollapsingHeader(getName().c_str()))
		{
			drawImGui();
			ImGui::Separator();
			ImGui::Checkbox("Floating", &gui_floating);
		}
	}
}

void SceneGui::drawImGui()
{
	ImGui::LabelText("tex", "ui not implemented");
}
