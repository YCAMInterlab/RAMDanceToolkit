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
