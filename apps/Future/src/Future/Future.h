#pragma once

class Future : public ramBaseScene
{
	
	ramGhost mGhost;
	
public:
	
	Future() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		mGhost.setupControlPanel(panel);
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
	
	void drawActor( ramActor& actor )
	{
		mGhost.update( actor );

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		ofPushStyle();
		ofNoFill();
		
		ramDrawBasicActor( (ramActor&)mGhost.getResult() );

		ofPopStyle();
		glPopAttrib();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	const string getName() { return "Future"; }
};

