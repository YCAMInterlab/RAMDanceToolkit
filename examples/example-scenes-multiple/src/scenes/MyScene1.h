#pragma once

class MyScene1 : public ramBaseScene
{
private:
	
	float fontColor;
	
public:
	
	MyScene1(): fontColor(100) {}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();
		
		gui.addSlider("Font Color", 0.0, 255.0, &fontColor);
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
		ofVec3f pos = actor.getNode(ramActor::JOINT_HEAD).getPosition();
		pos.y += 30.0;
		
		ofSetColor( fontColor );
		ofDrawBitmapString( "I am " + getName(), pos );
		ramDrawBasicActor(actor);
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
	
	const string getName() { return "MyScene1"; }
};

