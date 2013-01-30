#pragma once

class EmptyScene : public ramSceneBase
{
	
	string key_toggle;
	bool bColor;
	
public:
	
	EmptyScene()
	{
		setSceneName("Empty Scene");
		
		// ---
		
		key_toggle = "toggle test";
		bColor = false;
	}
	
	void setup()
	{
		
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		gui.addToggle( key_toggle );
	}
	
	void update()
	{
		if( gui().hasValueChanged( key_toggle ) )
		{
			bColor = gui().getValueB( key_toggle );
		}
	}
	
	void draw()
	{
		ofBackground( bColor ? ramColor::WHITE : ramColor::BLACK );
		
		ramCameraBegin();
		ofSetColor( ramColor::GRAY );
		ofDrawBitmapString( "Empty!", ofVec3f(0,0,0) );
		ramCameraEnd();
	}
	
	void drawActor( ramActor& actor )
	{
		
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
};

