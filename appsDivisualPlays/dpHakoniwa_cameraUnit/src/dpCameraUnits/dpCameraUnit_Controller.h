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

class dpCameraUnit_Controller{
public:
	dpCameraUnit_Controller();
	~dpCameraUnit_Controller();

	void update();
	void draw();
	void simulator_initialize();
	void refleshAddressList();

	void guiEvent(ofxUIEventArgs& e);
	
	ofxUICanvas gui;
	vector<string> oscSendList;

	dpCameraUnit_input		inputUnit;
	dpCameraUnit_cvFX		cvFXUnit[4];
	dpCameraUnit_cvAnalysis cvAnalysis[4];
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_Controller__) */
