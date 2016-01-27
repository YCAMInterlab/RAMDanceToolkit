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
