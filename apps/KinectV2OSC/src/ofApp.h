// 
// ofApp.h - KinectV2OSC
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

#include "ofMain.h"
#include "ofxKinect2.h"
#include "ofxOscSender.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		private:
		ofxKinect2::Device device_;
		ofxKinect2::BodyStream body_;
		ofxOscSender sender_;
	private:
		ofxImGui::Gui gui_;
		ofEasyCam preview_camera_;
		ofCamera virtual_camera_;

		bool view_from_virtual_camera_ = false;
		float preview_scale_ = 100;
		void save();
		void load();
};
