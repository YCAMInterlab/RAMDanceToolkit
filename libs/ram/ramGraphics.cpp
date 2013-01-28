#include "ramGraphics.h"

#include "ramPhysics.h"
#include "ramPrimitive.h"

void ramBox(const ramNode& o, float size)
{
	o.transformBegin();
	ofBox(size);
	o.transformEnd();
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramBoxPrimitive *p = new ramBoxPrimitive(o.getTransformMatrix(), size);
//		p->setTransformMatrix(o.getMatrix());
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	ofSphere(o, radius);
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getPosition(), radius);
//		p->setTransformMatrix(o.getMatrix());
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}