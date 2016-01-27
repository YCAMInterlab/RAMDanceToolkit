//
//  sectionSet.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#include "sectionSet.h"

void sectionSet::init(ofPtr<sceneController> scn, ofPtr<cameraUnitManager> cam, ofPtr<environmentManager> env)
{
	_scene = scn;
	_camera = cam;
	_environment = env;
	
	sceneList.clear();
}

void sectionSet::addScene(string scene, bool displayX, bool displayY)
{
	sceneSet ss;
	ss.name = scene;
	ss.displayX = displayX;
	ss.displayY = displayY;
	sceneList.push_back(ss);
}

void sectionSet::addExtractor(string scene, string actor, Joint node)
{
	extSet ex;
	ex.actor = actor;
	ex.jt = node;
	extList.push_back(ex);
}

void sectionSet::doSection()
{
	if (needSceneClear) _scene->clearScene();
	
	for (int i = 0;i < sceneList.size();i++)
	{
		if (sceneList[i].displayX)
			_scene->setScene(sceneList[i].name, true , true, true);
		
		if (sceneList[i].displayY)
			_scene->setScene(sceneList[i].name, false, true, true);
	}
	
	for(int i = 0;i < extList.size();i++)
	{

	}
}