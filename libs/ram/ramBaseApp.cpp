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
    
    getActorManager().draw();
    
    // actors
    {
        for (int n = 0; n < getActorManager().getNumActor(); n++)
        {
            ramActor &o = getActorManager().getActor(n);
            drawActor(o);
        }
    }
    
    // rigids
    {
        for (int n = 0; n < getActorManager().getNumRigidBody(); n++)
        {
            ramRigidBody &o = getActorManager().getRigidBody(n);
            drawRigid(o);
        }
    }
    
    // floor
    drawFloor();
    
	cam.end();
}


void ramBaseApp::exit(ofEventArgs &args)
{

}


// Rec & Play....
void ramBaseApp::ramRecStart(){}
void ramBaseApp::ramRecStop(){}
void ramBaseApp::ramPlay(){}
