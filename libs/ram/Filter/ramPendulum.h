// 
// ramPendulum.h - RAMDanceToolkit
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

class ramPendulum : public ramBaseFilter
{
	float mFreq;

	bool axis_x, axis_y, axis_z;

public:

	ramPendulum() : mFreq(30.0), axis_x(0), axis_y(1), axis_z(0)
	{
		randoms.clear();
		
		for (int i = 0; i < ramActor::NUM_JOINTS; i++)
			randoms.push_back(ofRandom(1.0, 5.0));
		
		periods.clear();
		periods.assign(ramActor::NUM_JOINTS, float());
	}

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());

		ramGetGUI().addSlider("Frequency", 0.00, 100.0, &mFreq);

		ramGetGUI().addSeparator();

		ramGetGUI().addToggle("Pendulum axis: X", &axis_x);
		ramGetGUI().addToggle("Pendulum axis: Y", &axis_y);
		ramGetGUI().addToggle("Pendulum axis: Z", &axis_z);
	}

	const ramNodeArray& filter(const ramNodeArray &nodeArray)
	{
		const float t = ofGetElapsedTimef();

		ofVec3f axis(axis_x, axis_y, axis_z);

		for (int i = 0; i < periods.size(); i++)
			periods.at(i) = sin(t * randoms.at(i)) * mFreq;

		processedArray = nodeArray;

		for (int i = 0; i < processedArray.getNumNode(); i++)
		{
			ramNode &node = processedArray.getNode(i);
			ofMatrix4x4 m = node.getLocalTransformMatrix();
			const ofQuaternion qt = ofQuaternion(periods.at(i), axis);

			node.setTransformMatrix(m * qt);
		}

		return processedArray;
	}

	inline string getName() const { return "ramPendulum"; };

protected:

	ramNodeArray processedArray;
	vector<float> periods;
	vector<float> randoms;

};