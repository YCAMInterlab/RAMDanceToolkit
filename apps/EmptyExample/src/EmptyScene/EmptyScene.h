#pragma once

class EmptyScene : public ramSceneBase
{
	
public:
	
	EmptyScene()
	{
		setSceneName("Empty Scene");
		
		// ---
		
	}
	
	
	void setup()
	{
		
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		gui.addToggle( "toggle test" );
		
		guiPtr = &gui;
	}
	
	void update()
	{

	}
	
	void draw()
	{

	}
};

