//
//  dpConductor.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#include "dpConductor.h"

void dpConductor::setSections()
{
	ofPtr<sectionSet> ns = newSection();
	ns->sectionName = "LineKojiri";
	ns->addScene("Donuts", true, true);
	ns->needExtClear = true;
	ns->needSceneClear = true;
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_LEFT_ELBOW);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_LEFT_SHOULDER);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_RIGHT_ELBOW);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_RIGHT_SHOULDER);
	ns->addTuneF("Line_ext", "Curve0", 100);
	ns->addTuneF("Line_ext", "ext_to0", 500);
	
	ns = newSection();
	ns->sectionName = "testSection";
	ns->addScene("Donuts", true, true);
	ns->needExtClear = true;
	ns->needSceneClear = true;
	ns->addExtractor("Line_ext", "kojiri", JOINT_LEFT_ELBOW);
	ns->addExtractor("Line_ext", "kojiri", JOINT_LEFT_SHOULDER);
	ns->addExtractor("Line_ext", "kojiri", JOINT_RIGHT_ELBOW);
	ns->addExtractor("Line_ext", "kojiri", JOINT_RIGHT_SHOULDER);
	ns->addTuneF("Line_ext", "Curve0", 100);
	ns->addTuneF("Line_ext", "ext_to0", 500);
}

void dpConductor::setup()
{
	sceneCon	= ofPtr<sceneController>	(new sceneController());
	cameraCon	= ofPtr<cameraUnitManager>	(new cameraUnitManager());
	envCon		= ofPtr<environmentManager>	(new environmentManager());
	
	sceneCon->setup("192.168.20.2", "192.168.20.3");
	
	setSections();
	
	for (int i = 0;i < sections.size();i++)
	{
		sections[i]->init(sceneCon, cameraCon, envCon);
	}
	
	receiver.setup(12400);
}

void dpConductor::update()
{
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		//マスター箱庭への割り振り
		if (m.getAddress() == "/ram/set_scene")
		{
			if (m.getArgAsString(0).substr(0,5) == "dpVis")
			{
				string scene = m.getArgAsString(0);
				cameraCon->setCameraSlot(scene,
										 m.getArgAsInt32(0),
										 m.getArgAsInt32(1),
										 m.getArgAsInt32(2));
			}
		}
	}
}

void dpConductor::draw()
{
	
}

void dpConductor::callSection(string name)
{
	for (int i = 0;i < sections.size();i++)
	{
		if (sections[i]->sectionName == name)
			sections[i]->doSection();
	}
}

ofPtr<sectionSet> dpConductor::newSection()
{
	ofPtr<sectionSet> ss = ofPtr<sectionSet>(new sectionSet);
	sections.push_back(ss);
	return ss;
}