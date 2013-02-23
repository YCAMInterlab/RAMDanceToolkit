#pragma once

#include "ramMain.h"
#include "ramBaseFilter.h"
#include "ramControlPanel.h"

class ramPendulum : public ramBaseFilter
{
	float mFreq;

	bool axis_x, axis_y, axis_z;

public:

	ramPendulum() : mFreq(30.0), axis_x(0), axis_y(1), axis_z(0) {}

	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();

		gui.addSection(getName());

		gui.addSlider("Frequency", 0.00, 100.0, &mFreq);

		gui.addSeparator();

		gui.addToggle("Pendulum axis: X", &axis_x);
		gui.addToggle("Pendulum axis: Y", &axis_y);
		gui.addToggle("Pendulum axis: Z", &axis_z);
	}

	void setup()
	{
		randoms.clear();

		for (int i = 0; i < ramActor::NUM_JOINTS; i++)
			randoms.push_back(ofRandom(1.0, 5.0));

		periods.clear();
		periods.assign(ramActor::NUM_JOINTS, float());
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

	inline const ramNodeArray& getResult() { return processedArray; }
	inline const string getName() { return "ramPendulum"; };

protected:

	ramNodeArray processedArray;
	vector<float> periods;
	vector<float> randoms;

};