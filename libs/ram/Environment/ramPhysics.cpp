#include "ramPhysics.h"

static bool ram_enable_physics_primitive = true;

void ramSetEnablePhysicsPrimitive(bool v)
{
	ram_enable_physics_primitive = v;
}

bool ramGetEnablePhysicsPrimitive()
{
	return ram_enable_physics_primitive;
}

ramPhysics* ramPhysics::_instance = NULL;

ramPhysics& ramPhysics::instance()
{
	if (!_instance)
	{
		_instance = new ramPhysics;
		_instance->setup();
	}
	return *_instance;
}

void ramPhysics::setup()
{
	if (inited) return;
	inited = true;
	
	world.setup(ofVec3f(0, -980, 0));
	world.addWorldBox(ofVec3f(-1000, 0, -1000), ofVec3f(1000, 1000, 1000));
	
	ofAddListener(ofEvents().update, this, &ramPhysics::onUpdate);
}

void ramPhysics::clear()
{
	world.clear();
}

void ramPhysics::debugDraw()
{
	world.draw();
}

void ramPhysics::onUpdate(ofEventArgs&)
{
	for (int i = 0; i < primitives.size(); i++)
	{
		primitives[i]->_update();
	}
	
	world.update();
	
	// remove all temporary primitives
	
	for (int i = 0; i < temporary_primitives.size(); i++)
	{
		ramPrimitive *p = temporary_primitives[i];
		delete p;
	}
	
	temporary_primitives.clear();
}

void ramPhysics::registerPrimitive(ramPrimitive *o)
{
	if (find(primitives.begin(), primitives.end(), o) != primitives.end()) return;
	primitives.push_back(o);
}

void ramPhysics::unregisterPrimitive(ramPrimitive *o)
{
	vector<ramPrimitive*>::iterator it = find(primitives.begin(), primitives.end(), o);
	if (it == primitives.end()) return;
	world.removeRigidBody(o->rigid);
	primitives.erase(it);
}

void ramPhysics::registerTempraryPrimitive(ramPrimitive *o)
{
	ofxBt::RigidBody(o->getRigidBody()).setKinematic(true);
	temporary_primitives.push_back(o);
}
