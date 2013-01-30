#pragma once
#include "ramSceneBase.h"
#include "ofxSimpleParticleEngine.h"


class Future : public ramSceneBase
{
	
	ofxSimpleParticleEngine pe;
	ramGhost ghost;
	bool bGhost, bParticle;
	
	string key_slider_speed, key_slider_distance, key_toggle_pe, key_toggle_draw;
	
public:
	
	Future()
	{
		setSceneName("Future");
		
		// ---
		
		pe.setup(100000);
		pe.addForceFactor(new Gravity);
		pe.addForceFactor(new Floor);
		
		bGhost = true;
		bParticle = false;
		
		key_slider_speed = "Speed";
		key_slider_distance = "Distance";
		key_toggle_draw = "Ghost";
		key_toggle_pe = "Salt";
	}
	
	void setupControlPanel(ramControlPanel& gui)
	{
//		gui.addToggle(key_toggle_draw, true);
//		gui.addToggle(key_toggle_pe);
//		gui.addSlider(key_slider_speed, 200, 0, 1000);
//		gui.addSlider(key_slider_distance, 60, 0, 1000);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		ghost.update( getActor(myActorName) );
		for (int i=0; i<ghost.getActor().getNumNode(); i++)
		{
			ramNode &node = ghost.getActor().getNode(i);
			for(int j=0; j<10; j++)
				pe.emit( node.getPosition() );
		}
		
		pe.update();
		
//		if(gui().hasValueChanged( variadic(key_slider_distance)(key_slider_speed)(key_toggle_pe)(key_toggle_draw) ))
//		{
//			float speed = gui().getValueF(key_slider_speed);
//			float distance = gui().getValueF(key_slider_distance);
//			
//			bGhost = gui().getValueB(key_toggle_draw);
//			bParticle = gui().getValueB(key_toggle_pe);
//			
//			ghost.setSpeed(speed);
//			ghost.setDistance(distance);
//		}
		
	}
	
	void draw()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		ofPushStyle();
		ofNoFill();
		
		ofColor shadowColor = ramColor::GRAY;
		shadowColor.a = 50;
		
		ramCameraBegin();
		if (bGhost)
		{
			for (int i=0; i<ghost.getActor().getNumNode(); i++)
			{
				ramNode &node = ghost.getActor().getNode(i);
				const int boxSize = (i==ramActor::JOINT_HEAD) ? 6 : 3;
				
				glEnable(GL_DEPTH_TEST);
				node.transformBegin();
				ofSetColor( ramColor::RED_DEEP );
				ofBox(boxSize);
				node.transformEnd();
				
				if (node.hasParent())
				{
					ofColor c = ramColor::RED_LIGHT;
					c -= 40;
					ofSetColor(c);
					ofLine(node, *node.getParent());
				}
				
				/*!
				 shadows
				 */
				ofColor shadowColor = ramColor::GRAY;
				shadowColor.a = 90;
				
				ramGlobal().beginShadowMatrix();
				ofEnableAlphaBlending();
				ofSetColor(shadowColor);
				
				ofBox(node, boxSize);
				if (node.hasParent())
					ofLine(node, *node.getParent());
				
				ramGlobal().endShadowMatrix();
			}
		}
		
		if (bParticle)
		{
			glEnable(GL_DEPTH_TEST);
			ofSetColor(255);
			pe.draw();
		}
		ramCameraEnd();
		
		ofPopStyle();
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	
	void drawActor(ramActor& actor)
	{
		
	}
	
	void drawRigidBody(ramRigidBody& rigid)
	{
		
	}
	
	void drawFloor()
	{
		
	}
};

