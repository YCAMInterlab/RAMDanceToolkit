#include "ramPhysics.h"

#include "ramPrimitive.h"
#include "ramSoftBodyPrimitive.h"

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
		primitives[i]->internal_update();
	}

	world.update();

	// remove all temporary primitives

	for (int i = 0; i < temporary_primitives.size(); i++)
	{
		ramBasePrimitive *p = temporary_primitives[i];
		delete p;
	}

	temporary_primitives.clear();
	cache_index.clear();
}

void ramPhysics::registerRigidBodyPrimitive(ramPrimitive *o)
{
	if (find(primitives.begin(), primitives.end(), o) != primitives.end()) return;

	primitives.push_back(o);
}

void ramPhysics::unregisterRigidBodyPrimitive(ramPrimitive *o)
{
	vector<ramBasePrimitive*>::iterator it = find(primitives.begin(), primitives.end(), o);
	if (it == primitives.end()) return;

	world.removeRigidBody(o->body);

	primitives.erase(it);
}

void ramPhysics::registerTempraryPrimitive(ramPrimitive *o)
{
	temporary_primitives.push_back(o);
}

void ramPhysics::registerSoftBodyPrimitive(ramSoftBodyPrimitive *o)
{
	if (find(primitives.begin(), primitives.end(), o) != primitives.end()) return;

	primitives.push_back(o);
}

void ramPhysics::unregisterSoftBodyPrimitive(ramSoftBodyPrimitive *o)
{
	vector<ramBasePrimitive*>::iterator it = find(primitives.begin(), primitives.end(), o);
	if (it == primitives.end()) return;

	world.removeSoftBody(o->body);

	primitives.erase(it);
}

bool ramPhysics::checkAndUpdateNodeCache(const ramNode *node)
{
	if (cache_index.find(node) == cache_index.end())
	{
		cache_index.insert(node);
		return true;
	}
	return false;
}