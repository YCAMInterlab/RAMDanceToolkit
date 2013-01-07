#include "ramBaseApp.h"

#include "ramCameraManager.h"

void ramBaseApp::update(ofEventArgs &args)
{
    getActorManager().update();
}


void ramBaseApp::draw(ofEventArgs &args)
{
	ofCamera &cam = ramCameraManager::instance().getActiveCamera();
	
    cam.begin();
    
    // floor
    drawFloor();
	
    // actors
	for (int n = 0; n < getActorManager().getNumActor(); n++)
	{
		ramActor &o = getActorManager().getActor(n);
		drawActor(o);
	}
    
    // rigids
	for (int n = 0; n < getActorManager().getNumRigidBody(); n++)
	{
		ramRigidBody &o = getActorManager().getRigidBody(n);
		drawRigid(o);
	}
    
	cam.end();
}


void ramBaseApp::exit(ofEventArgs &args)
{

}