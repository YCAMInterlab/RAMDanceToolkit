#pragma once

#include "ParticleEngine.h"

class Particles : public ramBaseScene
{
	
	ParticleEngine pe;
	Gravity *gravity;
	Floor *floor;
	
	float particle_amount;
	
public:
	
	Particles() : particle_amount(4.0) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		panel->addSlider("Amount", 1.0, 15.0, &particle_amount, gui.kLength, gui.kDim);
		panel->addSlider("Life", 0.1, 10.0, &pe.particle_life, gui.kLength, gui.kDim);
		panel->addSlider("Velocity", 0.1, 5, &pe.particle_velocity, gui.kLength, gui.kDim);
		panel->addSlider("Gravity", -0.5, 0.5, &gravity->force, gui.kLength, gui.kDim);
		
		ofAddListener(panel->newGUIEvent, this, &Particles::onValueChanged);
	}
	
	void setup()
	{
		pe.setup(10000);
		
		gravity = new Gravity;
		floor = new Floor;
		pe.addForceFactor(gravity);
		pe.addForceFactor(floor);
	}
	
	
	void update()
	{
		for (int n=0; n<getNumNodeArray(); n++)
		{
			ramNodeArray &nodeArray = getNodeArray(0);
			for (int i=0; i<nodeArray.getNumNode(); i++)
			{
				const ramNode &node = nodeArray.getNode(i);
				for(int j=0; j<particle_amount; j++)
					pe.emit( node.getGlobalPosition() );
			}
		}
		
		pe.update();
	}
	
	void draw()
	{
		ramBeginCamera();
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		ofPushStyle();
		ofNoFill();
		

		ofSetColor(255, 60);
		pe.draw();
		
		ofPopStyle();
		glPopAttrib();
		ramEndCamera();
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		
	}
	
	const string getName() { return "Particles"; }
};
