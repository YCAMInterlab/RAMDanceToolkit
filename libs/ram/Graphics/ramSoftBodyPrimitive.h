// 
// ramSoftBodyPrimitive.h - RAMDanceToolkit
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

#include "ramPrimitive.h"

#include "ofxBtSoftBodyWorld.h"

namespace rdtk{
	class SoftBodyPrimitive : public BasePrimitive
	{
		friend class Physics;
		
	public:
		
		SoftBodyPrimitive();
		~SoftBodyPrimitive();
		
		ofxBt::SoftBody getSoftBody() const { return body; }
		
	protected:
		
		ofxBt::SoftBody body;
		ofxBt::SoftBodyWorld& getWorld();
		inline const ofxBt::SoftBodyWorld& getWorld() const
		{
			return const_cast<ofxBt::SoftBodyWorld&>(getWorld());
		}
		
		void internal_update();
	};
	
	class RopePrimitive : public SoftBodyPrimitive
	{
	public:
		
		RopePrimitive(const ofVec3f p0, const ofVec3f p1);
		
	protected:
		
		ofNode controlPoint[2];
		
	};
}

typedef rdtk::SoftBodyPrimitive RAMDEPRECATED(ramSoftBodyPrimitive);
typedef rdtk::RopePrimitive RAMDEPRECATED(ramRopePrimitive);
