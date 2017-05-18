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
#include "ramCameraManager.h"

namespace rdtk{
	class SceneGui : public Unit{
	public:
		bool gui_floating = false;
		bool folding = false;
		
		void drawGuiBase();
		virtual void drawImGui();
	};
	
	class ActorGui{
		
	};
}

#endif /* ramImGuiPanel_hpp */
