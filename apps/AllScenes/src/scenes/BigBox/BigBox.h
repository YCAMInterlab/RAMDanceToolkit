#pragma once

class BigBox : public ramBaseScene
{
	vector<float> mSizeArray;
	float mBoxLineWidth;
	float mMasterBoxSize;
	bool mUseSingleColor;
	float r, g, b;
	
public:

	BigBox() : mBoxLineWidth(2.0), mMasterBoxSize(300.0), mUseSingleColor(true), r(0), g(250), b(250)
	{
		mSizeArray.clear();
		mSizeArray.resize(ramActor::NUM_JOINTS);
		for (int i=0; i<mSizeArray.size(); i++)
			mSizeArray.at(i) = mMasterBoxSize;
	}

	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addToggle("Use single color", &mUseSingleColor, 30, 30);
		panel->addSlider("BigBox R", 0, 255, &r, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addSlider("BigBox G", 0, 255, &g, 95, gui.kDim);
		panel->addSlider("BigBox B", 0, 255, &b, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Line width", 0.0, 10.0, &mBoxLineWidth, gui.kLength, gui.kDim);
		panel->addSlider("Master box size", 0.0, 1000.0, &mMasterBoxSize, gui.kLength, gui.kDim);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			panel->addSlider(ramActor::getJointName(i), 0.0, 1000.0, &mSizeArray.at(i), gui.kLength, gui.kDim/2);
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
				if (mUseSingleColor)
				{
					ofSetColor(r, g, b);
				}
				else
				{
					if (i%3 == 0)
					{
						ofSetColor( ramColor::BLUE_DEEP );
					}
					else if (i%3 == 1)
					{
						ofSetColor( ramColor::BLUE_NORMAL );
					}
					else
					{
						ofSetColor( ramColor::BLUE_LIGHT );
					}
				}
				
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
		for (int i=0; i<rigid.getNumNode(); i++)
		{
			const ramNode &node = rigid.getNode(i);
			float boxSize = (i==0) ? 6 : 3;
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
				if (mUseSingleColor)
				{
					ofSetColor(r, g, b);
				}
				else
				{
					if (i%3 == 0)
					{
						ofSetColor( ramColor::BLUE_DEEP );
					}
					else if (i%3 == 1)
					{
						ofSetColor( ramColor::BLUE_NORMAL );
					}
					else
					{
						ofSetColor( ramColor::BLUE_LIGHT );
					}
				}
				
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
