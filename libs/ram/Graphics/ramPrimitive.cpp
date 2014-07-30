// 
// ramPrimitive.cpp - RAMDanceToolkit
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

#include "ramPrimitive.h"

ramPrimitive::ramPrimitive()
{
	ramPhysics::instance().registerRigidBodyPrimitive(this);
}
ramPrimitive::~ramPrimitive()
{
	ramPhysics::instance().unregisterRigidBodyPrimitive(this);
}

ofxBt::World& ramPrimitive::getWorld()
{
	return ramPhysics::instance().getWorld();
}

void ramPrimitive::internal_update()
{
	if (body)
	{
		ofMatrix4x4 mat = body.getTransform();
		setTransformMatrix(mat);
	}
}

void ramPrimitive::updatePhysicsTransform()
{
	body.setTransform(getGlobalTransformMatrix());
}

#pragma mark - ramBoxPrimitive

ramBoxPrimitive::ramBoxPrimitive(float size)
{
	body = getWorld().addBox(ofVec3f(size), ofVec3f());
}

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& size)
{
	body = getWorld().addBox(size, ofVec3f());
}

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, float size)
{
	body = getWorld().addBox(ofVec3f(size), pos);
}

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, const ofVec3f& size)
{
	body = getWorld().addBox(size, pos);
}

ramBoxPrimitive::ramBoxPrimitive(const ofMatrix4x4& mat, float size)
{
	body = getWorld().addBox(ofVec3f(size), mat.getTranslation(), mat.getRotate().getEuler());
}

ramBoxPrimitive::ramBoxPrimitive(const ofMatrix4x4& mat, const ofVec3f& size)
{
	body = getWorld().addBox(size, mat.getTranslation(), mat.getRotate().getEuler());
}

void ramBoxPrimitive::customDraw()
{
	ofVec3f size = body.getSize();
	glScalef(size.x, size.y, size.z);
	ofDrawBox(2);
}

#pragma mark - ramSpherePrimitive

ramSpherePrimitive::ramSpherePrimitive(float size)
{
	body = getWorld().addSphere(size * 0.5, ofVec3f());
}

ramSpherePrimitive::ramSpherePrimitive(const ofVec3f& pos, float size)
{
	body = getWorld().addSphere(size * 0.5, pos);
}

void ramSpherePrimitive::customDraw()
{
	ofVec3f size = body.getSize();
	glScalef(size.x, size.x, size.x);
	ofDrawSphere(1);
}

#pragma mark - ramCylinderPrimitive

ramCylinderPrimitive::ramCylinderPrimitive(float radius, float height)
{
	body = getWorld().addCylinder(radius, height * 0.5, ofVec3f(), ofVec3f());
}

ramCylinderPrimitive::ramCylinderPrimitive(const ofMatrix4x4& mat, float radius, float height)
{
	body = getWorld().addCylinder(radius, height * 0.5, mat.getTranslation(), mat.getRotate().getEuler());
}

ramCylinderPrimitive::ramCylinderPrimitive(const ofVec3f& pos, float radius, float height)
{
	body = getWorld().addCylinder(radius, height * 0.5, pos);
}

void ramCylinderPrimitive::customDraw()
{
	ofVec3f size = body.getSize();

	const int num_vtx = 32;

	vector<ofVec2f> p(num_vtx);
	for (int i = 0; i < num_vtx; i++)
	{
		float d = ofMap(i, 0, num_vtx - 1, 0, TWO_PI);
		ofVec3f v;
		v.x = sin(d) * size.x;
		v.y = cos(d) * size.x;
		p[i] = v;
	}

	glPushAttrib(GL_ENABLE_BIT);

	if (ofGetStyle().bFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, -size.y, 0);

	for (int i = 0; i < num_vtx; i++)
	{
		glVertex3f(p[i].x, -size.y, p[i].y);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < num_vtx; i++)
	{
		glVertex3f(p[i].x, size.y, p[i].y);
		glVertex3f(p[i].x, -size.y, p[i].y);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, size.y, 0);

	for (int i = 0; i < num_vtx; i++)
	{
		glVertex3f(p[i].x, size.y, p[i].y);
	}
	glEnd();

	glPopAttrib();
}

// ramPyramidPrimitive

ramPyramidPrimitive::ramPyramidPrimitive(float size)
{
	size *= 0.5;

	mesh.addVertex(ofVec3f(1, 1, 1) * size);
	mesh.addVertex(ofVec3f(-1, 1, -1) * size);
	mesh.addVertex(ofVec3f(-1, -1, 1) * size);
	mesh.addVertex(ofVec3f(1, -1, -1) * size);

	mesh.addTriangle(0, 1, 2);
	mesh.addTriangle(0, 2, 3);
	mesh.addTriangle(0, 3, 1);
	mesh.addTriangle(3, 2, 1);

	body = getWorld().addMesh(mesh, ofVec3f(), ofVec3f());
}

ramPyramidPrimitive::ramPyramidPrimitive(const ofMatrix4x4& mat, float size)
{
	size *= 0.5;

	mesh.addVertex(ofVec3f(1, 1, 1) * size);
	mesh.addVertex(ofVec3f(-1, 1, -1) * size);
	mesh.addVertex(ofVec3f(-1, -1, 1) * size);
	mesh.addVertex(ofVec3f(1, -1, -1) * size);

	mesh.addTriangle(0, 1, 2);
	mesh.addTriangle(0, 2, 3);
	mesh.addTriangle(0, 3, 1);
	mesh.addTriangle(3, 2, 1);

	body = getWorld().addMesh(mesh, mat.getTranslation(), mat.getRotate().getEuler());
}

ramPyramidPrimitive::ramPyramidPrimitive(const ofVec3f& pos, float size)
{
	size *= 0.5;

	mesh.addVertex(ofVec3f(1, 1, 1) * size);
	mesh.addVertex(ofVec3f(-1, 1, -1) * size);
	mesh.addVertex(ofVec3f(-1, -1, 1) * size);
	mesh.addVertex(ofVec3f(1, -1, -1) * size);

	mesh.addTriangle(0, 1, 2);
	mesh.addTriangle(0, 2, 3);
	mesh.addTriangle(0, 3, 1);
	mesh.addTriangle(3, 2, 1);

	body = getWorld().addMesh(mesh, pos);
}

void ramPyramidPrimitive::customDraw()
{
	if (ofGetStyle().bFill)
		mesh.draw();
	else
		mesh.drawWireframe();
}