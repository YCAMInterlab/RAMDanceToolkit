#pragma once

#include "ramMain.h"
#include "ramCameraManager.h"
#include "imgui.h"
#include "ramBaseScene.h"

class MirrorCamera : public rdtk::BaseScene
{
public:
	ofCamera *camera_=nullptr;
	int my_camera_index_=0;
	ofVec3f scale_=ofVec3f(1,1,1);
	int active_camera_cache_ = 0;
	ofxOscReceiver osc_;
	float mirror_width_=300;
	float mirror_pos_=0;
	float camera_shift_;
	string getName() const { return "MirrorCamera"; }
	
	void drawImGui()
	{
		ImGui::DragFloat("mirror pos", &mirror_pos_);
		ImGui::SliderFloat("camera shift", &camera_shift_, -1,1);
		ImGui::DragFloat("mirror width", &mirror_width_);
		ImGui::DragFloat3("scale", &scale_[0], 0.01f);
	}
	
	void onEnabled() {
		auto &cm = rdtk::CameraManager::instance();
		active_camera_cache_ = cm.getActiveCameraIndex();
		cm.setActiveCamera(my_camera_index_);
		BaseScene::onEnabled();
	}
	
	void onDisabled() {
		auto &cm = rdtk::CameraManager::instance();
		cm.setActiveCamera(active_camera_cache_);
		BaseScene::onDisabled();
	}

	void setup()
	{
		auto &cm = rdtk::CameraManager::instance();
		my_camera_index_ = cm.getNumCameras();
		camera_ = cm.createCamera<ofCamera>();
	}
	
	void update()
	{
		ofVec3f actor_pos;
		if([this](ofVec3f &actor_pos) {
			const rdtk::Actor *actor = nullptr;
			size_t num_node_arrays = rdtk::ActorManager::instance().getNumNodeArray();
			for(int i = 0; i < num_node_arrays; ++i) {
				const rdtk::NodeArray &node = rdtk::ActorManager::instance().getNodeArray(i);
				if(node.isActor()) {
					actor = static_cast<const rdtk::Actor*>(&node);
				}
			}
			if(actor) {
				actor_pos = actor->getNode(rdtk::Actor::JOINT_HEAD).getGlobalPosition();
				return true;
			}
			return false;
		}(actor_pos)) {
			ofVec3f camera_pos = actor_pos;
			camera_pos.z = actor_pos.z-(actor_pos.z-mirror_pos_)*2;
			camera_->setLensOffset(ofVec2f(0,0));
			camera_->setPosition(camera_pos);
			camera_->lookAt(actor_pos);
			ofVec3f origin_screen_pos = camera_->worldToScreen(ofVec3f(0,0,0));
			float lens_offset = ofMap(origin_screen_pos.x, 0, ofGetWidth(), -1, 1);
			camera_->setLensOffset(ofVec2f(lens_offset,camera_shift_));
			{
				ofVec2f a = ofVec2f(-mirror_width_/2.f,0) - ofVec2f(camera_pos.x,camera_pos.z);
				ofVec2f b = ofVec2f(mirror_width_/2.f,0) - ofVec2f(camera_pos.x,camera_pos.z);
				camera_->setFov(b.angle(a));
			}
		}
	}
};
