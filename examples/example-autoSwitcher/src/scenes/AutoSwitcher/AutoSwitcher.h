// 
// AutoSwitcher.h - RAMDanceToolkit examples
// 
// Copyright 2018 nariakiiwatani
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "imgui.h"
#include "ramBaseScene.h"

class AutoSwitcher : public rdtk::BaseScene
{
public:
	void addScene(ofPtr<rdtk::BaseScene> scene) {
		scenes_.push_back(scene);
		scenes_enabled_.resize(scenes_.size());
	}
	string getName() const { return "AutoSwitcher"; }
	
	void drawImGui()
	{
		ImGui::SliderFloat("interval", &interval_, 0, 30);
		ImGui::SliderFloat("timer", &timer_, 0, interval_);
		if(ImGui::TreeNode("on/off switch")) {
			for(int i = 0, num = scenes_.size(); i < num; ++i) {
				bool flag = scenes_enabled_[i];
				if(ImGui::Checkbox(scenes_[i]->getName().c_str(), &flag)) {
					scenes_enabled_[i] = flag;
				}
			}
			ImGui::TreePop();
		}
	}
	
	void update()
	{
		float timef = ofGetLastFrameTime();
		timer_ += timef;
		if(timer_ >= interval_) {
			if(nextScene()) {
				timer_ = 0;
			}
		}
	}

	private:
	ofPtr<rdtk::BaseScene> getScene(int index) {
		if(0 <= index && index < scenes_.size()) {
			return scenes_enabled_[index] ? scenes_[index] : nullptr;
		}
		return nullptr;
	};
	bool nextScene() {
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(false);
		}
		if(++active_scene_index_ >= scenes_.size()) {
			active_scene_index_ = 0;
		}
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(true);
			return true;
		}
		return false;
	}
	
	float interval_=20;
	float timer_=0;
	
	std::vector<ofPtr<rdtk::BaseScene>> scenes_;
	std::vector<int> scenes_enabled_;
	int active_scene_index_=-1;
};
