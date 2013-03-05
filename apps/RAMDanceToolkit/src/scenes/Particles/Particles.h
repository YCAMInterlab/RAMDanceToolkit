#pragma once

#include "ParticleEngine.h"

class Particles : public ramBaseScene
{
	
	ParticleEngine pe;
	Gravity *gravity;
	Floor *floor;
	
	float particle_amount;
	
	ramFilterEach<ramGhost> filters;
	
public:
	
	Particles() : particle_amount(4.0) {}
	
	void setupControlPanel()
	{
		ramGetGUI().addSlider("Amount", 1.0, 15.0, &particle_amount);
		ramGetGUI().addSlider("Life", 0.1, 10.0, &pe.particle_life);
		ramGetGUI().addSlider("Velocity", 0.1, 5, &pe.particle_velocity);
		ramGetGUI().addSlider("Gravity", -0.1, 0.1, &gravity->force);
	}
	
	void setup()
	{
		pe.setup(50000);
		
		gravity = new Gravity;
		floor = new Floor;
		pe.addForceFactor(gravity);
		pe.addForceFactor(floor);
	}
	
	
	void update()
	{
		const vector<ramNodeArray>& NAs = filters.update(getAllNodeArrays());
        
		for (int n = 0; n < NAs.size(); n++)
		{
			const ramNodeArray &NA = NAs[n];
			
			for (int i = 0; i < NA.getNumNode(); i++)
			{
				const ramNode &node = NA.getNode(i);
				
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

		ofSetColor(255, 30);
		pe.draw();
		
		ofPopStyle();
		glPopAttrib();
		
		ramEndCamera();
	}
		
	const string getName() { return "Particles"; }
};
