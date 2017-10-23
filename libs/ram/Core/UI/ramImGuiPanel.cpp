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
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5,0.5,0.5,1.0));
		ImGui::Begin(getName().c_str(), &gui_floating, ImGuiWindowFlags_AlwaysAutoResize);
		
		if (ImGui::IsWindowHovered())
			CameraManager::instance().setEnableInteractiveCamera(false);
		
		if (ImGui::Checkbox("Enable", &bEnabled)) setEnabled(bEnabled);

		drawImGui();
		
		ImGui::End();
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5,0.5,0.5,1.0));
		
		bool b_tmp = bEnabled;
		if (ImGui::Checkbox("", &b_tmp))
			setEnabled(b_tmp);
		
		ImGui::SameLine();
		bool collapse = ImGui::CollapsingHeader(getName().c_str());
		
		if (collapse)
		{
			if (!folding) folding = bEnabled = true;
			if (ImGui::Button("Float Window")) gui_floating = true;
			drawImGui();
			ImGui::Separator();
		}
		else
		{
			folding = false;
		}
		

		ImGui::PopStyleColor();
	}
}

void SceneGui::drawImGui()
{
	ImGui::LabelText("tex", "ui not implemented");
}
