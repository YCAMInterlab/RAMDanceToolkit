//
//  sectionSet.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#ifndef sectionSet_h
#define sectionSet_h

#include "ofMain.h"
#include "sceneController.h"
#include "cameraUnitManager.h"
#include "environmentManager.h"
#include "ofxUI.h"

struct sceneSet
{
	string	name;
	bool	enable;
	bool	displayX;
	bool	displayY;
};

struct extSet
{
	string scene;
	string actor;
	Joint jt;
};

struct tuneSet
{
	string scene;
	string typeName;
	ofxUIWidgetType type;
	float valF;
	bool valT;
};

enum{
	ENV_CMD_CAMERA,
	ENV_CMD_NODECAM,
	ENV_CMD_FLOOR,
	ENV_CMD_MARGINE,
	ENV_CMD_DRAWACT
};

struct envSet
{
	int envCmd;
	float actorMargine = 150;
	bool viewFloor;
	ofVec3f camera_pos;
	ofVec3f camera_look;
	float camera_speed;
	float camera_smooth;
	
	string actorName;
	bool drawAct;
	Joint nCam_pos;
	Joint nCam_look;
};

class sectionSet{
public:
	
	void init(ofPtr<sceneController> scn,
			  ofPtr<cameraUnitManager> cam,
			  ofPtr<environmentManager> env);
	
	void addScene		(string scene, bool displayX, bool displayY);
	void addExtractor	(string scene, string actor, Joint node);
	void addTuneF		(string scene, string type, float value);
	void addTuneT		(string scene, string type, bool value);
	void addTuneB		(string scene, string type);
	void addEnv_camera	(ofVec3f pos, ofVec3f look, float speed, float smooth);
	void addEnv_nodeCam	(string name, Joint nodeFrom, Joint nodeLook, float speed, float smooth);
	void addEnv_drawAct (string name, bool view);
	void addEnv_floor	(bool view);
	void addEnv_margine	(float margine);
	
	void doSection();
	
	string				sectionName;
	
	vector<sceneSet>	sceneList;
	vector<extSet>		extList;
	vector<tuneSet>		tuneList;
	vector<envSet>		envList;

	bool				needSceneClear = false;
	bool				needExtClear = false;
	
	ofPtr<sceneController> _scene;
	ofPtr<cameraUnitManager> _camera;
	ofPtr<environmentManager> _environment;
	
};

#endif /* sectionSet_h */
