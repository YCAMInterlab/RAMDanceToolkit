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
}

void sectionSet::addScene(string scene, bool displayX, bool displayY)
{
	sceneSet ss;
	ss.name = scene;
	ss.displayX = displayX;
	ss.displayY = displayY;
	sceneList.push_back(ss);
}

void sectionSet::addTuneF(string scene, string type, float value)
{
	tuneSet ts;
	ts.scene = scene;
	ts.type = OFX_UI_WIDGET_SLIDER_H;
	ts.typeName = type;
	ts.valF = value;
	tuneList.push_back(ts);
}

void sectionSet::addTuneT(string scene, string type, bool value)
{
	tuneSet ts;
	ts.scene = scene;
	ts.type = OFX_UI_WIDGET_TOGGLE;
	ts.typeName = type;
	ts.valT = value;
	tuneList.push_back(ts);
}

void sectionSet::addTuneB(string scene, string type)
{
	tuneSet ts;
	ts.scene = scene;
	ts.type = OFX_UI_WIDGET_BUTTON;
	ts.typeName = type;
	tuneList.push_back(ts);
}

void sectionSet::addExtractor(string scene, string actor, Joint node)
{
	extSet ex;
	ex.scene = scene;
	ex.actor = actor;
	ex.jt = node;
	extList.push_back(ex);
}

void sectionSet::doSection()
{
	if (needSceneClear) _scene->clearScene();
	if (needExtClear)
	{
		for (int i = 0;i < extList.size();i++)
			_scene->clearExtractor(extList[i].scene);
	}
	
	//シーン登録
	for (int i = 0;i < sceneList.size();i++)
	{
		if (sceneList[i].displayX)
			_scene->setScene(sceneList[i].name, true , true, true);
		
		if (sceneList[i].displayY)
			_scene->setScene(sceneList[i].name, false, true, true);
	}
	
	//Extractorの登録
	for(int i = 0;i < extList.size();i++)
		_scene->setExtractor(extList[i].scene, extList[i].actor, extList[i].jt);
	
	//Tuneの呼び出し
	for (int i = 0;i < tuneList.size();i++)
	{
		if (tuneList[i].type == OFX_UI_WIDGET_BUTTON)
			_scene->setButtonTune(tuneList[i].scene, tuneList[i].typeName);
		
		if (tuneList[i].type == OFX_UI_WIDGET_TOGGLE)
			_scene->setToggleTune(tuneList[i].scene, tuneList[i].typeName, tuneList[i].valT);
		
		if (tuneList[i].type == OFX_UI_WIDGET_SLIDER_H)
			_scene->setFloatTune(tuneList[i].scene, tuneList[i].typeName, tuneList[i].valF);
	}
}