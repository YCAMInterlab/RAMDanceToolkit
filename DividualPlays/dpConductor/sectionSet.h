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
	
	void doSection();
	
	string				sectionName;
	
	vector<sceneSet>	sceneList;
	vector<extSet>		extList;
	vector<tuneSet>		tuneList;

	bool				needSceneClear = false;
	bool				needExtClear = false;
	
	ofPtr<sceneController> _scene;
	ofPtr<cameraUnitManager> _camera;
	ofPtr<environmentManager> _environment;
	
};

#endif /* sectionSet_h */
