//
//  dp16_actorDrawer.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/26.
//
//

#include "dp16_actorDrawer.h"

void dp16_actorDrawer::setupControlPanel()
{
	nameList[2] = "kojiri";
	nameList[0] = "miyashita";
	nameList[1] = "shimaji";
	nameList[3] = "ando";
	
//	nameList[0] = "Ando_2012-09-01_19-19-45";
//	nameList[1] = "Ando_2012-09-01_18-49-10";
//	nameList[2] = "Richi_2012-09-01_16-52-45";
//	nameList[3] = "Richi_2012-09-01_16-55-24";
	
	palette[0].setHsb( 0.5, 0.9, 0.7);
	palette[1].setHsb(0.95, 0.9, 0.7);
	palette[2].setHsb( 0.3, 0.9, 0.7);
	palette[3].setHsb( 0.0, 0.9, 0.7);
	
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	
	for (int i = 0;i < NUM_DRAWACTOR;i++)
	{
		gui->addToggle("V_"+nameList[i], &visibles[i]);
		visibles[i] = true;
	}
	
	gui->addSlider("saturation", 0.0, 1.0, &base_saturation);
	gui->addSlider("brightness", 0.0, 1.0, &base_brightness);
	
	/// light for drawActor
	light.setPosition(300, 600, 300);
	
	base_saturation = 0.6;
	base_brightness = 0.8;
	
	floor_scale = 300.0;
	floor_grid_span = 50;
}

void dp16_actorDrawer::update()
{
	for (int i = 0;i < NUM_DRAWACTOR;i++)
	{
		palette[i].setHue(i / float(NUM_DRAWACTOR));
		palette[i].setSaturation(base_saturation);
		palette[i].setBrightness(base_brightness);
	}
}

void dp16_actorDrawer::draw()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	
	ramBeginCamera();
	ofPushStyle();
	light.enable();
	
	for (int i = 0;i < am.getNumNodeArray();i++)
	{
		ramNodeArray & actor = am.getNodeArray(i);
		for (int j = 0;j < NUM_DRAWACTOR;j++)
		{
			if ((actor.getName() == nameList[j]) && visibles[j])
			{
				drawTargetActor(j, actor);
			}
		}
	}
	
	light.disable();
	
	ofNoFill();
	ofSetColor(dpColor::PALE_PINK_LIGHT);
	
	ofPushMatrix();
	ofRotateX(90);
	for (int i = 0;i < floor_scale; i+=floor_grid_span)
	{
		for (int j = 0;j < floor_scale;j+=floor_grid_span)
		{
			ofRect( i, j, floor_grid_span, floor_grid_span);
			ofRect(-i, j, floor_grid_span, floor_grid_span);
			ofRect( i,-j, floor_grid_span, floor_grid_span);
			ofRect(-i,-j, floor_grid_span, floor_grid_span);
		}
	}
	ofPopMatrix();
	
	ofFill();
	
	ofPopStyle();
	ramEndCamera();
}

void dp16_actorDrawer::drawTargetActor(int id, ramNodeArray &actor)
{
	ofSetColor(palette[id]);
	ramDrawBasicActor(actor);
}