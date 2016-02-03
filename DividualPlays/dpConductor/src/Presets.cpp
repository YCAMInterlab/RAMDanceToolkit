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

#pragma mark 0020
	ns = setBasicSection("0020=BigBoxKojiri", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0030
	ns = setBasicSection("0030=yasuIn-off", true, true);
	setCamActorScene(ns);
	
#pragma mark 0040
	ns = setBasicSection("0040=andoIn-2Box", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, true, false);

#pragma mark 0050
	ns = setBasicSection("0050=miyashita_in_5box", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	MIYASHITA_BIGBOX
	setActorViewing(ns, false, true, true, true);
	
#pragma mark 0060
	ns = setBasicSection("0060=kenYasuOut_AB-Off", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	MIYASHITA_BIGBOX
	setActorViewing(ns, false, true, true, true);

#pragma mark 0070
	ns = setBasicSection("0070=AndoCue=Kojiri_line", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, true, true, true, true);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addTuneF(SCENE_LINE, "Curve0", 0.0);
	ns->addTuneF(SCENE_LINE, "ext_to0", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from0", 800.0);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0090
	ns = setBasicSection("0090=AndoCue=2Line", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, true, false, true, false);
	ns->addScene(SCENE_FOURPT, false, true, false, true);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	
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
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0100
	ns = setBasicSection("0100=A=off", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, false, false, true, false);
	ns->addScene(SCENE_FOURPT, false, true, false, true);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	
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
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0120
	ns = setBasicSection("0120=threepoint", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_THREEPT, false, true, true, false);
	ns->addScene(SCENE_METABALL, false, false, false, true);
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_METABALL, ACTOR_ANDO, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_METABALL, ACTOR_ANDO, JOINT_RIGHT_KNEE);

	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	
	ns->addTuneT(SCENE_THREEPT, "Show rects", true);
	ns->addTuneF(SCENE_THREEPT, "Rect radius", 42.1);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0130
	ns = setBasicSection("0130=monster", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER, false, false, true, true);
	ns->addScene(SCENE_MONSTER_2, true, true, false, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_MIYASHITA, JOINT_NECK);
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0131
	ns = setBasicSection("0131=monster-geom", false, false);
	ns->addTuneT(SCENE_MONSTER, "Randomize Geometry", true);
	ns->addTuneT(SCENE_MONSTER_2, "Randomize Geometry", true);

#pragma mark 0132
	ns = setBasicSection("0132=monster-topo", false, false);
	ns->addTuneT(SCENE_MONSTER, "Randomize Topology", true);
	ns->addTuneT(SCENE_MONSTER_2, "Randomize Topology", true);

#pragma mark 0133
	ns = setBasicSection("0133=monster-line", false, false);
	ns->addTuneT(SCENE_MONSTER, "Random Line", true);
	ns->addTuneT(SCENE_MONSTER_2, "Random Line", true);

#pragma mark 0140
	ns = setBasicSection("0140=burst", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BURST, true, true, true, true);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
	setActorViewing(ns, false, false, true, false);
	
#pragma mark 0145
	ns = setBasicSection("0145=burst_AOff", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BURST, false, true, true, true);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
	setActorViewing(ns, false, false, true, false);
	
#pragma mark 0150
	ns = setBasicSection("0150=chaser", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_CHASER, true, true, true, true);
	ns->addExtractor(SCENE_CHASER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
	setActorViewing(ns, false, false, true, true);
	
#pragma mark 0160
	ns = setBasicSection("0160=Donuts", true, true);
	ns->addScene(SCENE_DONUTS, true, true, true, true);
	
	ns->addExtractor(SCENE_DONUTS, ACTOR_ANDO, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_LEFT_ANKLE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_RIGHT_ANKLE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_CHEST);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_LEFT_HAND);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_RIGHT_HAND);
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0170
	ns = newSection();
	ns->sectionName = "0170-all-off";
	ns->needExtClear = true;
	ns->needSceneClear = true;
	
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
	
#pragma mark master_threept
	ns = setBasicSection("fromMaster-threept", true, true);
	ns->addScene(SCENE_THREEPT, false, true, false, true);
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	
	ns->addTuneT(SCENE_THREEPT, "Show rects", true);
	ns->addTuneF(SCENE_THREEPT, "Rect radius", 42.1);
	setActorViewing(ns, false, true, true, false);
	
#pragma mark master_burst
	ns = setBasicSection("fromMaster-burst", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BURST, false, true, false, true);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
	setActorViewing(ns, false, false, true, false);

#pragma mark master_bigbox
	ns = setBasicSection("fromMaster-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	MIYASHITA_BIGBOX
	setActorViewing(ns, false, true, true, true);
	
#pragma mark master_monster
	ns = setBasicSection("0130=monster", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER, false, false, true, true);
	ns->addScene(SCENE_MONSTER_2, true, true, false, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneT(SCENE_MONSTER, "Randomize Topology", true);
	ns->addTuneT(SCENE_MONSTER_2, "Randomize Topology", true);
	setActorViewing(ns, true, true, true, true);
	
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