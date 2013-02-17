#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramUpsideDown : public ramBaseFilter
{
	
public:
	ramUpsideDown() : offset(-3.0f) {}
	
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
		
		m.translate(0.0f, -h-offset, 0.0f);
		m.rotate(180, 0.0, 0.0, 1.0);
		
		rootNode.setTransformMatrix(m);
		
		return hangedNodes;
	}
	
    inline void setOffset(float y) { offset = y; }
    inline float getOffset() const { return offset; }
	inline const ramNodeArray& getResult() { return hangedNodes; }
	inline const string getName() { return "ramUpsideDown"; };
	
protected:
	
	ramNodeArray hangedNodes;
    float offset;
	
};
