#include "ramPrimitive.h"

ramPrimitive::ramPrimitive()
{
	ramPhysics::instance().registerPrimitive(this);
}

ramPrimitive::~ramPrimitive()
{
	ramPhysics::instance().unregisterPrimitive(this);
}

void ramPrimitive::_update()
{
	if (rigid)
	{
		ofMatrix4x4 mat = rigid.getTransform();
		setTransformMatrix(mat);
	}
}

ramCube::ramCube(float size) : size(size)
{
	rigid = ramPhysics::instance().getWorld().addBox(ofVec3f(size), ofVec3f());
}
