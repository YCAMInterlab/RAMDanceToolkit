#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramTranslate : public ramBaseFilter
{
	
public:
	ramTranslate() {}
	
	void setTranslate(const ofVec3f& v) { translate = v; }
	void setTranslate(float x, float y, float z) { setTranslate(ofVec3f(x, y, z)); }
	const ofVec3f& getTranslate() { return translate; }
	
	const ramNodeArray& update(const ramNodeArray& src)
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
	
	inline const ramNodeArray& getResult() { return result; }
	inline const string getName() { return "ramTranslate"; };
	
protected:
	
	ramNodeArray result;
	ofVec3f translate;
};

class ramRotate : public ramBaseFilter
{
	
public:
	ramRotate() {}
	
	void setRotate(const ofVec3f& v) { rotate = ofQuaternion(v.y, ofVec3f(0, 1, 0), v.x, ofVec3f(1, 0, 0), v.z, ofVec3f(0, 0, 1)); }
	void setRotate(float rx, float ry, float rz) { setRotate(ofVec3f(rx, ry, rz)); }
	void setRotate(const ofQuaternion& quat) { rotate = quat; }
	
	const ofQuaternion& getRotate() { return rotate; }
	
	const ramNodeArray& update(const ramNodeArray& src)
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
	
	inline const ramNodeArray& getResult() { return result; }
	inline const string getName() { return "ramRotate"; };
	
protected:
	
	ramNodeArray result;
	ofQuaternion rotate;
};


class ramNodeTransform : public ramBaseFilter, public ofMatrix4x4
{
	
public:
	
	const ramNodeArray& update(const ramNodeArray& src)
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
	
	inline const ramNodeArray& getResult() { return result; }
	inline const string getName() { return "ramTranslate"; };
	
protected:
	
	ramNodeArray result;
};

class ramNodeLocalTransform : public ramBaseFilter, public ofMatrix4x4
{
	
public:
	
	const ramNodeArray& update(const ramNodeArray& src)
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
	
	inline const ramNodeArray& getResult() { return result; }
	inline const string getName() { return "ramNodeLocalTransform"; };
	
protected:
	
	ramNodeArray result;
};


