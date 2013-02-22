#pragma once

#include "ramMain.h"
#include "ramBaseFilter.h"
#include "ramControlPanel.h"

#include "ramLowPassFilter.h"

class ramExpansion : public ramBaseFilter
{
	float kExpandMax;
	float mExpand;
	
public:
	
	ramExpansion() : mExpand(1.0), kExpandMax(3.0) {}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();

		gui.addSection(getName());
		gui.addSlider("Expand", 0.0, kExpandMax, &mExpand);
	}
	
	const ramNodeArray& get(size_t index) const { return expandedArray; }
	size_t getSize() const { return 1; }
	
	inline const string getName() { return "ramExpansion"; };
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		expandedArray = src;
		
		for (int i=0; i<expandedArray.getNumNode(); i++)
		{
			const ramNode &node = src.getNode(i);
			ramNode &expandedNode = expandedArray.getNode(i);
			
			ofMatrix4x4 m = node.getGlobalTransformMatrix();
			ofMatrix4x4 mm = m;
			
			mm.preMultRotate(m.getRotate());
			mm.preMultTranslate(m.getTranslation() * mExpand);
			
			expandedNode.setGlobalPosition( mm.preMult(ofVec3f(0, 0, 0)) );
		}
		
		return expandedArray;
	}
	
protected:
	
	ramNodeArray expandedArray;
	
};