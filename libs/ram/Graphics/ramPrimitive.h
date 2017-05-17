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

namespace rdtk{
	class Physics;
	
	class BasePrimitive
	{
		friend class Physics;
		
	public:
		
		virtual ~BasePrimitive() {}
		
	private:
		
		virtual void internal_update() {}
		
	};
	
	class Primitive : public ofNode, public BasePrimitive
	{
		friend class Physics;
		
	public:
		
		Primitive();
		~Primitive();
		
		ofxBt::RigidBody getRigidBody() { return body; }
		
		void updatePhysicsTransform();
		
	protected:
		
		ofxBt::RigidBody body;
		ofxBt::World& getWorld();
		
	private:
		
		void internal_update();
		
	};
	
	class BoxPrimitive : public Primitive
	{
	public:
		
		BoxPrimitive(float size = 100);
		BoxPrimitive(const ofVec3f& size);
		
		BoxPrimitive(const ofVec3f& pos, float size);
		BoxPrimitive(const ofVec3f& pos, const ofVec3f& size);
		
		BoxPrimitive(const ofMatrix4x4& mat, float size);
		BoxPrimitive(const ofMatrix4x4& mat, const ofVec3f& size);
		
	protected:
		void customDraw();
	};
	
	class SpherePrimitive : public Primitive
	{
	public:
		
		SpherePrimitive(float radius = 100);
		SpherePrimitive(const ofVec3f& pos, float radius = 100);
		
	protected:
		void customDraw();
	};
	
	class CylinderPrimitive : public Primitive
	{
	public:
		
		CylinderPrimitive(float radius = 100, float height = 100);
		CylinderPrimitive(const ofMatrix4x4& mat, float radius = 100, float height = 100);
		CylinderPrimitive(const ofVec3f& pos, float radius = 100, float height = 100);
		
	protected:
		void customDraw();
	};
	
	class PyramidPrimitive : public Primitive
	{
	public:
		
		PyramidPrimitive(float size = 100);
		PyramidPrimitive(const ofMatrix4x4& mat, float size = 100);
		PyramidPrimitive(const ofVec3f& pos, float size = 100);
		
	protected:
		void customDraw();
		
		ofMesh mesh;
	};
}

typedef rdtk::BasePrimitive			RAMDEPRECATED(ramBasePrimitive);
typedef rdtk::Primitive				RAMDEPRECATED(ramPrimitive);
typedef rdtk::BoxPrimitive			RAMDEPRECATED(ramBoxPrimitive);
typedef rdtk::SpherePrimitive		RAMDEPRECATED(ramSpherePrimitive);
typedef rdtk::CylinderPrimitive		RAMDEPRECATED(ramCylinderPrimitive);
typedef rdtk::PyramidPrimitive		RAMDEPRECATED(ramPyramidPrimitive);
