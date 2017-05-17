// 
// ramUpsideDown.h - RAMDanceToolkit
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
	class UpsideDown : public BaseFilter
	{
	public:
		
		string getName() const { return "ramUpsideDown"; };
		
		UpsideDown() : offset(-3.0f) {}
		
		inline void setOffset(float y) { offset = y; }
		inline float getOffset() const { return offset; }
		
	protected:
		
		NodeArray hangedNodes;
		float offset;
		
		const NodeArray& filter(const NodeArray& src)
		{
			hangedNodes = src;
			
			float h = 0;
			for (int i = 0; i < src.getNumNode(); i++)
			{
				float nh = src.getNode(i).getGlobalPosition().y;
				if (nh > h) h = nh;
			}
			
			// rootnode
			Node &rootNode = hangedNodes.getNode(Actor::JOINT_HIPS);
			ofMatrix4x4 m = rootNode.getTransformMatrix();
			
			m.translate(0.0f, -h - offset, 0.0f);
			m.rotate(180, 0.0, 0.0, 1.0);
			
			rootNode.setTransformMatrix(m);
			
			return hangedNodes;
		}
		
	};
}

typedef rdtk::UpsideDown RAMDEPRECATED(ramUpsideDown);
