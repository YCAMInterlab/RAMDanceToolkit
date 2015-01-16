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

class labelChaser{
public:

	labelChaser(int lb){
		label = lb;
		pos.set(0.0,0.0,0.0);
		pos_smooth.set(0.0,0.0,0.0);
	}
	~labelChaser(){

	}

	void update(){
		pos_smooth += (pos - pos_smooth) / 3.0;
	}

	int label;
	ofVec3f pos;
	ofVec3f pos_smooth;
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
	string getName() const { return "hakoVisPLink_Oil"; }

protected:

	int isExistByLabel(int label);
	bool isExistByChaser(int label);
	vector<labelChaser> chasers;
};

#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Oil__) */
