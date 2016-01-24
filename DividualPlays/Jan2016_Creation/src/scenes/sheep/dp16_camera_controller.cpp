//
//  dp16_camera_controller.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/24.
//
//

#include "dp16_camera_controller.h"

void dp16_camera_controller::setupControlPanel()
{
	ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
	
	gui->addToggle("Bind", &bindCamera);
	gui->addSlider("smooth", 1.0, 30.0, &smoothing);
	gui->addSpacer();
	gui->addToggle("Targeting", &targeting);
	gui->addSlider("speed", 0.0, 10.0, &speed);
	gui->addSlider("targ_x", -500, 500, &camera_target.x);
	gui->addSlider("targ_y", -500, 500, &camera_target.y);
	gui->addSlider("targ_z", -500, 500, &camera_target.z);
	
	gui->addSlider("look_x", -500, 500, &camera_lTarget.x);
	gui->addSlider("look_y", -500, 500, &camera_lTarget.y);
	gui->addSlider("look_z", -500, 500, &camera_lTarget.z);
	
	gui->addSpacer();
	gui->addToggle("nodeCam", &nodeCam);

	bindCamera = true;
	targeting = true;
	nodeCam = false;
	speed = 5.0;
	smoothing = 15.0;
	camera_pos.set(0.0, 500.0, 500);
	camera_target.set(0.0,800.0, 800.0);
	
	mex.setupControlPanel(this);
}

void dp16_camera_controller::update()
{
	if (nodeCam)
	{
		camera_target = mex.getPositionAt(0);
		camera_lTarget = mex.getPositionAt(1);
	}
	
	if (targeting)
	{
		camera_vector = (camera_target - camera_pos).normalized() * speed;
		
		if (camera_vector.lengthSquared() > (camera_target.distanceSquared(camera_pos)))
			camera_vector = camera_target - camera_pos;
		
		camera_pos += camera_vector;
		
		camera_lVector = (camera_lTarget - camera_lookAt).normalized() * speed;
		if (camera_lVector.lengthSquared() > (camera_lTarget.distanceSquared(camera_lookAt)))
			camera_lVector = camera_lTarget - camera_lookAt;
		
		camera_lookAt += camera_lVector;
	}
	
	camera_smoothed += (camera_pos - camera_smoothed) / smoothing;
	camera_lSmoothed += (camera_lookAt - camera_lSmoothed) / smoothing;
	
	ramGlobalShortcut::getActiveCamera().setPosition(camera_smoothed);
	ramGlobalShortcut::getActiveCamera().lookAt(camera_lSmoothed);
	
	mex.update();
}

void dp16_camera_controller::draw()
{
	ramBeginCamera();
	mex.draw();
	ramEndCamera();
}

void dp16_camera_controller::onPanelChanged(ofxUIEventArgs &e)
{
	
}

void dp16_camera_controller::drawActor(const ramActor &actor)
{
	
}

void dp16_camera_controller::onEnabled()
{
	
}

void dp16_camera_controller::onDisabled()
{
	
}