//
//  sw-1010F_SerialController.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/21.
//
//

#ifndef __dpHakoniwa_cameraUnit__sw_1010F_SerialController__
#define __dpHakoniwa_cameraUnit__sw_1010F_SerialController__

#include "ofMain.h"

class sw_1010F_SerialController{
public:

	sw_1010F_SerialController();
	~sw_1010F_SerialController();

	const int DP_SW1010_GEAR_1		= 0;
	const int DP_SW1010_GEAR_2		= 1;
	const int DP_SW1010_STRUGGLE	= 2;
	const int DP_SW1010_PLINK_LASER	= 3;
	const int DP_SW1010_PLINK_MAGNE	= 4;
	const int DP_SW1010_PLINK_PRISM	= 5;
	const int DP_SW1010_PLINK_OIL	= 6;
	const int DP_SW1010_SAND		= 7;
	const int DP_SW1010_PENDULUM	= 8;
	const int DP_SW1010_SERVO_PENDULUM = 9;
	

	void setSW(int cameraChannel, int displayChannel);
	ofSerial serial_;

	
};

#endif /* defined(__dpHakoniwa_cameraUnit__sw_1010F_SerialController__) */
