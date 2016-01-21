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
#include "ofxOsc.h"

class sw_1010F_SerialController{
public:

	sw_1010F_SerialController();
	~sw_1010F_SerialController();

	void setSW(int cameraChannel, int displayChannel);
	ofSerial serial_;

	
};

#endif /* defined(__dpHakoniwa_cameraUnit__sw_1010F_SerialController__) */
