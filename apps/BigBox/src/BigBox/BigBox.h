#pragma once

class BigBox : public ramBaseScene
{
	vector<float> mSizeArray;
	float mBoxLineWidth;
	float mMasterBoxSize;
	
public:

	BigBox() : mBoxLineWidth(2.0), mMasterBoxSize(300.0)
	{
		mSizeArray.clear();
		mSizeArray.resize(ramActor::NUM_JOINTS);
		for (int i=0; i<mSizeArray.size(); i++)
			mSizeArray.at(i) = mMasterBoxSize;
	}

	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addSlider("Line width", 0.0, 10.0, &mBoxLineWidth, gui.kLength, gui.kDim);
		panel->addSlider("Master box size", 0.0, 1000.0, &mMasterBoxSize, gui.kLength, gui.kDim);
		panel->addSpacer(gui.kLength, 2);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			panel->addSlider(ramActor::getJointName(i), 0.0, 1000.0, &mSizeArray.at(i), gui.kLength, gui.kDim);
		}
		
		ofAddListener(panel->newGUIEvent, this, &BigBox::onPanelChanged);
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
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			float boxSize = (i==ramActor::JOINT_HEAD) ? 6 : 3;
			float bigBoxSize = mSizeArray.at(i);
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			{
				ofPushStyle();
				ofNoFill();
				
				glEnable(GL_DEPTH_TEST);
				
				/*!
				 big box
				 */
				ofSetColor( ramColor::BLUE_DEEP );
				ofSetLineWidth(mBoxLineWidth);
				node.beginTransform();
				ofBox(bigBoxSize);
				node.endTransform();
				
				ofPopStyle();
			}
			glPopMatrix();
			glPopAttrib();
		}
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Master box size")
		{
			for (int i=0; i<mSizeArray.size(); i++)
				mSizeArray.at(i) = mMasterBoxSize;
		}
	}
	
	const string getName() { return "Big Box"; }
};
