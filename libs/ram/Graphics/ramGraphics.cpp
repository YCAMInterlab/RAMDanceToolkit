// 
// ramGraphics.cpp - RAMDanceToolkit
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

#include "ramBaseApp.h"
#include "ramGraphics.h"
#include "ramPhysics.h"
#include "ramPrimitive.h"
#include "ramUtils.h"

#pragma mark - ramColor

const ofColor ramColor::RED_NORMAL      = ofColor::fromHex(0xff6666);
const ofColor ramColor::RED_DEEP        = ofColor::fromHex(0x993333);
const ofColor ramColor::RED_LIGHT       = ofColor::fromHex(0xff9898);

const ofColor ramColor::GREEN_NORMAL    = ofColor::fromHex(0x66cc33);
const ofColor ramColor::GREEN_DEEP      = ofColor::fromHex(0x339900);
const ofColor ramColor::GREEN_LIGHT     = ofColor::fromHex(0x99cc99);

const ofColor ramColor::BLUE_NORMAL     = ofColor::fromHex(0x0099cc);
const ofColor ramColor::BLUE_DEEP       = ofColor::fromHex(0x003366);
const ofColor ramColor::BLUE_LIGHT      = ofColor::fromHex(0x99cccc);

const ofColor ramColor::YELLOW_NORMAL   = ofColor::fromHex(0xffcc00);
const ofColor ramColor::YELLOW_DEEP     = ofColor::fromHex(0xcc9900);
const ofColor ramColor::YELLOW_LIGHT    = ofColor::fromHex(0xffff00);

const ofColor ramColor::BLACK           = ofColor::fromHex(0x000000);
const ofColor ramColor::DARK_GRAY       = ofColor::fromHex(0x444444);
const ofColor ramColor::GRAY            = ofColor::fromHex(0x888888);
const ofColor ramColor::LIGHT_GRAY      = ofColor::fromHex(0xbbbbbb);
const ofColor ramColor::WHITE           = ofColor::fromHex(0xffffff);

const ofColor ramColor::DARK_GRAY_ALPHA = ofColor(255, 64);
const ofColor ramColor::GRAY_ALPHA      = ofColor(255, 128);
const ofColor ramColor::LIGHT_GRAY_ALPHA= ofColor(255, 192);

const ofColor ramColor::SHADOW          = ofColor(0, 0, 0, 60);

//

void ramBox(const ramNode& o, float size)
{
	o.beginTransform();
	ofDrawBox(size);
	o.endTransform();

	if (ramGetEnablePhysicsPrimitive()
		&& ramPhysics::instance().checkAndUpdateNodeCache(&o))
	{
		ramBoxPrimitive *p = new ramBoxPrimitive(o.getGlobalTransformMatrix(), size);
		p->getRigidBody().setKinematic(true);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	o.beginTransform();
	ofDrawSphere(radius);
	o.endTransform();

	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getGlobalPosition(), radius);
		p->getRigidBody().setKinematic(true);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

//

void ramDrawBasicFloor(const int floorPattern,
					   const float floorSize,
					   const float tileSize,
					   const ofColor& c1,
					   const ofColor& c2)
{
	if (floorPattern == ramFloor::FLOOR_NONE) return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(4.0, 1.0);

	int division = floorSize / tileSize;

	ofPushStyle();
	ofSetLineWidth(1.0);

	ofPushMatrix();
	ofRotate(90.0f, 1.0f, 0.0f, 0.0f);

	if (ofGetRectMode() != OF_RECTMODE_CENTER)
	{
		float w = division * tileSize;
		ofTranslate(-w / 2.0f, -w / 2.0f);
	}

	glNormal3f(0.0f, 1.0f, 0.0f);

	if (floorPattern == ramFloor::FLOOR_PLANE)
	{
		ofFill();
		ofSetColor(c1);
		ofRect(0, 0, 0, division * tileSize, division * tileSize);
	}

	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			switch (floorPattern)
			{

			case ramFloor::FLOOR_CHECKER_PATTERN:
				ofFill();
				ofSetColor((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1) ? c1 : c2);
				ofRect(i * tileSize, j * tileSize, tileSize, tileSize);
				break;

			case ramFloor::FLOOR_GRID_LINES:
				ofNoFill();
				ofSetColor(c1);
				ofRect(i * tileSize, j * tileSize, tileSize, tileSize);
				break;
			}
		}
	}

	ofPopMatrix();
	ofPopStyle();

	glPopAttrib();
}

void ramDrawBasicActor(const ramActor& actor)
{
	if (actor.getNumNode() == 0) return;

	assert(actor.getNumNode() == ramActor::NUM_JOINTS);

	ofPushStyle();
	const ofColor& color0 = ofGetStyle().color;
	const ofColor& color1 = ofColor::fromHsb(((int)color0.getHue() + 30) % 255, color0.getSaturation() * 0.5, color0.getBrightness(), color0.a);

	glPushMatrix();
	for (int i = 0; i < actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;

		ofPushStyle();
		ofSetLineWidth(2);

		ofSetColor(color0);
		ramBox(node, jointSize);

		ofSetColor(color1);
		ramLine(node);

		ofPopStyle();
	}
	glPopMatrix();

	ofPopStyle();
}

void ramDrawBasicRigid(const ramRigidBody& rigid)
{
	if (rigid.getNumNode() == 0) return;

	for (int i = 0; i < rigid.getNumNode(); i++)
	{
		const ramNode &node = rigid.getNode(i);
		ramBox(node, 5);
	}
}

void ramDrawNodes(const ramNodeArray& nodeArray)
{
	if (nodeArray.getNumNode() == 0) return;

	if (nodeArray.isActor())
		ramDrawBasicActor((ramActor &)nodeArray);
	else
		ramDrawBasicRigid((ramRigidBody &)nodeArray);
}

void ramDrawActorCube(ramNodeArray& nodeArray)
{
	if (nodeArray.getNumNode() == 0) return;

	ofVec3f maxPos = nodeArray.getNode(ramActor::JOINT_CHEST).getGlobalPosition();
	ofVec3f minPos = nodeArray.getNode(ramActor::JOINT_CHEST).getGlobalPosition();

	for (int j = 0; j < nodeArray.getNumNode(); j++)
	{
		ofVec3f pos = nodeArray.getNode(j).getGlobalPosition();

		if (maxPos.x <= pos.x) maxPos.x = pos.x;
		if (maxPos.y <= pos.y) maxPos.y = pos.y;
		if (maxPos.z <= pos.z) maxPos.z = pos.z;

		if (minPos.x > pos.x) minPos.x = pos.x;
		if (minPos.y > pos.y) minPos.y = pos.y;
		if (minPos.z > pos.z) minPos.z = pos.z;
	}

	ofVec3f scale, axis;
	scale = (maxPos - minPos);
	axis = (maxPos + minPos) / 2;

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(axis.x, axis.y, axis.z);
		ofScale(scale.x, scale.y, scale.z);
		ofNoFill();
		ofDrawBox(1);
	}
	ofPopMatrix();
	ofPopStyle();
}

void ramDrawNodeCorresponds(const ramNodeArray &a, const ramNodeArray &b)
{
	if (a.getNumNode() == 0) return;

	if (b.getNumNode() == 0) return;

	assert(a.getNumNode() == b.getNumNode());

	for (int i = 0; i < a.getNumNode(); i++)
	{
		ofLine(a.getNode(i).getGlobalPosition(), b.getNode(i).getGlobalPosition());
	}
}

//

void ramBillboard()
{
	ofMatrix4x4 m;
	glGetFloatv(GL_MODELVIEW_MATRIX, m.getPtr());

	const ofVec3f& s = m.getScale();

	m(0, 0) = s.x;
	m(0, 1) = 0;
	m(0, 2) = 0;

	m(1, 0) = 0;
	m(1, 1) = s.y;
	m(1, 2) = 0;

	m(2, 0) = 0;
	m(2, 1) = 0;
	m(2, 2) = s.z;

	glLoadMatrixf(m.getPtr());
}

//

void ramStripe(const vector<ramNode> &nodes)
{
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < nodes.size(); i++)
	{
		ofVec3f v = nodes[i].getGlobalPosition();
		glVertex3fv(v.getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 5; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 6; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 7; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 8; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 9; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 10; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition(), n11.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 11; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}

void ramStripe(const ramNode& n1, const ramNode& n2, const ramNode& n3, const ramNode& n4, const ramNode& n5, const ramNode& n6, const ramNode& n7, const ramNode& n8, const ramNode& n9, const ramNode& n10, const ramNode& n11, const ramNode& n12)
{
	const ofVec3f v[] = { n1.getGlobalPosition(), n2.getGlobalPosition(), n3.getGlobalPosition(), n4.getGlobalPosition(), n5.getGlobalPosition(), n6.getGlobalPosition(), n7.getGlobalPosition(), n8.getGlobalPosition(), n9.getGlobalPosition(), n10.getGlobalPosition(), n11.getGlobalPosition(), n12.getGlobalPosition() };
	glBegin(ofGetFill() ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 12; i++)
	{
		glVertex3fv(v[i].getPtr());
	}
	glEnd();
}