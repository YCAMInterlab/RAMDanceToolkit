//
//  Presets.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/28.
//
//

#include "dpConductor.h"

#define MIYASHITA_BIGBOX \
ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_NECK);\
ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_HIP);\
ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);\
ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_SHOULDER);\
ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_RIGHT_SHOULDER);\
ns->addTuneF(SCENE_BIGBOX, "boxScale2", 30.82);\
ns->addTuneF(SCENE_BIGBOX, "boxScale3", 82.14);\
ns->addTuneF(SCENE_BIGBOX, "boxScale4", 30.82);\
ns->addTuneF(SCENE_BIGBOX, "boxScale5", 88.3);\
ns->addTuneF(SCENE_BIGBOX, "boxScale6", 88.3);

void dpConductor::setSections()
{
	//AddScene preset (Disp 1, Disp6, Disp2-3, Disp 4-5)

	ofPtr<sectionSet> ns = newSection();//===================================================
	ns->sectionName = "Blank";
	ns->needExtClear = true;
	ns->needSceneClear = true;
	
#pragma mark - 最終プリセット群


	
#pragma mark - fromMaster
	
#pragma mark master_line
	ns = setBasicSection("fromMaster-line_ext", true, true);
	setCamActorScene(ns);
    ns->addScene(SCENE_LINE, false, true, false, true);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_HIP);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addTuneF(SCENE_LINE, "Curve0", 000.0);
	ns->addTuneF(SCENE_LINE, "ext_to0", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from0", 800.0);
	ns->addTuneF(SCENE_LINE, "Curve1", 300.0);
	ns->addTuneF(SCENE_LINE, "ext_to1", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from1", 800.0);
	setActorViewing(ns, false, true, false, true);
	
#pragma mark master_metaball
	ns = setBasicSection("fromMaster-metaball", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_METABALL, false, true, false, true);
	string act[] = {ACTOR_ANDO, ACTOR_KOJIRI, ACTOR_SHIMAJI, ACTOR_MIYASHITA};
	for (int i = 0;i < 4;i++)
	{
		ns->addExtractor(SCENE_METABALL, act[i], JOINT_NECK);
		ns->addExtractor(SCENE_METABALL, act[i], JOINT_CHEST);
		ns->addExtractor(SCENE_METABALL, act[i], JOINT_LEFT_WRIST);
	}
	setActorViewing(ns, true, true, true, true);
	
#pragma mark master_upsidedown
	ns = setBasicSection("fromMaster-upsidedown", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_UPSIDE, false, true, false, true);
	ns->addExtractor(SCENE_UPSIDE, ACTOR_KOJIRI		, JOINT_NECK);
	ns->addExtractor(SCENE_UPSIDE, ACTOR_ANDO		, JOINT_NECK);
	ns->addExtractor(SCENE_UPSIDE, ACTOR_SHIMAJI	, JOINT_NECK);
	ns->addExtractor(SCENE_UPSIDE, ACTOR_MIYASHITA	, JOINT_NECK);
	
#pragma mark master_
}



ofPtr<sectionSet> dpConductor::setBasicSection(string sectionName, bool sceneClear, bool extClear)
{
	ofPtr<sectionSet> ns = newSection();
	ns->sectionName = sectionName;
	ns->needSceneClear = sceneClear;
	ns->needExtClear = extClear;
	return ns;
}

void dpConductor::setCamActorScene(ofPtr<sectionSet> s)
{
	s->addScene(SCENE_CAMERA, false, false, false, false);
	s->addScene(SCENE_ACTOR, false, false, false, false);
}

void dpConductor::setActorViewing(ofPtr<sectionSet> s, bool ando, bool kojiri, bool shimaji, bool miyashita)
{
	s->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, ando);
	s->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, kojiri);
	s->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, shimaji);
	s->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, miyashita);
}