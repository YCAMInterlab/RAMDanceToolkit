// 
// ramExpansion.h - RAMDanceToolkit
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
#include "ramControlPanel.h"

#include "ramLowPassFilter.h"

class ramExpansion : public ramBaseFilter
{
	float kExpandMax;
	float mExpand;

public:

	string getName() const { return "ramExpansion"; };

	ramExpansion() : mExpand(1.0), kExpandMax(10.0) {}

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());
		ramGetGUI().addSlider("Expand", 0.0, kExpandMax, &mExpand);
	}

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		expandedArray = src;

		for (int i = 0; i < expandedArray.getNumNode(); i++)
		{
			const ramNode &node = src.getNode(i);
			ramNode &expandedNode = expandedArray.getNode(i);

			ofMatrix4x4 m = node.getGlobalTransformMatrix();
			ofMatrix4x4 mm = m;

			mm.preMultRotate(m.getRotate());
			mm.preMultTranslate(m.getTranslation() * mExpand);

			expandedNode.setGlobalPosition(mm.preMult(ofVec3f(0, 0, 0)));
		}

		return expandedArray;
	}
    
    inline void setExpand(float ratio) { mExpand = ratio; }

protected:

	ramNodeArray expandedArray;

};