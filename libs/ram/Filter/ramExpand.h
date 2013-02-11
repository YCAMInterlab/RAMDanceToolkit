#pragma once

#include "ramMain.h"
#include "ramFilter.h"
#include "ramControlPanel.h"

class ramExpand : public ramFilter
{
	float expand;
	
public:
	
	ramExpand() : expand(1.0) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();

		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Expand", 0.01, 3.0, &expand, gui.kLength, gui.kDim);
	}
	
	const ramNodeArray& update(const ramNodeArray &nodeArray)
	{
		expandedArray = nodeArray;
		
		
		for (int i=0; i<expandedArray.getNumNode(); i++)
		{
			const ramNode &node = nodeArray.getNode(i);
			ramNode &expandedNode = expandedArray.getNode(i);
			
			ofMatrix4x4 m = node.getGlobalTransformMatrix();
			m = m * m;
			m.scale(expand, expand, expand);
			expandedNode.setPosition( m.preMult(ofVec3f(0, 0, 0)) );
		}
		
		return expandedArray;
	}
	
	inline const ramNodeArray& getResult() { return expandedArray; }
	inline const string getName() { return "ramExpand"; };
	
protected:
	ramNodeArray expandedArray;
	
};