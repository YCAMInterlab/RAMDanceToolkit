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

namespace rdtk{
	class Translate : public BaseFilter
	{
	public:
		
		string getName() const { return "ramTranslate"; };
		
		void setTranslate(const ofVec3f& v) { translate = v; }
		void setTranslate(float x, float y, float z) { setTranslate(ofVec3f(x, y, z)); }
		const ofVec3f& getTranslate() const { return translate; }
		
	protected:
		
		NodeArray result;
		ofVec3f translate;
		
		const NodeArray& filter(const NodeArray& src)
		{
			result = src;
			
			for (int i = 0; i < result.getNumNode(); i++)
			{
				Node &node = result.getNode(i);
				if (node.getParent() == NULL)
				{
					node.move(translate);
				}
			}
			
			return result;
		}
		
	};
	
	class Rotate : public BaseFilter
	{
	public:
		
		string getName() const { return "ramRotate"; };
		
		void setRotate(const ofVec3f& v) { rotate = ofQuaternion(v.y, ofVec3f(0, 1, 0), v.x, ofVec3f(1, 0, 0), v.z, ofVec3f(0, 0, 1)); }
		void setRotate(float rx, float ry, float rz) { setRotate(ofVec3f(rx, ry, rz)); }
		void setRotate(const ofQuaternion& quat) { rotate = quat; }
		
		const ofQuaternion& getRotate() const { return rotate; }
		
	protected:
		
		NodeArray result;
		ofQuaternion rotate;
		
		const NodeArray& filter(const NodeArray& src)
		{
			result = src;
			
			for (int i = 0; i < result.getNumNode(); i++)
			{
				Node &node = result.getNode(i);
				if (node.getParent() == NULL)
				{
					node.rotate(rotate);
				}
			}
			
			return result;
		}
		
	};
	
	class NodeTransform : public BaseFilter, public ofMatrix4x4
	{
	public:
		
		string getName() const { return "ramTranslate"; };
		
	protected:
		
		NodeArray result;
		
		const NodeArray& filter(const NodeArray& src)
		{
			result = src;
			
			for (int i = 0; i < result.getNumNode(); i++)
			{
				Node &node = result.getNode(i);
				if (node.getParent() == NULL)
				{
					node.setTransformMatrix(node.getLocalTransformMatrix() * *this);
				}
			}
			
			return result;
		}
	};
	
	class NodeLocalTransform : public BaseFilter, public ofMatrix4x4
	{
	public:
		
		string getName() const { return "ramNodeLocalTransform"; };
		
	protected:
		
		NodeArray result;
		
		const NodeArray& filter(const NodeArray& src)
		{
			result = src;
			
			for (int i = 0; i < result.getNumNode(); i++)
			{
				Node &node = result.getNode(i);
				if (node.getParent() == NULL)
				{
					node.move(getTranslation());
					node.rotate(getRotate());
				}
			}
			
			return result;
		}
	};

}

typedef rdtk::Translate			RAMDEPRECATED(ramTranslate);
typedef rdtk::Rotate				RAMDEPRECATED(ramRotate);
typedef rdtk::NodeTransform		RAMDEPRECATED(ramNodeTransform);
typedef rdtk::NodeLocalTransform	RAMDEPRECATED(ramNodeLocalTransform);
