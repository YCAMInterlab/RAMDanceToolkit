//
//  dpCameraUnit_ShaderFX.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpCameraUnit_ShaderFX__
#define __dpHakoniwa_cameraUnit__dpCameraUnit_ShaderFX__

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "ofxKsmrFragmentFx.h"

class dpCameraUnit_ShaderFX{
public:
	dpCameraUnit_ShaderFX();
	~dpCameraUnit_ShaderFX();

	void update(ofImage& pix);
	void draw(int x,int y);

	const int buffer_width = 1280.0;
	const int buffer_height = 720.0;

	ofxUICanvas gui;


	ofxKsmrFragmentFx mFx;
	ofFbo mBuffer;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_ShaderFX__) */