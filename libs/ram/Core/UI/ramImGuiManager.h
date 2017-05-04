//
//  ramImGui.hpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/01.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#ifndef ramImGui_hpp
#define ramImGui_hpp

#include "ramImGuiPanel.h"
#include "ramCameraManager.h"

namespace rdtk{
	
	class GuiManager{
	public:
		
		void draw();
		
		void addActor(ofPtr<ActorGui> act);
		void addScene(ofPtr<SceneGui> scn);
		void removeActorGui();
		void removeSceneGui();
		
		inline static GuiManager& instance()
		{
			if (__instance == NULL)
				__instance = new GuiManager;
			return *__instance;
		}
		
	protected:
		vector< ofPtr<ActorGui> > actorArr;
		vector< ofPtr<SceneGui> > sceneArr;

		ofxImGui::Gui gui;
		
	private:
		static GuiManager *__instance;
		
		GuiManager();
		GuiManager(const GuiManager&){}
		GuiManager& operator=(const GuiManager&){return *__instance;}
		~GuiManager();
	};
	
}

#endif /* ramImGui_hpp */
