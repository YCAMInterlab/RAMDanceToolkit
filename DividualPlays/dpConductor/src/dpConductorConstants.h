//
//  dpConductorConstants.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef dpConductorConstants_h
#define dpConductorConstants_h

#define OSC_SET_SCENE_ADDR string("/ram/set_scene")
#define OSC_CLEAR_SCENE_ADDR string("/ram/clear_scene")
#define OSC_TUNE_ADDR string("/ram/tune/")
#define OSC_EXTRACTOR_ADDR string("/ram/MEX/")

//#define ADDR_RDTK_1 string("192.168.20.2")
//#define ADDR_RDTK_2 string("192.168.20.3")
//#define ADDR_CAMERAUNIT string("192.168.20.5")

#define ADDR_RDTK_1 string("127.0.0.1")
#define ADDR_RDTK_2 string("192.168.20.3")
#define ADDR_CAMERAUNIT string("127.0.0.1")

//#define ACTOR_ANDO string("ando")
//#define ACTOR_KOJIRI string("kojiri")
//#define ACTOR_SHIMAJI string("shimaji")
//#define ACTOR_MIYASHITA string("miyashita")

#define ACTOR_ANDO string("Ando_2012-09-01_18-49-10")
#define ACTOR_KOJIRI string("Richi_2012-09-01_16-55-24")
#define ACTOR_SHIMAJI string("Richi_2012-09-01_16-52-45")
#define ACTOR_MIYASHITA string("Ando_2012-09-01_19-19-45")


#define SCENE_LINE		string("Line_ext")
#define SCENE_BIGBOX	string("Scene_BigBox")
#define SCENE_FOURPT	string("FourPoints_ext")
#define SCENE_THREEPT	string("Three Points")
#define SCENE_BROKEN	string("BrokenBody_ext")
#define SCENE_METABALL	string("distanceMetaball")
#define SCENE_DONUTS	string("Donuts")
#define SCENE_MONSTER	string("monster")

enum Joint
{
	JOINT_HIPS              = 0,
	JOINT_ABDOMEN           = 1,
	JOINT_CHEST             = 2,
	JOINT_NECK              = 3,
	JOINT_HEAD              = 4,
	
	JOINT_LEFT_HIP          = 5,
	JOINT_LEFT_KNEE         = 6,
	JOINT_LEFT_ANKLE        = 7,
	JOINT_LEFT_TOE          = 8,
	
	JOINT_RIGHT_HIP         = 9,
	JOINT_RIGHT_KNEE        = 10,
	JOINT_RIGHT_ANKLE       = 11,
	JOINT_RIGHT_TOE         = 12,
	
	JOINT_LEFT_COLLAR       = 13,
	JOINT_LEFT_SHOULDER     = 14,
	JOINT_LEFT_ELBOW        = 15,
	JOINT_LEFT_WRIST        = 16,
	JOINT_LEFT_HAND         = 17,
	
	JOINT_RIGHT_COLLAR      = 18,
	JOINT_RIGHT_SHOULDER    = 19,
	JOINT_RIGHT_ELBOW       = 20,
	JOINT_RIGHT_WRIST       = 21,
	JOINT_RIGHT_HAND        = 22,
	
	NUM_JOINTS              = 23,
};

#endif /* dpConductorConstants_h */
