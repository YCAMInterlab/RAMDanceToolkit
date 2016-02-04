//
//  dp16_ramActorTranslator.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "dp16_ramActorTranslator.h"

#define ACTOR_ANDO string("Ando_2012-09-01_18-49-10")
#define ACTOR_KOJIRI string("Richi_2012-09-01_16-55-24")
#define ACTOR_SHIMAJI string("Richi_2012-09-01_16-52-45")
#define ACTOR_MIYASHITA string("Ando_2012-09-01_19-19-45")

void dp16_ramActorTranslator::setupControlPanel()
{
//	nameList[0] = ACTOR_MIYASHITA;
//	nameList[1] = ACTOR_SHIMAJI;
//	nameList[2] = ACTOR_KOJIRI;
//	nameList[3] = ACTOR_ANDO;
	nameList[0] = "miyashita";
	nameList[1] = "shimaji";
	nameList[2] = "kojiri";
	nameList[3] = "ando";
	
    palette[0] = dpDancerFloatColor::MIYASHITA_COLOR;
    palette[1] = dpDancerFloatColor::SHIMAJI_COLOR;
    palette[2] = dpDancerFloatColor::KOJIRI_COLOR;
    palette[3] = dpDancerFloatColor::ANDO_COLOR;
	
	ramGetGUI().addIntSlider("fix_pos", -1, 22, &fixer);
	ramGetGUI().addToggle("floorStay", &floorStay);
	ramGetGUI().addToggle("drawFloor", &drawFloor);
	ramGetGUI().addSlider("offset_x", -500, 500, &offset.x);
	ramGetGUI().addSlider("offset_y", -500, 500, &offset.y);
	ramGetGUI().addSlider("offset_z", -500, 500, &offset.z);
	ramGetGUI().addSeparator();
	ramGetGUI().addToggle("autoLayout", &autoLayout);
	ramGetGUI().addSlider("margine", 0, 1000, &margine);
	ramGetGUI().addSeparator();
	
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	
	for (int i = 0;i < NUM_DRAWACTOR;i++)
	{
		gui->addToggle("V_"+nameList[i], &visibles[i]);
		visibles[i] = true;
	}
	
	gui->addSlider("saturation", 0.0, 1.0, &base_saturation);
	gui->addSlider("brightness", 0.0, 1.0, &base_brightness);
	
	base_saturation = 0.6;
	base_brightness = 0.8;
	
	floor_scale = 300.0;
	floor_grid_span = 50;
	
	floorStay = true;
	autoLayout = true;
	margine = 160;
	drawFloor = true;
    
    mPhong.setup();
}

void dp16_ramActorTranslator::update()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	
	int numArray = am.getNumNodeArray();
	
	if (!autoLayout)
	{
		for (int i = 0;i < am.getNumNodeArray();i++)
		{
			ramNodeArray & act = am.getNodeArray(i);

			act.setFloor(floorStay);
			act.setFix(fixer);
			
			float floor_y = 0;
			for (int j = 0;j < am.getNodeArray(i).getNumNode();j++)
			{
				ramNode &nd = act.getNode(j);
				floor_y = MIN(floor_y, nd.getGlobalPosition().y);
			}
			
			floor_y = act.getNode(0).getGlobalPosition().y;
			
			act.setOffset(offset);
			
		}
	}
	else
	{
		int currentViewArray = 0;
		for (int i = 0;i < am.getNumNodeArray();i++){
			ramNodeArray & actor = am.getNodeArray(i);
			for (int j = 0;j < NUM_DRAWACTOR;j++)
			{
				if ((actor.getName() == nameList[j]) && visibles[j])
					currentViewArray++;
			}
		}
		
		int cnt = 0;
		for (int i = 0;i < am.getNumNodeArray();i++)
		{
			ramNodeArray & act = am.getNodeArray(i);
			
			for (int j = 0;j < NUM_DRAWACTOR;j++)
			{
				if ((act.getName() == nameList[j]) && visibles[j])
				{
					act.setFloor(floorStay);
					act.setFix(fixer);
					
					float x = margine * cnt - (margine * (currentViewArray - 1)) / 2.0;
					am.getNodeArray(i).setFix(fixer);
					am.getNodeArray(i).setOffset(offset + ofVec3f(x,0,0));
					cnt++;
				}
			}
		}
	}
	
	for (int i = 0;i < NUM_DRAWACTOR;i++)
	{
		palette[i].setHue(i / float(NUM_DRAWACTOR));
		palette[i].setSaturation(base_saturation);
		palette[i].setBrightness(base_brightness);
	}
}

void dp16_ramActorTranslator::draw()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	
	ramBeginCamera();
	ofPushStyle();
	
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
	
	ofNoFill();
	ofSetColor(dpColor::PALE_PINK_LIGHT);
	
	ofPushMatrix();
	ofRotateX(90);
    if (drawFloor)
    {
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
    }
	ofPopMatrix();
	
	ofFill();
	
	ofPopStyle();
	ramEndCamera();
}

void dp16_ramActorTranslator::drawTargetActor(int id, ramNodeArray &actor)
{
	ofSetColor(palette[id]);
    mPhong.begin();
	ramDrawBasicActor(actor);
    mPhong.end();
}

void dp16_ramActorTranslator::exit()
{
	ramActorManager & am = ramGlobalShortcut::getActorManager();
	for (int i = 0;i < am.getNumNodeArray();i++)
	{
		am.getNodeArray(i).setFix(-1);
		am.getNodeArray(i).setOffset(ofVec3f(0,0,0));
	}
}