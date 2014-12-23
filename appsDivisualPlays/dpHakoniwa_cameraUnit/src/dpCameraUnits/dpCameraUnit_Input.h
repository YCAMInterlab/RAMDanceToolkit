//
//  dpCameraUnit_Input.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/18.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpCameraUnit_Input__
#define __dpHakoniwa_cameraUnit__dpCameraUnit_Input__

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"


class dpCameraUnit_input{
public:

	dpCameraUnit_input();
	~dpCameraUnit_input();

	void update();
	void draw(int x,int y);

	void mouseMoved(ofMouseEventArgs &arg);
	void mousePressed(ofMouseEventArgs &arg);
	void mouseDragged(ofMouseEventArgs &arg);
	void mouseReleased(ofMouseEventArgs &arg);
	void guiEvent(ofxUIEventArgs &e);

	ofxUICanvas			mGui;

	
	const float			input_width		= 640.0;
	const float			input_height	= 480.0;
	const float			thumb_ratio		= 0.5;
	const float			cvSrc_ratio		= 0.5;

	int					mSourceType;
	float				mGain;
	ofVec2f				mWarpPoint[4];
	ofVec2f				mDrawnPoint;
	vector<string>		mCameraList;
	bool				mEnablePerspective;
	bool				mIsFrameNew;
	bool				mFourSplit;

	int					mGuiParam_catchWarpPoint;

	ofVideoGrabber		mVideoGrabber;
	ofVideoPlayer		mVideoPlayer;

	ofImage				mFinalSource_Large;
	ofImage				mFinalSource_Small;
	
	ofImage				mFinalSource_FourSplit[4];
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_Input__) */
