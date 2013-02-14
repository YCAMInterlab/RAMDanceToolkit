#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramHangedMan : public ramBaseFilter
{
	
public:
	ramHangedMan() {}
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		hangedNodes = src;
		
		float h = 0;
		for (int i=0; i<src.getNumNode(); i++)
		{
			float nh = src.getNode(i).getGlobalPosition().y;
			if (nh > h) h = nh;
		}
		
		// rootnode
		ramNode &rootNode = hangedNodes.getNode(ramActor::JOINT_HIPS);
		ofMatrix4x4 m = rootNode.getTransformMatrix();
		
		m.translate(0.0f, -h-3.0f, 0.0f);
		m.rotate(180, 0.0, 0.0, 1.0);
		
		rootNode.setTransformMatrix(m);
		
		return hangedNodes;
	}
	
	inline const ramNodeArray& getResult() { return hangedNodes; }
	inline const string getName() { return "ramHangedMan"; };
	
protected:
	
	ramNodeArray hangedNodes;
	
};
