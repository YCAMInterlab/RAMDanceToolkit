//
//  Presets.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/28.
//
//

#include "dpConductor.h"

void dpConductor::setSections()
{
	//AddScene preset (Disp 1, Disp6, Disp2-3, Disp 4-5)

	ofPtr<sectionSet> ns = newSection();//===================================================
	ns->sectionName = "Blank";
	ns->needExtClear = true;
	ns->needSceneClear = true;
	
#pragma mark - BigBoxセクション
	ns = newSection();//===================================================
	ns->sectionName = "DefaultBone";
	ns->addScene("dp16_camera_controller", true, false, true, false);
	ns->addScene("dp16_ramActorTranslator", true, false, true, false);
	ns->needExtClear = true;
	ns->needSceneClear = true;
	
	//安藤さんビッグボックス右肘:7
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0010-Ando_BigBox_1";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, false);
	
	//安藤さんビッグボックス左膝＋小尻さん右膝:8
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0020-ando_BigBox_2";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, false);
	
	//安藤さんビッグボックスキープ＋小尻さん右膝:9
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0030-kojiri_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, false);
	
	//ヤスさんin:10
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0040-yasu_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, false);

	//みやしたさんin:11
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0050-miyashita_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	//みやしたさんBigBox5個
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0060-miyashita_5Box";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	//小尻さん全Box
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BIGBOX, true, false, true, false);
	ns->addScene(SCENE_LINE, false, true, false, true);
	ns->sectionName = "0070-kojiri_allBox";
	
	for (int i = 0;i < 23;i++)
		ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, (Joint)i);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_TOE);
	for (int i = 0;i < 3;i++)
	{
		ns->addTuneF(SCENE_LINE, "Curve"+ofToString(i)		, i == 1 ? 0.0 : 100.0);
		ns->addTuneF(SCENE_LINE, "ext_to"+ofToString(i)		, 800.0);
		ns->addTuneF(SCENE_LINE, "ext_from"+ofToString(i)	, 800.0);
		ofFloatColor c;
		c.setHsb(i / 6.0, 1.0, 1.0);
		ns->addTuneF(SCENE_LINE, "Red_"+ofToString(i)		, c.r);
		ns->addTuneF(SCENE_LINE, "Green_"+ofToString(i)		, c.g);
		ns->addTuneF(SCENE_LINE, "Blue_"+ofToString(i)		, c.b);
	}
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

#pragma mark - Lineセクション
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0080-ando_line";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_LINE, true, true, true, true);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_ANDO, JOINT_LEFT_KNEE);

	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_TOE);
	for (int i = 0;i < 3;i++)
	{
		ns->addTuneF(SCENE_LINE, "Curve"+ofToString(i)		, i == 1 ? 0.0 : 100.0);
		ns->addTuneF(SCENE_LINE, "ext_to"+ofToString(i)		, 800.0);
		ns->addTuneF(SCENE_LINE, "ext_from"+ofToString(i)	, 800.0);
		ofFloatColor c;
		c.setHsb(i / 6.0, 1.0, 1.0);
		ns->addTuneF(SCENE_LINE, "Red_"+ofToString(i)		, c.r);
		ns->addTuneF(SCENE_LINE, "Green_"+ofToString(i)		, c.g);
		ns->addTuneF(SCENE_LINE, "Blue_"+ofToString(i)		, c.b);
	}
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0090-kojiri_line";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_LINE		, true, true, true, true);
	
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_TOE);
	
	for (int i = 0;i < 3;i++)
	{
		ns->addTuneF(SCENE_LINE, "Curve"+ofToString(i)		, i == 1 ? 0.0 : 100.0);
		ns->addTuneF(SCENE_LINE, "ext_to"+ofToString(i)		, 800.0);
		ns->addTuneF(SCENE_LINE, "ext_from"+ofToString(i)	, 800.0);
		ofFloatColor c;
		c.setHsb(i / 6.0, 1.0, 1.0);
		ns->addTuneF(SCENE_LINE, "Red_"+ofToString(i)		, c.r);
		ns->addTuneF(SCENE_LINE, "Green_"+ofToString(i)		, c.g);
		ns->addTuneF(SCENE_LINE, "Blue_"+ofToString(i)		, c.b);
	}
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0100-three-and-line";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_LINE, true, true, true, false);
	ns->addScene(SCENE_THREEPT, false, false, false, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_LINE, ACTOR_MIYASHITA, JOINT_RIGHT_TOE);
	
	for (int i = 0;i < 3;i++)
	{
		ns->addTuneF(SCENE_LINE, "Curve"+ofToString(i)		, i == 1 ? 0.0 : 100.0);
		ns->addTuneF(SCENE_LINE, "ext_to"+ofToString(i)		, 800.0);
		ns->addTuneF(SCENE_LINE, "ext_from"+ofToString(i)	, 800.0);
		ofFloatColor c;
		c.setHsb(i / 6.0, 1.0, 1.0);
		ns->addTuneF(SCENE_LINE, "Red_"+ofToString(i)		, c.r);
		ns->addTuneF(SCENE_LINE, "Green_"+ofToString(i)		, c.g);
		ns->addTuneF(SCENE_LINE, "Blue_"+ofToString(i)		, c.b);
	}
	
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	ns->addTuneT(SCENE_THREEPT, "Show spheres", true);
	
#pragma mark - 三者ミックス
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0110-3-4-metaMix";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_THREEPT, false, true, false, true);
	ns->addScene(SCENE_FOURPT, false, false, true, false);
	ns->addScene(SCENE_METABALL, false, true, false, true);
	
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_FOURPT, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addTuneF(SCENE_FOURPT, "resolution", 50.0);
	ns->addTuneF(SCENE_FOURPT, "width", 3.0);
	ns->addTuneF(SCENE_FOURPT, "height", 1.3);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_THREEPT, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	ns->addTuneT(SCENE_THREEPT, "Show spheres", true);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

#pragma mark - Broken
	ns = newSection();//===================================================
	ns->sectionName = "0120-Broken";
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BROKEN, true, true, true, true);
	ns->addExtractor(SCENE_BROKEN, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_BROKEN, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_BROKEN, ACTOR_MIYASHITA, JOINT_NECK);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
#pragma mark TODO: FloorLine
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0130-Broken_burst";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BURST, true, false, true, false);
	ns->addScene(SCENE_BROKEN,false, false, true, false);

	ns->addExtractor(SCENE_BROKEN, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
//	ns->addExtractor(SCENE_FLOOR, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0140-Broken_donuts";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_BROKEN, false, false, false, true);
	ns->addScene(SCENE_BURST, false, false, false, true);
	ns->addScene(SCENE_DONUTS, true, false, true, false);
	ns->addExtractor(SCENE_BROKEN, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_BURST, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	ns->addTuneT(SCENE_DONUTS, "Show Actor", false);

	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
#pragma mark - ここから先は順不同
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0160-All-Metaball";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_METABALL, false, true, true, true);
	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_RIGHT_SHOULDER);
	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_HEAD);
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
	
#pragma mark TODO: ドーナツ
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0170-All-Donuts";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_DONUTS, true, true, true, true);

	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_DONUTS, ACTOR_KOJIRI, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_DONUTS, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_LEFT_TOE);
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_HEAD);
	
	ns->addTuneF(SCENE_DONUTS, "Radius", 5.0);
	ns->addTuneT(SCENE_DONUTS, "Show Actor", false);

	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0180-All-Monster";
	ns->addScene(SCENE_MONSTER, true, true, true, true);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addExtractor(SCENE_MONSTER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_MONSTER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneF(SCENE_MONSTER, "Randomization amount", 0.15);
	ns->addTuneB(SCENE_MONSTER, "Randomize Topology");
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0190-All-FourPoints";
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_FOURPT, true, true, true, true);
	
	string actor_fp[] = {ACTOR_KOJIRI, ACTOR_SHIMAJI, ACTOR_MIYASHITA};
	for (int i = 0;i < 3;i++)
	{
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_KNEE);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_KNEE);
	}
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

#pragma mark TODO: Chaser
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0200-All-Chaser";

	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, true, true, true, true);
	ns->addScene(SCENE_CHASER, true, true, true, true);
	ns->addExtractor(SCENE_CHASER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneF(SCENE_CHASER, "buffer_time", 3500);
	ns->addTuneF(SCENE_CHASER, "rate", 1.04);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
#pragma mark 以下箱庭
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0220-SandStorm_fourpoint";
	ns->addHakoniwa(HAKO_SANDSTORM, true, true, true);

	ns->addScene("dpVisSandStorm", false, true, false, false);
	ns->addScene("dpHSandStorm", false, false, false, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_FOURPT, true, false, true, true);
	
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_CHEST);
	for (int i = 0;i < 3;i++)
	{
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_KNEE);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_KNEE);
	}
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0230-Chaser-Tornado";
	
	ns->addHakoniwa(HAKO_TORNADO, true, true, true);
	ns->addScene("dpHTornado", false, false, false, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_CHASER, true, true, true, true);
	
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_CHEST);
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0240-Chaser-Tornado-SandStorm-Fp";
	
	ns->addHakoniwa(HAKO_TORNADO, true, true, false);
	ns->addHakoniwa(HAKO_SANDSTORM, true, false, true);
	ns->addScene("dpHSandStorm", false, false, false, false);
	ns->addScene("dpVisSandStorm", false, true, false, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_CHASER, true, false, true, true);
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_SANDSTORM, ACTOR_KOJIRI, JOINT_CHEST);

	ns->addExtractor(SCENE_CHASER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_CHEST);
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_RIGHT_ELBOW);
	ns->addExtractor(HAKO_TORNADO, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0250-Stage-Chaser";
	
	ns->addHakoniwa(HAKO_STAGE, true, true, true);
	ns->addScene("dpVisStage", true, false, true, false);
	ns->addScene(SCENE_CHASER, false, true, false, true);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, true, false, true);
	
	ns->addExtractor(SCENE_CHASER, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_CHASER, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);
	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0260-Servo";
	
	ns->addHakoniwa(HAKO_SERVOPENDULUM, true, true, true);
	ns->addScene("dpHServoPendulum", false, false, false, false);
	ns->addScene("dpVisServoPendulum", false, false, false, true);
	ns->addScene(SCENE_LINE, false, false, true, false);
	ns->addScene(SCENE_DONUTS, false, true, false, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	
	ns->addExtractor(HAKO_SERVOPENDULUM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_SERVOPENDULUM, ACTOR_KOJIRI, JOINT_CHEST);
	ns->addExtractor(HAKO_SERVOPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_WRIST);

	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_LEFT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_SHOULDER);
	ns->addExtractor(SCENE_LINE, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	
	ns->addExtractor(SCENE_DONUTS, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addTuneF(SCENE_DONUTS, "Radius", 5.0);
	ns->addTuneT(SCENE_DONUTS, "Show Actor", true);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0270-magPendulum";
	ns->addHakoniwa(HAKO_MAGPENDULUM, true, true, true);
	ns->addScene("dpHMagPendulum", false, false, false, false);
	ns->addScene("dpVisMagPendulum"	, false, true, true, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	ns->addScene(SCENE_METABALL		, false, false, false, true);
	
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);

	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_RIGHT_SHOULDER);
	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_METABALL, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_HEAD);
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_METABALL, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_ABDOMEN);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_METABALL, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_ANDO, false);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_KOJIRI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_SHIMAJI, true);
	ns->addTuneT(SCENE_ACTOR, "V_"+ACTOR_MIYASHITA, true);

	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0280-magPendulum";
	ns->addHakoniwa(HAKO_MAGPENDULUM, true, true, true);
	ns->addScene("dpHMagPendulum", false, false, false, false);
	ns->addScene("dpVisMagPendulum"	, false, true, false, true);
	ns->addScene(SCENE_FOURPT		, false, true, true, false);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);

	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_KOJIRI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_LEFT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_RIGHT_WRIST);
	ns->addExtractor(HAKO_MAGPENDULUM, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);
	for (int i = 0;i < 3;i++)
	{
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_WRIST);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_RIGHT_KNEE);
		ns->addExtractor(SCENE_FOURPT, actor_fp[i], JOINT_LEFT_KNEE);
	}
	
	ns = newSection();//===================================================
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0290-worm";
	ns->addHakoniwa(HAKO_WORM, true, true, true);
	ns->addScene("dpHWorm", false, false, false, false);
	ns->addScene(SCENE_BROKEN, true, true, true, true);
	ns->addScene(SCENE_CAMERA, false, false, false, false);
	ns->addScene(SCENE_ACTOR, false, false, false, false);
	
	ns->addExtractor(HAKO_WORM, ACTOR_ANDO, JOINT_CHEST);
	ns->addExtractor(HAKO_WORM, ACTOR_ANDO, JOINT_RIGHT_WRIST);
	ns->addExtractor(SCENE_BROKEN, ACTOR_KOJIRI, JOINT_NECK);
	ns->addExtractor(SCENE_BROKEN, ACTOR_SHIMAJI, JOINT_NECK);
	ns->addExtractor(SCENE_BROKEN, ACTOR_MIYASHITA, JOINT_NECK);
}
