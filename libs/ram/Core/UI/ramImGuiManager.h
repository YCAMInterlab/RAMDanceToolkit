//
//  ramImGui.hpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/01.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#ifndef ramImGui_hpp
#define ramImGui_hpp

#include "ramCameraManager.h"
#include "ramPreferencesTab.h"
#include "ramControlPanel.h"

namespace rdtk{
	
	class GuiManager{
	public:
		
		void draw();
		
		void addActor(ofPtr<ActorGui> act);
		void addScene(ofPtr<SceneGui> scn);
		void removeActorGui();
		void removeSceneGui();
		
		bool visible = true;
		int currentUIView = 0;
		
		void keyPressed(ofKeyEventArgs & key);
		
		inline static GuiManager& instance()
		{
			if (__instance == NULL)
			{
				__instance = new GuiManager;
				ofAddListener(ofEvents().keyPressed, __instance, &GuiManager::keyPressed);
			}
			return *__instance;
		}
		
	protected:
		vector< ofPtr<ActorGui> > actorArr;
		vector< ofPtr<SceneGui> > sceneArr;

		ofxImGui::Gui gui;
		
	private:
		static GuiManager *__instance;
		
		
		ofPtr<PreferencesGui> prefGui;
		GuiManager();
		GuiManager(const GuiManager&){}
		GuiManager& operator=(const GuiManager&){return *__instance;}
		~GuiManager();
	};
	
}

#endif /* ramImGui_hpp */
