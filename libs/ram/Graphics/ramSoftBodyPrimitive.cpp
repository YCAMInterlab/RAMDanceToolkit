#include "ramSoftBodyPrimitive.h"

ramSoftBodyPrimitive::ramSoftBodyPrimitive()
{
	ramPhysics::instance().registerSoftBodyPrimitive(this);
}

ramSoftBodyPrimitive::~ramSoftBodyPrimitive()
{
	ramPhysics::instance().unregisterSoftBodyPrimitive(this);
}

ofxBt::SoftBodyWorld& ramSoftBodyPrimitive::getWorld()
{
	return ramPhysics::instance().getSoftBodyWorld();
}

void ramSoftBodyPrimitive::internal_update() {}

//

ramRopePrimitive::ramRopePrimitive(const ofVec3f p0, const ofVec3f p1)
{
	body = getWorld().addRope(p0, p1);
}