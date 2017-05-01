// 
// ramSoftBodyPrimitive.cpp - RAMDanceToolkit
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

#include "ramSoftBodyPrimitive.h"

using namespace rdtk;

SoftBodyPrimitive::SoftBodyPrimitive()
{
	Physics::instance().registerSoftBodyPrimitive(this);
}

SoftBodyPrimitive::~SoftBodyPrimitive()
{
	Physics::instance().unregisterSoftBodyPrimitive(this);
}

ofxBt::SoftBodyWorld& SoftBodyPrimitive::getWorld()
{
	return Physics::instance().getSoftBodyWorld();
}

void SoftBodyPrimitive::internal_update() {}

//

RopePrimitive::RopePrimitive(const ofVec3f p0, const ofVec3f p1)
{
	body = getWorld().addRope(p0, p1);
}
