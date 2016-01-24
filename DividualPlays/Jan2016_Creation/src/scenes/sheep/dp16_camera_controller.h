//
//  dp16_camera_controller.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/24.
//
//

#ifndef dp16_camera_controller_h
#define dp16_camera_controller_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class dp16_camera_controller : public ramBaseScene{
public:
	
	inline string getName() const {return "dp16_camera_controller";}
	dp16_camera_controller(){};
	~dp16_camera_controller(){};
	
	void setupControlPanel();
	
	void update();
	void draw();
	void onPanelChanged(ofxUIEventArgs &e);
	
	void drawActor(const ramActor &actor);
	virtual void onEnabled();
	virtual void onDisabled();
	
private:
	ramOscReceiveTag receiver;
	ramMotionExtractor mex;
	
	ofVec3f camera_pos;
	ofVec3f camera_target;
	ofVec3f camera_vector;
	ofVec3f camera_smoothed;
	
	ofVec3f camera_lookAt;
	ofVec3f camera_lTarget;
	ofVec3f camera_lVector;
	ofVec3f camera_lSmoothed;

	float speed;
	float smoothing;
	
	bool	bindCamera, targeting;
	bool	nodeCam;
};

#endif /* dp16_camera_controller_h */
