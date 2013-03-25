#include "ramBaseApp.h"
#include "ramCameraManager.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

bool drawModel = true;

void ramBaseApp::exit(ofEventArgs &args)
{
	ramDisableAllEvents();
	ofSetFullscreen(false);
}

void ramBaseApp::update(ofEventArgs &args)
{
	getActorManager().update();
}

void ramBaseApp::draw(ofEventArgs &args)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);

	ramBeginCamera();

	if (draw_floor_auto)
		drawFloor();

	getActorManager().draw();

	bool enable_physics = ramGetEnablePhysicsPrimitive();

	ramEnablePhysicsPrimitive(false);

	if (ramShadowEnabled())
	{
		// shadow

		ramBeginShadow();
		drawNodeArrays();
		ramEndShadow();
	}

	ramEnablePhysicsPrimitive(enable_physics);

	if (drawModel)
	{
		// entities
		drawNodeArrays();
	}

	ramEndCamera();

	glPopAttrib();
}

void ramBaseApp::drawNodeArrays()
{
	// draw nodearray

	for (int n = 0; n < getNumNodeArray(); n++)
	{
		const ramNodeArray &o = getNodeArray(n);
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		ofPushStyle();
		if (o.isActor())
			drawActor((ramActor &)o);
		else
			drawRigid((ramRigidBody &)o);
		
		ofPopStyle();
		glPopMatrix();
		glPopAttrib();
	}

	// draw bus

	map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();

	while (it != getActorManager().getAllBus().end())
	{
		const ramNodeArray &o = (*it).second;
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		ofPushStyle();
		if (o.isActor())
			drawActor((ramActor &)o);
		else
			drawRigid((ramRigidBody &)o);
		ofPopStyle();
		glPopMatrix();
		glPopAttrib();
		++it;
	}
}

void ramBaseApp::drawFloor()
{
	ramDrawBasicFloor(ramGetGUI().getPreferencesTab().getFloorPattern(),
					  ramGetGUI().getPreferencesTab().getFloorSize(),
					  ramGetGUI().getPreferencesTab().getFloorGridSize(),
					  ramColor::GRAY_ALPHA,
					  ramColor::DARK_GRAY_ALPHA);
}