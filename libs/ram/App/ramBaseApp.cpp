#include "ramGlobal.h"
#include "ramBaseApp.h"
#include "ramCameraManager.h"
#include "ramControlPanel.h"

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
    
	drawFloor();

	{
		// draw shadow
		
		ramSharedData::instance().shadow.begin();
		
		for (int n = 0; n < getActorManager().getNumActor(); n++)
		{
			ramActor &o = getActorManager().getActor(n);
			drawActor(o);
		}
		
		for (int n = 0; n < getActorManager().getNumRigidBody(); n++)
		{
			ramRigidBody &o = getActorManager().getRigidBody(n);
			drawRigid(o);
		}
		
		ramSharedData::instance().shadow.end();
	}
	
	{
		// draw object
		
		for (int n = 0; n < getActorManager().getNumActor(); n++)
		{
			ramActor &o = getActorManager().getActor(n);
			drawActor(o);
		}
		
		for (int n = 0; n < getActorManager().getNumRigidBody(); n++)
		{
			ramRigidBody &o = getActorManager().getRigidBody(n);
			drawRigid(o);
		}
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
