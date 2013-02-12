#pragma once
#include "ofxSimpleParticleEngine.h"

class Future : public ramSceneBase
{
	
	ofxSimpleParticleEngine pe;
	ramGhost mGhost;
	bool mShowGhost, mShowParticle;
	
public:
	
	Future() : mShowGhost(true), mShowParticle(true) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		mGhost.setupControlPanel(panel);
		panel->addToggle("Show Ghost", &mShowGhost, 50, 50);
		panel->addToggle("Show Salt", &mShowParticle, 50, 50);
	}
	
	void setup()
	{
		pe.setup(100000);
	}
	
	void update()
	{
		
		for (int i=0; i<mGhost.getResult().getNumNode(); i++)
		{
			const ramNode &node = mGhost.getResult().getNode(i);
			for(int j=0; j<10; j++)
				pe.emit( node.getPosition() );
		}
		pe.update();
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		mGhost.update( actor );

		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		ofPushStyle();
		ofNoFill();
		
		if (mShowGhost)
		{
			ramBasicActor( (ramActor&)mGhost.getResult() );
		}

		if (mShowParticle)
		{
			ofSetColor(255);
			pe.draw();
		}
		
		ofPopStyle();
		glPopAttrib();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
		
	}
	
	const string getName() { return "Future events"; }
};

