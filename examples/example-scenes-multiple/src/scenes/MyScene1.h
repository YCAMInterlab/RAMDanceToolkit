#pragma once

class MyScene1 : public ramSceneBase
{
private:
	
	float fontColor;
	
public:
	
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
		
		ofSetColor( fontColor );
		ofDrawBitmapString( "I am " + getName(), pos );
		ramBasicActor(actor);
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
	
	const string getName() { return "MyScene1"; }
};

