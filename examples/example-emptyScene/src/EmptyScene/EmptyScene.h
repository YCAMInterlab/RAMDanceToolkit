#pragma once

class EmptyScene : public ramBaseScene
{

public:
	
	string getName() const { return "My scene"; }

	void setupControlPanel()
	{
		ramGetGUI().addSlider("Slider", 0.0, 10.0, &mySlider);
		ramGetGUI().addToggle("Toggle", &myToggle);
		
		ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &EmptyScene::onPanelChanged);
	}

	void setup()
	{

	}

	void update()
	{

	}

	void draw()
	{
		ofColor color( myToggle ? 150 : 50 );
		ofSetColor( color );
		
		ramBeginCamera();
		ofDrawBitmapString( "Hello, "+getName()+ "!" + " Slider value is " + ofToString(mySlider), ofVec3f(0,200,0) );
		ramEndCamera();
	}

	void drawActor(const ramActor& actor)
	{

	}

	void drawRigid(const ramRigidBody &rigid)
	{

	}
	
	void onActorSetup(const ramActor &actor)
	{
		
	}
	
	void onActorExit(const ramActor &actor)
	{
		
	}
	
	void onRigidSetup(const ramRigidBody &rigid)
	{
		
	}
	
	void onRigidExit(const ramRigidBody &rigid)
	{
		
	}
	
private:
	
	void onPanelChanged(ofxUIEventArgs &e)
	{
		const string name = e.widget->getName();
		
		// do something...
	}
	
	float mySlider;
	bool myToggle;
};

