// 
// ramGraphics.h - RAMDanceToolkit
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

#include "ramActor.h"

#pragma mark - ramColor

namespace rdtk{
	struct Color
	{
		static const ofColor RED_NORMAL;
		static const ofColor RED_DEEP;
		static const ofColor RED_LIGHT;
		static const ofColor GREEN_NORMAL;
		static const ofColor GREEN_DEEP;
		static const ofColor GREEN_LIGHT;
		static const ofColor BLUE_NORMAL;
		static const ofColor BLUE_DEEP;
		static const ofColor BLUE_LIGHT;
		static const ofColor YELLOW_NORMAL;
		static const ofColor YELLOW_DEEP;
		static const ofColor YELLOW_LIGHT;
		static const ofColor BLACK;
		static const ofColor DARK_GRAY;
		static const ofColor GRAY;
		static const ofColor LIGHT_GRAY;
		static const ofColor WHITE;
		static const ofColor DARK_GRAY_ALPHA;
		static const ofColor GRAY_ALPHA;
		static const ofColor LIGHT_GRAY_ALPHA;
		static const ofColor SHADOW;
	};

#pragma mark - ramGraphics
	
	class Floor
	{
		
	public:
		
		enum FloorName
		{
			FLOOR_NONE              = 0,
			FLOOR_GRID_LINES        = 1,
			FLOOR_PLANE             = 2,
			FLOOR_CHECKER_PATTERN   = 3,
			NUM_FLOOR_PATTERNS      = 4
		};
		
		static vector<string> getFloorNames()
		{
			vector<string> floorNames;
			floorNames.push_back("NONE");
			floorNames.push_back("GRID_LINES");
			floorNames.push_back("PLANE");
			floorNames.push_back("CHECKER_PATTERN");
			
			return floorNames;
		}
	};
	
	//
	
	void Billboard();
	
	//
	
	inline void Line(const Node& node)
	{
		if (!node.hasParent()) return;
		
		ofDrawLine(node.getGlobalPosition(),
				   node.getParent()->getGlobalPosition());
	}
	
	void Box(const Node& o, float size);
	void Sphere(const Node& o, float radius);
	
	//
	
	void DrawNodeCorresponds(const NodeArray &a, const NodeArray &b);
	
	void DrawBasicFloor(const int floorPattern,
						   const float floorSize,
						   const float tileSize,
						   const ofColor& c1,
						   const ofColor& c2);
	
	void DrawActorCube(NodeArray& nodeArray);
	
	void DrawBasicActor(const Actor& actor);
	void DrawBasicRigid(const RigidBody& rigid);
	void DrawNodes(const NodeArray& nodeArray);
	
	//
	
	void Stripe(const vector<Node> &nodes);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7, const Node& n8);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7, const Node& n8, const Node& n9);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7, const Node& n8, const Node& n9, const Node& n10);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7, const Node& n8, const Node& n9, const Node& n10,
				const Node& n11);
	void Stripe(const Node& n1, const Node& n2, const Node& n3, const Node& n4, const Node& n5,
				const Node& n6, const Node& n7, const Node& n8, const Node& n9, const Node& n10,
				const Node& n11, const Node& n12);
}

OF_DEPRECATED(void ramBillboard());
OF_DEPRECATED(void ramLine(const rdtk::Node& node));
OF_DEPRECATED(void ramBox(const rdtk::Node& o, float size));
OF_DEPRECATED(void ramSphere(const rdtk::Node& o, float radius));
OF_DEPRECATED(void ramDrawNodeCorresponds(const rdtk::NodeArray &a, const rdtk::NodeArray &b));
OF_DEPRECATED(void ramDrawBasicFloor(const int floorPattern,const float floorSize,
									 const float tileSize,const ofColor& c1,const ofColor& c2));
OF_DEPRECATED(void ramDrawActorCube(rdtk::NodeArray& nodeArray));
OF_DEPRECATED(void ramDrawBasicActor(const rdtk::RigidBody& rigid));
OF_DEPRECATED(void ramDrawNodes(const rdtk::NodeArray& nodeArray));

typedef rdtk::Floor RAMDEPRECATED(ramFloor);
typedef rdtk::Color RAMDEPRECATED(ramColor);
