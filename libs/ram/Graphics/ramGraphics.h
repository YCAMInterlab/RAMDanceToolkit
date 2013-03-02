#pragma once

#include "ofMain.h"

#include "ramActor.h"

#pragma mark - ramColor

struct ramColor
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

class ramFloor
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

void ramBillboard();

//

inline void ramLine(const ramNode& node)
{
	if (!node.hasParent()) return;

	ofLine(node.getGlobalPosition(),
		   node.getParent()->getGlobalPosition());
}

void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

//

void ramDrawNodeCorresponds(const ramNodeArray &a, const ramNodeArray &b);

void ramDrawBasicFloor(const int floorPattern,
					   const float floorSize,
					   const float tileSize,
					   const ofColor& c1,
					   const ofColor& c2);

void ramDrawActorCube(ramNodeArray& nodeArray);

void ramDrawBasicActor(const ramActor& actor);
void ramDrawBasicRigid(const ramRigidBody& rigid);
void ramDrawNodes(const ramNodeArray& nodeArray);

//

void ramStripe(const vector<ramNode> &nodes);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11);
void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11, const ramNode& n12);