#include "ramSharedData.h"

#include "ramBaseApp.h"
#include "ramCameraManager.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

void ramBaseApp::exit(ofEventArgs &args)
{
	ramDisableAllEvents();
}

void ramBaseApp::update(ofEventArgs &args)
{
    getActorManager().update();
}

void ramBaseApp::draw(ofEventArgs &args)
{
	ramBeginCamera();
	
	drawFloor();

	bool enable_physics = ramGetEnablePhysicsPrimitive();
	ramSetEnablePhysicsPrimitive(false);
	
	{
		// draw shadow
		
		ramBeginShadow();
		
		for (int n=0; n<getActorManager().getNumNodeArray(); n++)
		{
			ramNodeArray &o = getNodeArray(n);
			
			if (o.getNumNode() == ramActor::NUM_JOINTS)
				drawActor((ramActor&)o);
			else
				drawRigid((ramRigidBody&)o);
		}
		
		ramEndShadow();
	}
	
	ramSetEnablePhysicsPrimitive(enable_physics);
	{
		for (int n=0; n<getActorManager().getNumNodeArray(); n++)
		{
			ramNodeArray &o = getNodeArray(n);
			if (o.getNumNode() == ramActor::NUM_JOINTS)
				drawActor((ramActor&)o);
			else
			{
				drawRigid((ramRigidBody&)o);
			}
		}
	}
	
	getActorManager().draw();
    
	ramEndCamera();
}

void ramBaseApp::drawFloor()
{
	if (!draw_floor_auto) return;
	
	ramControlPanel &gui = ramGetGUI();
	ramBasicFloor(gui.getFloorPattern(),
				  gui.getFloorSize(),
				  gui.getGridSize(),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_DEEP);
}
