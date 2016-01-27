//
//  dpConductor.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#include "dpConductor.h"

void dpConductor::setup()
{
	sceneCon	= ofPtr<sceneController>	(new sceneController());
	cameraCon	= ofPtr<cameraUnitManager>	(new cameraUnitManager());
	envCon		= ofPtr<environmentManager>	(new environmentManager());
	
	sceneCon->setup("127.0.0.1", "192.168.20.3");
	
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

void dpConductor::setSections()
{
	ofPtr<sectionSet> ns = newSection();
	ns->sectionName = "LineKojiri";
	ns->addScene("Line_ext", true, true);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_LEFT_ELBOW);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_LEFT_SHOULDER);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_RIGHT_ELBOW);
	ns->addExtractor("Line_ext", "Richi_2012-09-01_16-55-24", JOINT_RIGHT_SHOULDER);
	ns->addTuneF("Line_ext", "Curve0", 100);
	ns->addTuneF("Line_ext", "ext_to0", 500);
}

ofPtr<sectionSet> dpConductor::newSection()
{
	ofPtr<sectionSet> ss = ofPtr<sectionSet>(new sectionSet);
	sections.push_back(ss);
	return ss;
}