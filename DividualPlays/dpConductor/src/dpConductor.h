//
//  dpConductor.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef dpConductor_h
#define dpConductor_h

#include "ofMain.h"
#include "sectionSet.h"

class dpConductor{
public:
	
	void setup();
	void update();
	void draw();
	
	void setSections();
	void callSection(string name);
	void receiveMasterHakoniwa(ofxOscMessage m);
	void listSection(ofxOscMessage m);
	
	void switchHakoniwa(string nameHakoniwa,bool enable, bool A, bool B);
	
	ofPtr<sectionSet> newSection();
	
	vector< ofPtr<sectionSet> > sections;

	ofPtr<sceneController>		sceneCon;
	ofPtr<cameraUnitManager>	cameraCon;
	ofPtr<environmentManager>	envCon;
	
	string display[6];
};

#endif /* dpConductor_h */
