#pragma once

#include "ofMain.h"
#include "ramActor.h"
#include <cstdarg>


void ramActorCube(ramActor& actor);
void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

void ramBasicFloor(const int floorPattern,
				   const float floorSize = 600.0,
				   const float tileSize = 50.0,
				   const ofColor& c1 = ofColor(60),
				   const ofColor& c2 = ofColor(230));

void ramBasicActor(ramActor& actor, float* matrixPtr = NULL);
void ramBasicActor(ramActor& actor,
				   ofColor jointColor = ramColor::BLUE_LIGHT,
				   ofColor lineColor = ramColor::GRAY,
				   float* matrixPtr = NULL);


void ramPlate(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5)
{
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition(), n8.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition(), n8.getPosition(), n9.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition(), n8.getPosition(), n9.getPosition(), n10.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition(), n8.getPosition(), n9.getPosition(), n10.getPosition(), n11.getPosition() };
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
	ofVec3f v[] = { n1.getPosition(), n2.getPosition(), n3.getPosition(), n4.getPosition(), n5.getPosition(), n6.getPosition(), n7.getPosition(), n8.getPosition(), n9.getPosition(), n10.getPosition(), n11.getPosition(), n12.getPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINES);
	for (int i=0; i<12; i++)
	{
		int to = (i==11) ? 0 : i+1;
		glVertex3f(v[i].x, v[i].y, v[i].z); glVertex3f(v[to].x, v[to].y, v[to].z);
	}
	glEnd();
}