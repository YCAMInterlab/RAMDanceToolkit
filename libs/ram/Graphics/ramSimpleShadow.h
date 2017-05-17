// 
// ramSimpleShadow.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
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

#include "ramGraphics.h"

namespace rdtk{
	class SimpleShadow
	{
	public:
		
		void setup();
		void setLightPosition(ofVec3f pos);
		void setShadowColor(ofFloatColor color);
		
		void begin();
		void end();
		
		void setEnable(bool v) { enable = v; }
		bool getEnable() const { return enable; }
		
		void setShadowAlpha(float alpha);
		
	protected:
		
		ofMatrix4x4 shadow_matrix;
		ofShader shader;
		
		bool enable;
		ofFloatColor shadow_color;
		
	};
}

typedef rdtk::SimpleShadow RAMDEPRECATED(ramSimpleShadow);
