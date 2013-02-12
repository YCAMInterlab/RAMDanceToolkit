#pragma once

class EmptyScene : public ramBaseScene
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
		
		guiPtr = &gui;
	}
	
	void update()
	{
		if( guiPtr->hasValueChanged( key_toggle ) )
		{
			bColor = guiPtr->getValueB( key_toggle );
		}
	}
	
	void draw()
	{
		ofBackground( bColor ? ramColor::WHITE : ramColor::BLACK );
		
		if (!bEnabled) return;
		
		ramBeginCamera();
		ofSetColor( ramColor::GRAY );
		ofDrawBitmapString( "Empty!", ofVec3f(0,0,0) );
		ramEndCamera();
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

