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
	ofTranslate(850, 500);
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
    sceneCon->clearScene();
    
    sceneCon->setScene(SCENE_CAMERA, true, false, false);
    sceneCon->setScene(SCENE_CAMERA, true, false, false);
    sceneCon->setScene(SCENE_ACTOR, false, false, false);
    sceneCon->setScene(SCENE_ACTOR, false, false, false);
    
	if ((m.getArgAsString(0).substr(0,3) == "dpH") ||
		(m.getArgAsString(0).substr(0,5) == "dpVis"))//箱庭の分配処理
	{
		string sceneName = m.getArgAsString(0);
		
		switchHakoniwa(sceneName, m.getArgAsInt32(1), m.getArgAsInt32(2), m.getArgAsInt32(3));
		
	}else{
		
        //箱庭も消しておく
        string hakos[] = {HAKO_MAGPENDULUM, HAKO_WORM, HAKO_SERVOPENDULUM, HAKO_STRUGGLE, HAKO_STAGE, HAKO_SANDSTORM, HAKO_GEAR, HAKO_TORNADO};
        string hakoh[] = {HAKO_H_MAGPENDULUM, HAKO_H_WORM, HAKO_H_SERVOPENDULUM, HAKO_H_STRUGGLE, HAKO_H_STAGE, HAKO_H_SANDSTORM, HAKO_H_GEAR, HAKO_H_TORNADO};
        for (int i = 0;i < 8;i++)
        {
            sceneCon->disableScene(hakos[i], true);
            sceneCon->disableScene(hakoh[i], true);
            sceneCon->disableScene(hakos[i], false);
            sceneCon->disableScene(hakoh[i], false);
        }
        
		//通常のシーンの扱い...４−５,6に送る
		string sceneName = m.getArgAsString(0);
		bool sceneEnable = m.getArgAsInt32(1);
		bool ViewX = m.getArgAsInt32(2);
		bool ViewY = m.getArgAsInt32(3);
        
		if (sceneEnable)
		{
			sceneCon->setScene(sceneName, true, false, true);
			sceneCon->setScene(sceneName, false, false, true);
		}else{
			sceneCon->disableScene(sceneName, true);
			sceneCon->disableScene(sceneName, false);
		}
        
        if (sceneName == SCENE_METABALL)	callSection("fromMaster-metaball");
        if (sceneName == SCENE_UPSIDE)		callSection("fromMaster-upsidedown");
		if (sceneName == SCENE_LINE)		callSection("fromMaster-line_ext");
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
	
	cameraCon->setCameraSlot(VisName, enable, A, B);
	
	if (enable)
	{
		//マスターからの受けは2-3-6ディスプレイに出力
		bool allowed23 = true;//2-3出しの許可
		
		string hakos[] = {HAKO_MAGPENDULUM, HAKO_WORM, HAKO_SERVOPENDULUM, HAKO_STRUGGLE, HAKO_STAGE, HAKO_SANDSTORM, HAKO_GEAR, HAKO_TORNADO};
		string hakoh[] = {HAKO_H_MAGPENDULUM, HAKO_H_WORM, HAKO_H_SERVOPENDULUM, HAKO_H_STRUGGLE, HAKO_H_STAGE, HAKO_H_SANDSTORM, HAKO_H_GEAR, HAKO_H_TORNADO};
		
		for (int i = 0;i < 8;i++)
		{
			sceneCon->disableScene(hakos[i], true);
			sceneCon->disableScene(hakoh[i], true);
			sceneCon->disableScene(hakos[i], false);
			sceneCon->disableScene(hakoh[i], false);
		}
		sceneCon->disableScene(HAKO_RAWCAM, true);
		sceneCon->disableScene(HAKO_RAWCAM, false);
		
		sceneCon->setScene(hakoName, false, false, false);
        callHakoniwaPreset(VisName);
        callHakoniwaPreset(VisName);
	}
	else
	{//箱庭からのDisable処理
        
		sceneCon->disableScene(hakoName, true);
		sceneCon->disableScene(hakoName, false);
		sceneCon->disableScene(VisName, true);
		sceneCon->disableScene(VisName, false);
	}

}

void dpConductor::listSection(ofxOscMessage m)
{
	ofxOscSender sender;
	sender.setup(m.getRemoteIp(), 24800);
	
	for (int i = 0;i < sections.size();i++)
	{
		ofxOscMessage ret;
		ret.setAddress("/dpc/queList");
		ret.addStringArg(sections[i]->sectionName);
		sender.sendMessage(ret);
	}
}

void dpConductor::callHakoniwaPreset(string scene)
{
	sceneCon->clearExtractor(scene);

	if (scene == HAKO_STAGE)
	{
		sceneCon->setScene(scene, true, false, true);
		sceneCon->setScene(scene, false, true, true);
	}
	if (scene == HAKO_TORNADO)
	{
		sceneCon->disableScene(scene, true);
		sceneCon->disableScene(scene, false);
        sceneCon->setScene(HAKO_TORNADO, false, false, true);
        sceneCon->setScene(HAKO_RAWCAM, false, false, true);
        sceneCon->clearExtractor(HAKO_H_TORNADO);
        sceneCon->setExtractor(HAKO_H_TORNADO, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
        sceneCon->setExtractor(HAKO_H_TORNADO, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
        sceneCon->setExtractor(HAKO_H_TORNADO, ACTOR_SHIMAJI, JOINT_LEFT_HIP);
		sceneCon->setActorView(false, false, true, false);
	}
#pragma mark TODO: ギアビズ
	if (scene == HAKO_GEAR)
	{
        sceneCon->setScene(HAKO_RAWCAM, true, false, true);
        sceneCon->setScene(HAKO_RAWCAM, false, true, true);
		
		sceneCon->clearExtractor(HAKO_H_GEAR);
		sceneCon->setExtractor(HAKO_H_GEAR, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
		sceneCon->setExtractor(HAKO_H_GEAR, ACTOR_ANDO, JOINT_LEFT_ELBOW);
		sceneCon->setExtractor(HAKO_H_GEAR, ACTOR_ANDO, JOINT_RIGHT_KNEE);
		sceneCon->setExtractor(HAKO_H_GEAR, ACTOR_ANDO, JOINT_RIGHT_KNEE);
		sceneCon->setActorView(false, false, false, false);
	}
	if (scene == HAKO_STRUGGLE)
	{
		sceneCon->setScene(HAKO_STRUGGLE, true, false, true);
		sceneCon->setScene(HAKO_STRUGGLE, false, true, true);
        sceneCon->clearExtractor(HAKO_H_STRUGGLE);
		string act[] = {ACTOR_ANDO, ACTOR_KOJIRI, ACTOR_SHIMAJI, ACTOR_MIYASHITA};
		for (int i = 0;i < 4;i++)
		{
			sceneCon->setExtractor(HAKO_H_STRUGGLE, act[i], JOINT_LEFT_KNEE);
			sceneCon->setExtractor(HAKO_H_STRUGGLE, act[i], JOINT_RIGHT_KNEE);
		}
	}
	if (scene == HAKO_MAGPENDULUM)
	{
        sceneCon->setScene(SCENE_ACTOR, false, false, true);
		sceneCon->setScene(HAKO_MAGPENDULUM, true, false, true);
        sceneCon->setToggleTune(SCENE_ACTOR, "drawFloor", false);

        sceneCon->clearExtractor(HAKO_H_MAGPENDULUM);
        
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_HIP);
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_RIGHT_ANKLE);
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_LEFT_WRIST);
        sceneCon->setExtractor(HAKO_H_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
		
		sceneCon->setActorView(false, true, true, true);
	}
	if (scene == HAKO_WORM)
	{
        sceneCon->setScene(HAKO_RAWCAM, false, true, true);
        sceneCon->clearExtractor(HAKO_H_WORM);
		sceneCon->setExtractor(HAKO_H_WORM, ACTOR_ANDO, JOINT_RIGHT_WRIST);
		sceneCon->setExtractor(HAKO_H_WORM, ACTOR_ANDO, JOINT_LEFT_KNEE);
	}
	if (scene == HAKO_SERVOPENDULUM)
	{
		sceneCon->setScene(HAKO_SERVOPENDULUM, true, false, true);
		sceneCon->setScene(HAKO_SERVOPENDULUM, false, true, true);
		sceneCon->clearExtractor(HAKO_H_SERVOPENDULUM);
		sceneCon->setExtractor(HAKO_H_SERVOPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
		sceneCon->setExtractor(HAKO_H_SERVOPENDULUM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	}
	if (scene == HAKO_SANDSTORM)
	{
		sceneCon->setScene(HAKO_SANDSTORM, true, false, true);
		sceneCon->setScene(HAKO_SANDSTORM, false, true, true);
        sceneCon->clearExtractor(HAKO_H_SANDSTORM);
        
		sceneCon->setExtractor(HAKO_H_SANDSTORM, ACTOR_SHIMAJI, JOINT_NECK);
		sceneCon->setExtractor(HAKO_H_SANDSTORM, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
		sceneCon->setExtractor(HAKO_H_SANDSTORM, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	}
	
//	if (scene == HAKO_SANDSTORM)
//	{
//		sceneCon->setScene(HAKO_SANDSTORM, false, false, true);
//		sceneCon->setScene(HAKO_SANDSTORM, true, true, true);
//		sceneCon->setExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_NECK);
//		sceneCon->setExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
//		sceneCon->setExtractor(HAKO_SANDSTORM, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
//		sceneCon->setToggleTune(SCENE_ACTOR, "V_"+ACTOR_ANDO		, false);
//		sceneCon->setToggleTune(SCENE_ACTOR, "V_"+ACTOR_KOJIRI	, true);
//		sceneCon->setToggleTune(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI	, false);
//		sceneCon->setToggleTune(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
//	}
}