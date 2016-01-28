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
	
	sceneCon->setup(ADDR_RDTK_1, ADDR_RDTK_2);
	cameraCon->setup(ADDR_CAMERAUNIT);
	
	setSections();
	
	for (int i = 0;i < sections.size();i++)
	{
		sections[i]->init(sceneCon, cameraCon, envCon);
	}
	
	for (int i = 0;i < 6;i++) display[i] = "Not assigned";
}

void dpConductor::update()
{
	display[0] = sceneCon->scene_rdtk1_a;//1
	
	display[1] = sceneCon->scene_rdtk2_a;//2
	display[2] = sceneCon->scene_rdtk2_a;//3
	
	display[3] = sceneCon->scene_rdtk2_b;//4
	display[4] = sceneCon->scene_rdtk2_b;//5
	
	display[5] = sceneCon->scene_rdtk1_b;//6
}

void dpConductor::draw()
{
	ofPushMatrix();
	ofTranslate(500, 500);
	ofNoFill();
	
	ofRect(125, 0, 200, 30);
	ofDrawBitmapString("Disp:1", 130, -2);
	ofDrawBitmapString(display[0], 130, 20);
	
	ofRect(0, 50, 200, 30);
	ofDrawBitmapString("Disp:2", 5, 48);
	ofDrawBitmapString(display[1], 5, 70);

	ofRect(250, 50, 200, 30);
	ofDrawBitmapString("Disp:3", 255, 48);
	ofDrawBitmapString(display[2], 255, 70);

	ofRect(0, 100, 200, 30);
	ofDrawBitmapString("Disp:4",  5, 98);
	ofDrawBitmapString(display[3], 5, 120);

	ofRect(250, 100, 200, 30);
	ofDrawBitmapString("Disp:5", 255, 98);
	ofDrawBitmapString(display[4], 255, 120);

	ofRect(125, 150, 200, 30);
	ofDrawBitmapString("Disp:6", 130, 148);
	ofDrawBitmapString(display[5], 130, 170);

	ofFill();
	ofPopMatrix();
}

void dpConductor::receiveMasterHakoniwa(ofxOscMessage m)
{
	if ((m.getArgAsString(0).substr(0,3) == "dpH") ||
		(m.getArgAsString(0).substr(0,5) == "dpVis"))//箱庭の分配処理
	{
		string sceneName = m.getArgAsString(0);
		
		switchHakoniwa(sceneName, m.getArgAsInt32(1), m.getArgAsInt32(2), m.getArgAsInt32(3));
		
	}else{
		
		//通常のシーンの扱い
		string sceneName = m.getArgAsString(0);
		bool sceneEnable = m.getArgAsInt32(1);
		bool ViewX = m.getArgAsInt32(2);
		bool ViewY = m.getArgAsInt32(3);
		
		if (sceneEnable)
		{
			sceneCon->setScene(sceneName, true, ViewY, ViewY);
			sceneCon->setScene(sceneName, false, ViewX, ViewY);
		}else{
			sceneCon->disableScene(sceneName, true);
			sceneCon->disableScene(sceneName, false);
		}
	}
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

void dpConductor::switchHakoniwa(string nameHakoniwa, bool enable, bool A, bool B)
{
	string baseName;
	string hakoName;
	string VisName;
	
	if (nameHakoniwa.substr(0,3) == "dpH")
		baseName = nameHakoniwa.substr(3, nameHakoniwa.length() - 3);
	
	if (nameHakoniwa.substr(0,5) == "dpVis")
		baseName = nameHakoniwa.substr(5, nameHakoniwa.length() - 5);
	
	hakoName = "dpH" + baseName;
	VisName = "dpVis" + baseName;
	
	cout << "SWH::hakoniwa name : " << hakoName << endl;
	cout << "SWH::hakoVis  name : " << VisName << endl;
	
	cameraCon->setCameraSlot(hakoName, enable, A, B);
	
	if (enable)
	{
		sceneCon->setScene(hakoName, true, false, false);
		sceneCon->setScene(hakoName, true, false, false);
		sceneCon->setScene(VisName, false, A, B);
		sceneCon->setScene(VisName, true, A, B);
	}
	else
	{//箱庭からのDisable処理
		sceneCon->disableScene(hakoName, true);
		sceneCon->disableScene(hakoName, false);
		sceneCon->disableScene(VisName, true);
		sceneCon->disableScene(VisName, false);
		
	}
}