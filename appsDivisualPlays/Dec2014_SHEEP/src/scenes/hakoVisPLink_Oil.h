//
//  hakoVisPLink_Oil.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#ifndef __RAMDanceToolkit__hakoVisPLink_Oil__
#define __RAMDanceToolkit__hakoVisPLink_Oil__

#include "ramMain.h"
#include "dpConstants.h"

class labelChaser{
public:

	labelChaser(int lb){
		dead = false;
		label = lb;
		vec.set(0,0,0);
		pos.set(0.0,0.0,0.0);
		pos_smooth.set(0.0,0.0,0.0);
		col.set(ofRandom(255));

		palette[0].set(255,50,150);
		palette[1].set(255,220,235);
		palette[2].set(200,50,120);
		palette[3].set(130,30,80);
		palette[4].set(120,120,120);

		col = palette[int(ofRandom(5))];
		seed = ofRandomuf();
	}
	~labelChaser(){

	}

	void update(){
		pos_smooth += (pos - pos_smooth) / 2.0;
	}

	bool dead;
	int label;
	ofVec3f vec;
	ofVec3f pos;
	ofVec3f pos_smooth;
	ofColor col;
	ofColor palette[5];
	float seed;
};

class hakoVisPLink_Oil : public ramBaseScene{
public:

	hakoVisPLink_Oil();
	~hakoVisPLink_Oil(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void draw_PatternA();
	void draw_PatternB();

	ramOscReceiveTag receiver;

	ofxUICanvasPlus* gui;
	vector<int> labels;
	vector<ofVec3f> pts;

	inline
	string getName() const { return "dpVisPLink_Oil"; }

protected:

	int		isExistByLabel(int label);
	bool	isExistByChaser(int label);

	bool mDebugDraw;
	vector<labelChaser> chasers;
};

#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Oil__) */
