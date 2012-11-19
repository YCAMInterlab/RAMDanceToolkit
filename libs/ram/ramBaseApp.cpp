#include "ramBaseApp.h"

#include "ramCameraManager.h"

void ramBaseApp::update(ofEventArgs &args)
{
    getActorManager().update();
//	recorder.update();
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
void ramBaseApp::ramRecStart(ramSession* session)
{
	session->startRecording();
//	recorder.recStart(session);
}
void ramBaseApp::ramRecStop(ramSession* session)
{
	session->stopRecording();
//	recorder.recStop(session);
}
void ramBaseApp::ramPlaySession(ramSession* session)
{
//	recorder.playSession(session);
}
void ramBaseApp::ramStopSession(ramSession* session)
{
//	recorder.stopSession(session);
}
