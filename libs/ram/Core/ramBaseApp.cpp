// 
// ramBaseApp.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
	
	getCommunicationManager().update();

	getOscManager().update();
	
	update(); // calling testApp(or ofApp)::update()
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
	
	getCommunicationManager().draw();
	
	draw(); // calling testApp(or ofApp)::draw()
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