#pragma once

#include "ofMain.h"

#include "ramActor.h"

#pragma mark - ramColor

class ramColor
{
public:
	
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
	static const ofColor GRAY;
	static const ofColor WHITE;
	static const ofColor SHADOW;
};

inline static const ofColor getRamColor(ofColor color)
{
	ofLogWarning("getRamColor is deprecated");
	return color;
}

#pragma mark - ramGraphics

inline void ofLine(const ramNode& o1, const ramNode& o2)
{
	ofLine(o1.getGlobalPosition(), o2.getGlobalPosition());
}

inline void ofBox(const ramNode& o, float size)
{
	ofBox(o.getGlobalPosition(), size);
}

inline void ofSphere(const ramNode& o, float radius)
{
	ofSphere(o.getGlobalPosition(), radius);
}

void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

void ramBasicActor(ramActor& actor,
				   const ofColor& jointColor = ramColor::BLUE_LIGHT,
				   const ofColor& lineColor = ramColor::GRAY);

void ramActorCube(ramActor& actor);

class ramFloor
{
public:
	
	enum FloorName
	{
		FLOOR_NONE				= 0,
		FLOOR_PLANE				= 1,
		FLOOR_CHECKER_PATTERN	= 2,
		FLOOR_GRID_LINES		= 3,
		NUM_FLOOR_PATTERNS		= 4
	};
	
	static vector<string> getFloorNames()
	{
		string name[NUM_FLOOR_PATTERNS] =
		{
			"NONE",
			"PLANE",
			"CHECKER_PATTERN",
			"GRID_LINES"
		};
		
		vector<string> floorNames;
		floorNames.clear();
		floorNames.resize(NUM_FLOOR_PATTERNS);
		
		for (int i=0; i<floorNames.size(); i++)
			floorNames.at(i) = name[i];
		
		return floorNames;
	}
};

void ramBasicFloor(const int floorPattern,
				   const float floorSize = 600.0,
				   const float tileSize = 50.0,
				   const ofColor& c1 = ofColor(60),
				   const ofColor& c2 = ofColor(230));

