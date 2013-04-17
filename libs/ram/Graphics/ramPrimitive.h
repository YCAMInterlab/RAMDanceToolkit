// 
// ramPrimitive.h - RAMDanceToolkit
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
#include "ramPhysics.h"

class ramPhysics;

class ramBasePrimitive
{
	friend class ramPhysics;

public:

	virtual ~ramBasePrimitive() {}

private:

	virtual void internal_update() {}

};

class ramPrimitive : public ofNode, public ramBasePrimitive
{
	friend class ramPhysics;

public:

	ramPrimitive();
	~ramPrimitive();

	ofxBt::RigidBody getRigidBody() { return body; }

	void updatePhysicsTransform();

protected:

	ofxBt::RigidBody body;
	ofxBt::World& getWorld();

private:

	void internal_update();

};

class ramBoxPrimitive : public ramPrimitive
{
public:

	ramBoxPrimitive(float size = 100);
	ramBoxPrimitive(const ofVec3f& size);

	ramBoxPrimitive(const ofVec3f& pos, float size);
	ramBoxPrimitive(const ofVec3f& pos, const ofVec3f& size);

	ramBoxPrimitive(const ofMatrix4x4& mat, float size);
	ramBoxPrimitive(const ofMatrix4x4& mat, const ofVec3f& size);

protected:
	void customDraw();
};

class ramSpherePrimitive : public ramPrimitive
{
public:

	ramSpherePrimitive(float radius = 100);
	ramSpherePrimitive(const ofVec3f& pos, float radius = 100);

protected:
	void customDraw();
};

class ramCylinderPrimitive : public ramPrimitive
{
public:

	ramCylinderPrimitive(float radius = 100, float height = 100);
	ramCylinderPrimitive(const ofMatrix4x4& mat, float radius = 100, float height = 100);
	ramCylinderPrimitive(const ofVec3f& pos, float radius = 100, float height = 100);

protected:
	void customDraw();
};

class ramPyramidPrimitive : public ramPrimitive
{
public:

	ramPyramidPrimitive(float size = 100);
	ramPyramidPrimitive(const ofMatrix4x4& mat, float size = 100);
	ramPyramidPrimitive(const ofVec3f& pos, float size = 100);

protected:
	void customDraw();

	ofMesh mesh;
};