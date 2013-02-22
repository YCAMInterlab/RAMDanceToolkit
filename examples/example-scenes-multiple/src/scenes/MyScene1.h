#pragma once

class MyScene1 : public ramBaseScene
{
private:
	
	float fontColor;
	
public:
	
	MyScene1(): fontColor(100) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Font Color", 0.0, 255.0, &fontColor, gui.kLength, gui.kDim);
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

