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
	/*=============================================================*/
	ns = setBasicSection("fp-0020-kenta-box-relbow", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, true, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0030
	/*=============================================================*/
	ns = setBasicSection("fp-0030-yasu-in-disp-off", true, true);
	setCamActorScene(ns);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0040
	/*=============================================================*/
	ns = setBasicSection("fp-0040-yoko-out", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, true, false);
	
#pragma mark 0050 TODO: ボックスサイズプリセット
	/*=============================================================*/
	ns = setBasicSection("fp-0050-miyashita-in", false, false);
	MIYASHITA_BIGBOX
	
	setActorViewing(ns, false, true, true, true);
	
#pragma mark 0070
	/*=============================================================*/
	ns = setBasicSection("fp-0070-dispClear", true, true);
	setCamActorScene(ns);
	setActorViewing(ns, false, false, false, false);
	
#pragma mark 0080
	/*=============================================================*/
	ns = setBasicSection("fp-0080-line", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, true, true, true, true);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addTuneF(SCENE_LINE, "Curve0", 0.0);
	ns->addTuneF(SCENE_LINE, "ext_to0", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from0", 800.0);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0090 TODO: 4P 長さ
	ns = setBasicSection("fp-0090-line-4p", true, true);
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
	
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_HIP);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addTuneF(SCENE_LINE, "Curve1", 300.0);
	ns->addTuneF(SCENE_LINE, "ext_to1", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from1", 800.0);
	ns->addTuneF(SCENE_FOURPT, "resolution", 52.0);
	ns->addTuneF(SCENE_FOURPT, "width", 3.47);
	ns->addTuneF(SCENE_FOURPT, "height", 1.40);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0100
	ns = setBasicSection("fp-0010-off_proj", true, false);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, false, false, true, false);
	ns->addScene(SCENE_FOURPT, false, true, false, true);
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0110
	ns = setBasicSection("fp-0110-4pt-upside-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_FOURPT, false, false, true, false);
	ns->addScene(SCENE_UPSIDE, false, true, false, true);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	
	MIYASHITA_BIGBOX
	ns->addExtractor(SCENE_UPSIDE, ACTOR_KOJIRI, JOINT_NECK);
	
	ns->addTuneF(SCENE_FOURPT, "Twist resolution", 52.0);
	ns->addTuneF(SCENE_FOURPT, "Twist extension width", 3.47);
	ns->addTuneF(SCENE_FOURPT, "Twist extension height", 1.40);
	setActorViewing(ns, true, true, true, true);
	
#pragma mark 0120
	ns = setBasicSection("fp-0120-3pt-bigbox", true, true);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_THREEPT, false, true, true, true);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_KNEE);

	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	
	MIYASHITA_BIGBOX
    ns->addTuneT(SCENE_THREEPT, "Show rects", true);
    ns->addTuneF(SCENE_THREEPT, "Rect radius", 42.1);
	setActorViewing(ns, false, true, true, true);

#pragma mark - 0130 TODO:[DONE]ここ宮下さんだけモンスターインスタンス2つ作る
	ns = setBasicSection("fp-0130-all-monster", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER_2, false, false, true, true);
	ns->addScene(SCENE_MONSTER, true, true, false, false);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
    ns->addTuneT(SCENE_MONSTER_2, "Randomize Topology", true);
    ns->addTuneT(SCENE_MONSTER, "Randomize Topology", true);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0135
    ns = setBasicSection("fp-0135-monster-randtopo", false, false);
    ns->addTuneT(SCENE_MONSTER, "Randomize Topology", true);
    ns->addTuneT(SCENE_MONSTER_2, "Randomize Topology", true);
    
#pragma mark 0140
	ns = setBasicSection("fp-0140-monster-burst", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER, false, false, false, true);
	ns->addScene(SCENE_BURST, true, true, true, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
    ns->addTuneT(SCENE_MONSTER, "Randomize Topology", true);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0150 TODO: ここ追跡スピード変わる？
	ns = setBasicSection("fp-0150-chaser", true, true);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, true, true, true, true);
	ns->addScene(SCENE_CHASER, true, true, true, true);
	ns->addExtractor(SCENE_CHASER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_ANDO, JOINT_NECK);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0160 TODO: 個別設定できない？ & TOE付いてない
	ns = setBasicSection("fp-0160-donuts", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_DONUTS, true, true, true, true);
	ns->addExtractor(SCENE_DONUTS, ACTOR_ANDO, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_LEFT_TOE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_RIGHT_TOE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_CHEST);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_LEFT_HAND);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_RIGHT_HAND);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0180
	ns = setBasicSection("fp-0180-donuts_all_off", true, true);
	setCamActorScene(ns);
	
#pragma mark 0190
	ns = setBasicSection("fp-0190-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, false, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	MIYASHITA_BIGBOX
	setActorViewing(ns, false, true, true, true);

#pragma mark 0200
	ns = setBasicSection("fp-0200-kojiri-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0210
	ns = setBasicSection("fp-0210-yasu-in", true, true);
	setCamActorScene(ns);
	
#pragma mark 0220
	ns = setBasicSection("fp-0220-yoko-out-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	setActorViewing(ns, false, true, true, false);
	
#pragma mark 0230
	ns = setBasicSection("fp-0230-miyashita-in-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	MIYASHITA_BIGBOX
	setActorViewing(ns, false, true, true, true);

#pragma mark 0240
	ns = setBasicSection("fp-0240-disp-alloff", true, true);
	setCamActorScene(ns);
	setActorViewing(ns, false, true, true, true);

#pragma mark 0250
	ns = setBasicSection("fp-0250-kojiri-line", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_LINE, false, true, false, true);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addTuneF(SCENE_LINE, "Curve0", 0.0);
	ns->addTuneF(SCENE_LINE, "ext_to0", 800.0);
	ns->addTuneF(SCENE_LINE, "ext_from0", 800.0);
	setActorViewing(ns, false, true, false, false);
	
#pragma mark 0260 TODO:[DONE] 宮下さんLineパラメ(LINE出ないので不要)
	ns = setBasicSection("fp-0260-4pt", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_FOURPT, false, true, false, true);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_ANDO, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	
	ns->addTuneF(SCENE_FOURPT, "Twist resolution", 52.0);
	ns->addTuneF(SCENE_FOURPT, "Twist extension width", 3.47);
	ns->addTuneF(SCENE_FOURPT, "Twist extension height", 1.40);
	setActorViewing(ns, true, true, true, true);

	
#pragma mark 0280
	ns = setBasicSection("fp-0280-upside-bigbox", true, true);
	ns->addScene(SCENE_UPSIDE, false, true, false, true);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addExtractor(SCENE_UPSIDE, ACTOR_KOJIRI, JOINT_NECK);
	MIYASHITA_BIGBOX
	setActorViewing(ns, true, true, true, true);

#pragma mark 0290
	ns = setBasicSection("fp-0290-3pt-bigbox", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_BIGBOX, false, true, false, true);
	ns->addScene(SCENE_THREEPT, false, true, false, true);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	MIYASHITA_BIGBOX
	setActorViewing(ns, true, true, true, true);

#pragma mark 0300 TODO: [DONE]モンスター分配
	ns = setBasicSection("fp-0300-monster", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER, false, false, false, true);
	ns->addScene(SCENE_MONSTER_2, false, true, false, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER_2, ACTOR_MIYASHITA, JOINT_NECK);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0310
	ns = setBasicSection("fp-0310-monster-burst", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_MONSTER, false, false, false, true);
	ns->addScene(SCENE_BURST, false, true, false, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_ANDO, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
	setActorViewing(ns, true, true, true, true);

#pragma mark 0320
	ns = setBasicSection("fp-0320-chaser", true, true);
	setCamActorScene(ns);
	ns->addScene(SCENE_CHASER, false, true, false, true);
	ns->addExtractor(SCENE_CHASER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
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