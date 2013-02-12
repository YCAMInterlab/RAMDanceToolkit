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


void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

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
				   const float floorSize,
				   const float tileSize,
				   const ofColor& c1,
				   const ofColor& c2);

void ramActorCube(ramActor& actor);
void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

void ramBasicActor(ramActor& actor,
				   const ofColor& jointColor = ramColor::BLUE_LIGHT,
				   const ofColor& lineColor = ramColor::GRAY);


void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<5; i++)
	{
		int to = (i==4) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}


void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<6; i++)
	{
		int to = (i==5) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<7; i++)
	{
		int to = (i==6) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<8; i++)
	{
		int to = (i==7) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<9; i++)
	{
		int to = (i==8) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<10; i++)
	{
		int to = (i==9) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition(), n11.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<11; i++)
	{
		int to = (i==10) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}

void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11, const ramNode& n12)
{
	ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition(), n11.getGlobalPosition(), n12.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<12; i++)
	{
		int to = (i==11) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}