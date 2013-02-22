#pragma once

class EmptyScene : public ramBaseScene
{
	
public:
	
	const string getName() { return "My scene"; }
	
	void setupControlPanel()
	{
	}

	void setup()
	{
		
	}
	
	
	void update()
	{
		
	}
	
	void draw()
	{
		ramBeginCamera();
		ofDrawBitmapString( "Hello, "+getName()+ "!", ofVec3f(0,200,0) );
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

