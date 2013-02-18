#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramUpsideDown : public ramBaseFilter
{
public:
	
	ramUpsideDown() : offset(-3.0f) {}
	
    inline void setOffset(float y) { offset = y; }
    inline float getOffset() const { return offset; }
	
	const ramNodeArray& get(size_t index) const { return hangedNodes; }
	size_t getSize() const { return 1; }
	
	inline const string getName() { return "ramUpsideDown"; };
	
protected:
	
	ramNodeArray hangedNodes;
    float offset;
	
	const ramNodeArray& filter(const ramNodeArray& src)
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

};
