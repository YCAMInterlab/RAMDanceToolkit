#pragma once

#include "ramMain.h"
#include "ramFilter.h"
#include "ramControlPanel.h"

class ramPendulum : public ramFilter
{
	float mFreq;
	ofVec3f mAxis;
	
public:
	
	ramPendulum() : mFreq(30.0), mAxis(ofVec3f(0.0, 1.0, 0.0)) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();

		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Frequency", 0.00, 100.0, &mFreq, gui.kLength, gui.kDim);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addToggle("Pendulum axis: X", false, 10, 10);
		panel->addToggle("Pendulum axis: Y", true,  10, 10);
		panel->addToggle("Pendulum axis: Z", false, 10, 10);
		
		ofAddListener(panel->newGUIEvent, this, &ramPendulum::onValueChanged);
	}
	
	void setup()
	{
		randoms.clear();
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			randoms.push_back( ofRandom(1.0, 5.0) );
		
		periods.clear();
		periods.assign( ramActor::NUM_JOINTS, float() );
	}
	
	const ramNodeArray& update(const ramNodeArray &nodeArray)
	{
		const float t = ofGetElapsedTimef();
		
		for (int i=0; i<periods.size(); i++)
			periods.at(i) = sin( t*randoms.at(i) ) * mFreq;
		
		processedArray = nodeArray;
		
		for (int i=0; i<processedArray.getNumNode(); i++)
		{
			ramNode &node = processedArray.getNode(i);
			ofMatrix4x4 m = node.getLocalTransformMatrix();
			const ofQuaternion qt = ofQuaternion(periods.at(i), mAxis);
			
			node.setTransformMatrix(m * qt);
		}
		
		return processedArray;
	}
	
	void onValueChanged(ofxUIEventArgs &e)
	{
		string name = e.widget->getName();
		
		if (name == "Pendulum axis: X")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			float v = t->getValue();
			mAxis.x = v;
		}
		if (name == "Pendulum axis: Y")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			float v = t->getValue();
			mAxis.y = v;
		}
		if (name == "Pendulum axis: Z")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			float v = t->getValue();
			mAxis.z = v;
		}
	}
	
	inline const ramNodeArray& getResult() { return processedArray; }
	inline const string getName() { return "ramPendulum"; };
	
protected:
	
	ramNodeArray processedArray;
	vector<float> periods;
	vector<float> randoms;
	
};