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

	dpCameraUnit_input inputUnit;
	dpCameraUnit_ShaderFX shaderUnit;
	dpCameraUnit_cvFX cvFXUnit;
	dpCameraUnit_cvAnalysis cvAnalysis;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_Controller__) */
