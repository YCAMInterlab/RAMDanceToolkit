#pragma once

#include "ramBaseEvent.h"

#include "ramPrimitive.h"

enum ramTriggerTiming
{
	RAM_TRIGGER_UP,
	RAM_TRIGGER_DOWN,
	RAM_TRIGGER_BOTH
};

class ramCollisionEvent : public ramBaseEvent
{
public:
	
	ofEvent<ofEventArgs> onCollision;
	ofEvent<ofEventArgs> onCollisionEnd;
	
	ramCollisionEvent() : primitive(NULL), collision_state(false), last_collision_frame(false), timing(RAM_TRIGGER_UP) {}
	
	void setPrimitive(ramPrimitive *o)
	{
		primitive = o;
		primitive->getRigidBody().setCollisionCallback(CollisionCallback(this));
	}
	
	void setTiming(ramTriggerTiming t) { timing = t; }
	ramTriggerTiming getTiming() const { return timing; }
	
	bool getState() { return collision_state; }
	
protected:

	struct CollisionCallback
	{
		ramCollisionEvent *evt;
		
		CollisionCallback(ramCollisionEvent *evt) : evt(evt) {}
		
		void operator()()
		{
			evt->last_collision_frame = ofGetFrameNum();
		}
	};

	ramPrimitive *primitive;
	
	bool collision_state;
	int last_collision_frame;
	
	ramTriggerTiming timing;
	
	bool tick()
	{
		if (primitive == NULL) return false;
		
		bool b = ofGetFrameNum() - last_collision_frame < 2;
		if (b != collision_state)
		{
			collision_state = b;
			
			static ofEventArgs e;
			
			if (b) ofNotifyEvent(onCollision, e);
			else ofNotifyEvent(onCollisionEnd, e);
			
			if (timing == RAM_TRIGGER_BOTH) return true;
			else if (timing == RAM_TRIGGER_UP && b) return true;
			else if (timing == RAM_TRIGGER_DOWN && !b) return true;
			
			return false;
		}
		
		return false;
	}
	
};
