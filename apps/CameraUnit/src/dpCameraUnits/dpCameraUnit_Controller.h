//
//  dpCameraUnit_Controller.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpCameraUnit_Controller__
#define __dpHakoniwa_cameraUnit__dpCameraUnit_Controller__

#include "ofMain.h"
#include "dpCameraUnit_Input.h"
#include "dpCameraUnit_ShaderFX.h"
#include "dpCameraUnit_cvFX.h"
#include "dpCameraUnit_cvAnalysis.h"

#define NUM_CV_UNIT 1

class dpCameraUnit_Controller{
public:
	dpCameraUnit_Controller();
	~dpCameraUnit_Controller();

	void update();
	void draw();
	void simulator_initialize();
	void refleshAddressList();

	void guiEvent(ofxUIEventArgs& e);

	bool mbMakeSettings;
	int makeSettings_targetInput;

	vector<string> oscSendList;

	dpCameraUnit_input		inputUnit;
	dpCameraUnit_cvFX		cvFXUnit[NUM_CV_UNIT];
	dpCameraUnit_cvAnalysis cvAnalysis[NUM_CV_UNIT];
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_Controller__) */
