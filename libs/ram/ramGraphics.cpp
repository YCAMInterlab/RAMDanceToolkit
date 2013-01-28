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
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	ofSphere(o, radius);
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getPosition(), radius);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}