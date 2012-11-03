#include "ramActorManager.h"

ramActorManager* ramActorManager::_instance = NULL;

ramActorManager& ramActorManager::instance()
{
	if (_instance == NULL)
		_instance = new ramActorManager;
	return *_instance;
}

void ramActorManager::update()
{
	/*
	   order:
	   - check onstage-actors, onstage-rigids
	   - renderer update
	 */

	// check onstage-actors
	for (int i = 0; i < actors.size(); i++)
	{
		ramActor &actor = getActor(i);
		if (actor.isOutdated())
		{
//            !!!: TODO
//            ofNotifyEvent(onStageOut, actor);
			actors.remove(actor.getName());
		}
	}

	for (int i = 0; i < rigids.size(); i++)
	{
		ramRigidBody &rigid = getRigidBody(i);
		if (rigid.isOutdated())
		{
			//            !!!:
			//            ofNotifyEvent(onStageOut, actor);
			rigids.remove(rigid.getName());
		}
	}


	// !!!: collision update
	// for (int i=0; i<rigids.size(); i++) getRigidBody(i).update();
}

void ramActorManager::draw()
{
}


void ramActorManager::updateWithOscMessage(const ofxOscMessage &m)
{
	const std::string addr = m.getAddress();

	if (addr == RAM_OSC_ADDR_SKELETON)
	{
		const std::string name = m.getArgAsString(0);

		if (!actors.hasKey(name))
		{
			ramActor o;
			o.updateWithOscMessage(m);
			o.setName(name);
			actors.add(name, o);
		}
		else
		{
			ramActor &o = actors[name];
			o.updateWithOscMessage(m);
		}
	}
	else if (addr == RAM_OSC_ADDR_RIGID_BODY)
	{
		const std::string name = m.getArgAsString(0);

		if (!rigids.hasKey(name))
		{
			ramRigidBody o;
			o.updateWithOscMessage(m);
			rigids.add(name, o);
		}
		else
		{
			ramRigidBody &o = rigids[name];
			o.updateWithOscMessage(m);
		}
	}
}
