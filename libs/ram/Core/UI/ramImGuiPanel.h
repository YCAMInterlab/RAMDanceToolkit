//
//  ramImGuiPanel.hpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/04.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#ifndef ramImGuiPanel_hpp
#define ramImGuiPanel_hpp

#include "ofMain.h"
#include "ofxImGui.h"
#include "ramUnit.h"

namespace rdtk{
	class SceneGui : public Unit{
	public:
		bool gui_floating = false;
		
		void drawGuiBase()
		{
			ImGui::Checkbox("Float", &gui_floating);
			drawImGui();
			
			if (gui_floating)
			{
				ImGui::Begin(getName().c_str(), &gui_floating);
//				if (ImGui::IsWindowHovered()) camEnable = false;
				drawImGui();
				ImGui::End();
			}
		}
		virtual void drawImGui() {}
	};
	
	class ActorGui{
		
	};
}

#endif /* ramImGuiPanel_hpp */
