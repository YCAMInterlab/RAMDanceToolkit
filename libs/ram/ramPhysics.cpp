#include "ramPhysics.h"

ramPhysics* ramPhysics::_instance = NULL;

ramPhysics& ramPhysics::instance()
{
	if (!_instance)
		_instance = new ramPhysics;
	return *_instance;
}

void ramPhysics::setup()
{
	if (inited) return;
	inited = true;
	
	world.setup();
	
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
	world.update();
}