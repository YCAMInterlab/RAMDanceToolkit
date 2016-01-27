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

class sectionSet{
public:
	
	void init(ofPtr<sceneController> scn,
			  ofPtr<cameraUnitManager> cam,
			  ofPtr<environmentManager> env);
	
	void addScene		(string scene, bool displayX, bool displayY);
	void addExtractor	(string scene, string actor, Joint node);
	
	void doSection();
	
	string				sectionName;
	
	vector<sceneSet>	sceneList;
	vector<extSet>		extList;
	
	bool				needSceneClear;
	
	ofPtr<sceneController> _scene;
	ofPtr<cameraUnitManager> _camera;
	ofPtr<environmentManager> _environment;
	
};

#endif /* sectionSet_h */
