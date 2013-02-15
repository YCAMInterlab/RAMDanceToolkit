#pragma once

class DuplicateMe : public ramBaseScene
{
	
	float mNumDuplicate;
	float mRadius;
	float mScale;
	float mTranslateX;
	float mTranslateZ;
	bool mShowActor;
	
public:
	
	DuplicateMe() : mNumDuplicate(10), mRadius(300), mScale(1.0), mTranslateX(0.0), mTranslateZ(0.0), mShowActor(false) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Scale", 0.1, 10.0, &mScale, gui.kLength, gui.kDim);
		panel->addSlider("Position: X", -300.0, 300.0, &mTranslateX, gui.kLength, gui.kDim);
		panel->addSlider("Position: Z", -300.0, 300.0, &mTranslateZ, gui.kLength, gui.kDim);
		
		
		panel->addSpacer(gui.kLength, 2);
		panel->addToggle("Show actor", &mShowActor, 50, 50);
		panel->addSlider("Duplicate", 1, 100, &mNumDuplicate, gui.kLength, gui.kDim);
		panel->addSlider("Radius", 10.0, 1000.0, &mRadius, gui.kLength, gui.kDim);
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
		float radian = 2 * M_PI / mNumDuplicate;
		int numDuplicate = floor(mNumDuplicate);
		
		for (int i=0; i<mNumDuplicate; i++)
		{
			float x = cos( radian * i ) * mRadius;
			float z = sin( radian * i ) * mRadius;

			ofColor c1 = ramColor::RED_DEEP;
			ofColor c2 = ramColor::BLUE_LIGHT;

			c1.g += i*5;
			c2.b += i*5;

			ofPushMatrix();
			ofTranslate(x, 0, z);
			if( i==0 ) ofTranslate(mTranslateX, 0, mTranslateZ);
			
			ofRotateY(360/mNumDuplicate * i);
			if( i==0 ) ofScale(mScale, mScale, mScale);

			if(mShowActor)
			{
				ramDrawBasicActor(actor, c1, c2);
			}
			else
			{
				ofSetColor(c1);
				ramNode &node = actor.getNode(ramActor::JOINT_LEFT_HAND);
				node.beginTransform();
				ofBox(5);
				node.endTransform();

				ofSetColor(c2);
				ramNode &node2 = actor.getNode(ramActor::JOINT_RIGHT_HAND);
				node2.beginTransform();
				ofBox(5);
				node2.endTransform();
			}
			ofPopMatrix();
		}

	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	const string getName() { return "Duplicate me"; }
};

