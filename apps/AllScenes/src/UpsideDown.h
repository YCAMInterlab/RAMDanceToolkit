#pragma once

#include "ramUpsideDown.h"

class UpsideDown : public ramBaseScene
{
public:
	
	ramUpsideDown upsideDown;
	
	UpsideDown() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		
	}
	
	void drawActor(ramActor& actor)
	{
		const ramActor &result = upsideDown.update(actor);
		ramBasicActor(result);
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	const string getName() { return "Upside Down"; }
};

