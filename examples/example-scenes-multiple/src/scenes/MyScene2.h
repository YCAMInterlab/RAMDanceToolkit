#pragma once

class MyScene2 : public ramBaseScene
{
private:
	
	ramGhost ghost;
	float fontColor;
	
public:
	
	MyScene2(): fontColor(100) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Font Color", 0.0, 255.0, &fontColor, gui.kLength, gui.kDim);
		
		ghost.setupControlPanel(panel);
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
		ghost.update(actor);
		
		ofVec3f pos = ghost.getResult().getNode(ramActor::JOINT_HEAD).getPosition();
		pos.y += 30.0;
		
		ofSetColor( fontColor );
		ofDrawBitmapString( "I am " + getName(), pos );
		ramBasicActor((ramActor&)ghost.getResult());
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
	
	const string getName() { return "MyScene2"; }
};
