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

void sectionSet::addScene(string scene, bool A_1, bool B_1, bool A_2, bool B_2)
{
	sceneSet ss;
	ss.name = scene;
	ss.RDTK1_A = A_1;
	ss.RDTK1_B = B_1;
	ss.RDTK2_A = A_2;
	ss.RDTK2_B = B_2;
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

void sectionSet::addEnv_camera(ofVec3f pos, ofVec3f look, float speed, float smooth)
{
	envSet ev;
	ev.envCmd = ENV_CMD_CAMERA;
	ev.camera_pos = pos;
	ev.camera_look = look;
	ev.camera_speed = speed;
	ev.camera_smooth = smooth;
	envList.push_back(ev);
}

void sectionSet::addEnv_nodeCam(string name, Joint nodeFrom, Joint nodeLook, float speed, float smooth)
{
	envSet ev;
	ev.envCmd = ENV_CMD_NODECAM;
	ev.actorName = name;
	ev.nCam_pos = nodeFrom;
	ev.nCam_look = nodeLook;
	ev.camera_speed = speed;
	ev.camera_smooth = smooth;
	envList.push_back(ev);
}

void sectionSet::addEnv_drawAct(string name, bool view)
{
	envSet ev;
	ev.envCmd = ENV_CMD_DRAWACT;
	ev.actorName = name;
	ev.drawAct = view;
	envList.push_back(ev);
}

void sectionSet::addEnv_floor(bool view)
{
	envSet ev;
	ev.envCmd = ENV_CMD_FLOOR;
	ev.viewFloor = view;
	envList.push_back(ev);
}

void sectionSet::addEnv_margine(float margine)
{
	envSet ev;
	ev.envCmd = ENV_CMD_MARGINE;
	ev.actorMargine = margine;
	envList.push_back(ev);
}

void sectionSet::addHakoniwa(string name, bool enable, bool A, bool B)
{
	hakoSet hk;
	hk.scene = name;
	hk.enable = enable;
	hk.A = A;
	hk.B = B;
	hakoList.push_back(hk);
}

void sectionSet::doSection()
{
	if (needSceneClear) _scene->clearScene();
	if (needExtClear)
	{
		for (int i = 0;i < extList.size();i++)
			_scene->clearExtractor(extList[i].scene);
	}
	
	for (int i = 0;i < hakoList.size();i++)
	{
		switchHakoniwa(hakoList[i].scene, hakoList[i].enable, hakoList[i].A, hakoList[i].B);
	}
	
	//シーン登録
	for (int i = 0;i < sceneList.size();i++)
	{
		_scene->setScene(sceneList[i].name, true , sceneList[i].RDTK1_A, sceneList[i].RDTK1_B);
		_scene->setScene(sceneList[i].name, false, sceneList[i].RDTK2_A, sceneList[i].RDTK2_B);
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
	
	for (int i = 0;i < envList.size();i++)
	{
		envSet *e = &envList[i];
		if (e->envCmd == ENV_CMD_CAMERA)
			_environment->setCameraPreset(e->camera_pos, e->camera_look,
										  e->camera_speed, e->camera_smooth);
		if (e->envCmd == ENV_CMD_DRAWACT)
			_environment->setActorDraw(e->actorName, e->drawAct);

		if (e->envCmd == ENV_CMD_FLOOR)
			_environment->setViewFloor(e->viewFloor);
		
		if (e->envCmd == ENV_CMD_MARGINE)
			_environment->setActorMargine(e->actorMargine);
		
		if (e->envCmd == ENV_CMD_DRAWACT)
			_environment->setActorDraw(e->actorName, e->drawAct);
	}
}

void sectionSet::switchHakoniwa(string nameHakoniwa, bool enable, bool A, bool B)
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
    
    if (baseName == "Stage")
    {
        cout << "send" << VisName << endl;
        _camera->setCameraSlot(VisName, enable, A, B);
    }
    else
    {
        cout << "send" << hakoName << endl;
        _camera->setCameraSlot(hakoName, enable, A, B);
    }
    
    if (enable)
    {
        _scene->setScene(hakoName, true, false, false);
        _scene->setScene(hakoName, true, false, false);
        _scene->setScene(VisName, false, A, B);
        _scene->setScene(VisName, true, A, B);
    }
    else
    {//箱庭からのDisable処理
        _scene->disableScene(hakoName, true);
        _scene->disableScene(hakoName, false);
        _scene->disableScene(VisName, true);
        _scene->disableScene(VisName, false);
    }

}