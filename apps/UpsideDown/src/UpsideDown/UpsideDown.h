#pragma once

class EmptyScene : public ramBaseScene
{
	
public:
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ofAddListener(panel->newGUIEvent, this, &EmptyScene::onValueChanged);
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
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		cout << "I am " << e.widget->getName() << endl;
	}
	
	const string getName() { return "My scene"; }
};

