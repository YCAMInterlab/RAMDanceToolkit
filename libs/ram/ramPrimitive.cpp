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

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, float size)
{
	rigid = getWorld().addBox(ofVec3f(size), pos);
}

ramBoxPrimitive::ramBoxPrimitive(const ofVec3f& pos, const ofVec3f& size)
{
	rigid = getWorld().addBox(size, pos);
}

void ramBoxPrimitive::customDraw()
{
	ofVec3f size = rigid.getSize();
	glScalef(size.x, size.y, size.z);
	ofBox(2);
}

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
