#include "ramPrimitive.h"

ramPrimitive::ramPrimitive()
{
	ramPhysics::instance().registerPrimitive(this);
}

ramPrimitive::~ramPrimitive()
{
	ramPhysics::instance().unregisterPrimitive(this);
}

ofxBt::World& ramPrimitive::getWorld()
{
	return ramPhysics::instance().getWorld();
}

void ramPrimitive::_update()
{
	if (rigid)
	{
		ofMatrix4x4 mat = rigid.getTransform();
		setTransformMatrix(mat);
	}
}

#pragma mark - ramBoxPrimitive

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, float size)
{
	rigid = getWorld().addBox(ofVec3f(size), pos);
}

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, const ofVec3f& size)
{
	rigid = getWorld().addBox(size, pos);
}

ramBoxPrimitive::ramBoxPrimitive(const ofMatrix4x4& mat, float size)
{
	rigid = getWorld().addBox(ofVec3f(size), mat.getTranslation(), mat.getRotate().getEuler());
}

ramBoxPrimitive::ramBoxPrimitive(const ofMatrix4x4& mat, const ofVec3f& size)
{
	rigid = getWorld().addBox(size, mat.getTranslation(), mat.getRotate().getEuler());
}

void ramBoxPrimitive::customDraw()
{
	ofVec3f size = rigid.getSize();
	glScalef(size.x, size.y, size.z);
	ofBox(2);
}

#pragma mark - ramSpherePrimitive

ramSpherePrimitive::ramSpherePrimitive(const ofVec3f& pos, float size)
{
	rigid = getWorld().addSphere(size, pos);
}

void ramSpherePrimitive::customDraw()
{
	ofVec3f size = rigid.getSize();
	glScalef(size.x, size.x, size.x);
	ofSphere(1);
}

#pragma mark - ramCylinderPrimitive

ramCylinderPrimitive::ramCylinderPrimitive(const ofMatrix4x4& mat, float radius, float height)
{
	rigid = getWorld().addCylinder(radius, height * 0.5, mat.getTranslation(), mat.getRotate().getEuler());
}

ramCylinderPrimitive::ramCylinderPrimitive(const ofVec3f& pos, float radius, float height)
{
	rigid = getWorld().addCylinder(radius, height * 0.5, pos);
}

void ramCylinderPrimitive::customDraw()
{
	ofVec3f size = rigid.getSize();
	
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
