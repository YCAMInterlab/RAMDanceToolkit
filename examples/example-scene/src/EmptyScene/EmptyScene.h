#pragma once

class EmptyScene : public ramSceneBase
{
	
public:
	
	void setupControlPanel(ofxUICanvas* panel)
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
		{
			ofDrawBitmapString( "Hello, "+getName()+ "!", ofVec3f(0,200,0) );
		}
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
	
	const string getName() { return "My scene"; }
};

