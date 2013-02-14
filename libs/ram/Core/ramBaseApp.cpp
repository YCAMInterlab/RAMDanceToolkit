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
	ofCamera &cam = ramCameraManager::instance().getActiveCamera();
	
    cam.begin();
    
	getActorManager().draw();
	
	drawFloor();

	bool enable_physics = ramGetEnablePhysicsPrimitive();
	
	ramSetEnablePhysicsPrimitive(false);
	
	{
		// shadow
		
		ramBeginShadow();
		
		drawNodeArrays();

		ramEndShadow();
	}
	
	{
		// entities
		
		ramSetEnablePhysicsPrimitive(enable_physics);
		
		drawNodeArrays();
	}
    
	cam.end();
}

void ramBaseApp::drawFloor()
{
	ramControlPanel &gui = ramGetGUI();
	
	ramBasicFloor(gui.getFloorPattern(),
				  gui.getFloorSize(),
				  gui.getGridSize(),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_DEEP);
}

void ramBaseApp::drawNodeArrays()
{
	// draw nodearray
	
	for (int n=0; n<getNumNodeArray(); n++)
	{
		ramNodeArray &o = getNodeArray(n);
		
		if (o.isActor())
			drawActor((ramActor&)o);
		else
			drawRigid((ramRigidBody&)o);
	}
	
	// draw bus
	
	map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();
	
	while( it != getActorManager().getAllBus().end() )
	{
		ramNodeArray &o = (*it).second;
		
		if (o.isActor())
			drawActor((ramActor&)o);
		else
			drawRigid((ramRigidBody&)o);
		++it;
	}
}