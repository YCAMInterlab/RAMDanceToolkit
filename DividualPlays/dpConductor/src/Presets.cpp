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
	
#pragma mark - BigBoxセクション
	ofPtr<sectionSet> ns = newSection();
	ns->sectionName = "DefaultBone";
	ns->addScene("dp16_camera_controller", true, false, true, false);
	ns->addScene("dp16_ramActorTranslator", true, false, true, false);
	ns->needExtClear = true;
	ns->needSceneClear = true;
	
	//安藤さんビッグボックス右肘:7
	ns = newSection();
	ns->needSceneClear;
	ns->needExtClear = true;
	ns->sectionName = "0010-Ando_BigBox_1";
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	
	//安藤さんビッグボックス左膝＋小尻さん右膝:8
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0020-ando_BigBox_2";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_LEFT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	
	//安藤さんビッグボックスキープ＋小尻さん右膝:9
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0030-kojiri_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	
	//ヤスさんin:10
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0040-yasu_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_ANDO, JOINT_RIGHT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	
	//みやしたさんin:11
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0050-miyashita_BigBox";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	
	//みやしたさんBigBox5個
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0060-miyashita_5Box";
	ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);
	
	//小尻さん全Box
	ns = newSection();
	ns->needExtClear = true;
	ns->addScene(SCENE_BIGBOX, true, true, true, true);
	ns->sectionName = "0070-kojiri_allBox";
	for (int i = 0;i < 23;i++)
		ns->addExtractor(SCENE_BIGBOX, ACTOR_KOJIRI, (Joint)i);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_SHIMAJI, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_RIGHT_KNEE);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_WRIST);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_NECK);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_ELBOW);
	ns->addExtractor(SCENE_BIGBOX, ACTOR_MIYASHITA, JOINT_LEFT_KNEE);
	
#pragma mark - Lineセクション
	ns = newSection();
	ns->needSceneClear = true;
	ns->needExtClear = true;
	ns->sectionName = "0080-ando_line";
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
	
	
	
}
