#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramTranslate : public ramBaseFilter
{
public:
	
	const string getName() { return "ramTranslate"; };
	
	void setTranslate(const ofVec3f& v) { translate = v; }
	void setTranslate(float x, float y, float z) { setTranslate(ofVec3f(x, y, z)); }
	const ofVec3f& getTranslate() { return translate; }
	
protected:
	
	ramNodeArray result;
	ofVec3f translate;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		result = src;
		
		for (int i = 0; i < result.getNumNode(); i++)
		{
			ramNode &node = result.getNode(i);
			if (node.getParent() == NULL)
			{
				node.move(translate);
			}
		}
		
		return result;
	}

};

class ramRotate : public ramBaseFilter
{
public:
	
	const string getName() { return "ramRotate"; };
	
	void setRotate(const ofVec3f& v) { rotate = ofQuaternion(v.y, ofVec3f(0, 1, 0), v.x, ofVec3f(1, 0, 0), v.z, ofVec3f(0, 0, 1)); }
	void setRotate(float rx, float ry, float rz) { setRotate(ofVec3f(rx, ry, rz)); }
	void setRotate(const ofQuaternion& quat) { rotate = quat; }
	
	const ofQuaternion& getRotate() { return rotate; }
	
protected:
	
	ramNodeArray result;
	ofQuaternion rotate;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		result = src;
		
		for (int i = 0; i < result.getNumNode(); i++)
		{
			ramNode &node = result.getNode(i);
			if (node.getParent() == NULL)
			{
				node.rotate(rotate);
			}
		}
		
		return result;
	}

};


class ramNodeTransform : public ramBaseFilter, public ofMatrix4x4
{
public:
	
	const string getName() { return "ramTranslate"; };
	
protected:
	
	ramNodeArray result;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		result = src;
		
		for (int i = 0; i < result.getNumNode(); i++)
		{
			ramNode &node = result.getNode(i);
			if (node.getParent() == NULL)
			{
				node.setTransformMatrix(node.getLocalTransformMatrix() * *this);
			}
		}
		
		return result;
	}
};

class ramNodeLocalTransform : public ramBaseFilter, public ofMatrix4x4
{
public:
	
	const string getName() { return "ramNodeLocalTransform"; };
	
protected:
	
	ramNodeArray result;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		result = src;
		
		for (int i = 0; i < result.getNumNode(); i++)
		{
			ramNode &node = result.getNode(i);
			if (node.getParent() == NULL)
			{
				node.move(getTranslation());
				node.rotate(getRotate());
			}
		}
		
		return result;
	}
};


