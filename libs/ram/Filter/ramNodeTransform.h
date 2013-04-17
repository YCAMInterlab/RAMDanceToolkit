// 
// ramNodeTransform.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ramMain.h"

#include "ramBaseFilter.h"

class ramTranslate : public ramBaseFilter
{
public:

	string getName() const { return "ramTranslate"; };

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

	string getName() const { return "ramRotate"; };

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

	string getName() const { return "ramTranslate"; };

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

	string getName() const { return "ramNodeLocalTransform"; };

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