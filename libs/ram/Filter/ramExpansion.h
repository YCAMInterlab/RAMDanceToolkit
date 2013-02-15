#pragma once

#include "ramMain.h"
#include "ramBaseFilter.h"
#include "ramControlPanel.h"

class ramExpansion : public ramBaseFilter
{
	float kExpandMax;
	float mExpand;
	
public:
	
	ramExpansion() : mExpand(1.0), kExpandMax(3.0) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();

		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Expand", 0.0, kExpandMax, &mExpand, gui.kLength, gui.kDim);
	}
	
	const ramNodeArray& update(const ramNodeArray &nodeArray)
	{
		expandedArray = nodeArray;
		
		for (int i=0; i<expandedArray.getNumNode(); i++)
		{
			const ramNode &node = nodeArray.getNode(i);
			ramNode &expandedNode = expandedArray.getNode(i);
			
			ofMatrix4x4 m = node.getGlobalTransformMatrix();
			ofMatrix4x4 mm = m;
			
			mm.preMultRotate(m.getRotate());
			mm.preMultTranslate(m.getTranslation() * mExpand);
			
			expandedNode.setGlobalPosition( mm.preMult(ofVec3f(0, 0, 0)) );
		}
		
		return expandedArray;
	}
	
	inline const ramNodeArray& getResult() { return expandedArray; }
	inline const string getName() { return "ramExpansion"; };
	
protected:
	ramNodeArray expandedArray;
	
};