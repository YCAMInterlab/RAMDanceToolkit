//
//  environmentManager.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#ifndef environmentManager_h
#define environmentManager_h

#include "ofMain.h"
#include "sceneController.h"

class environmentManager : public sceneController{
public:
	
	void setCameraPreset(ofVec3f position, ofVec3f target, float speed, float smooth);
	void setNodeCamera(string actor, Joint from, Joint look);
	
	void setViewFloor(bool b);
	void setActorDraw(string name, bool view);
	void setActorMargine(float value);
	
	const string camScn = "dp16_camera_controller";
};

#endif /* environmentManager_h */
