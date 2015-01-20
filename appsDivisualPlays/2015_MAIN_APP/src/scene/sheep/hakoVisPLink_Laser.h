//
//  hakoVisPLink_Laser.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/11.
//
//

#ifndef __RAMDanceToolkit__hakoVisPLink_Laser__
#define __RAMDanceToolkit__hakoVisPLink_Laser__

#include "ramMain.h"
#include "ofxDelaunay.h"
#include "sheepCam.h"

class liningUnit{
public:
	liningUnit(){
		pixel	= false;
		transp	= 0;
		transp_smooth  = 0;
		pts.set(0,0,0);
		stand = true;
	}
	~liningUnit(){};
	bool	pixel;
	bool	stand;
	int		transp;
	int		transp_smooth;
	int		flagValue;
	ofVec3f pts;
	ofVec3f pt_smooth;
};

class hakoVisPLink_Laser :public ramBaseScene{
public:

	hakoVisPLink_Laser();
	virtual ~hakoVisPLink_Laser(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	void draw_PatternA();
	void draw_PatternB();

	inline	string getName() const { return "dpVisPLink_Laser"; }
	virtual string getAddress() {return "192.168.20.57";};

//	virtual void onEnabled();
//	virtual void onDisabled();

protected:
	ofxUICanvasPlus* gui;
	ramOscReceiveTag receiver;
	int pix_w,pix_h;

	//===== Pattern A ======
	vector<liningUnit> lines;

	//===== Pattern B ======
	vector<ofMesh> meshes;
	sheepCam ShCam;
	ofxDelaunay del;
};

#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Laser__) */
