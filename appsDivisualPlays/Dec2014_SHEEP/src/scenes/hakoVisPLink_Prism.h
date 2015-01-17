//
//  hakoVisPLink_Prism.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#ifndef __RAMDanceToolkit__hakoVisPLink_Prism__
#define __RAMDanceToolkit__hakoVisPLink_Prism__

#include "ramMain.h"
#include "ofxHexGrid.h"

#include "dpConstants.h"

class hexChaser{
public:
	void setGrid(ofxHexGrid* grd,int lb){
		mGrid = grd;
		dead = false;
		label = lb;
	}

	void setDefault(int tx,int ty){
		targX = tx;
		targY = ty;
		x = tx;
		y = ty;
		pos = mGrid->getPosition(x, y);
		pos_smooth = pos;
	}

	void setNextPosition(int tx, int ty){
		targX = tx;
		targY = ty;
	}

	void update(){
		pos_smooth += (pos - pos_smooth) / 1.0;
		if (pos_smooth.distanceSquared(pos) < 2){
			pos_smooth = pos;
			Refleshchase();
		}
		pos_log.push_back(pos_smooth);
		while (pos_log.size() > 90){
			pos_log.pop_front();
		}

	}

	void Refleshchase(){
		dirIsX = (ofRandomuf() < 0.5);
		if (dirIsX){
			if (x < targX) x++;
			if (x > targX) x--;
		}else{
			if (y < targY) y++;
			if (y > targY) y--;
		}

		pos = mGrid->getPosition(x, y);
	}

	bool dead;
	int label;

	bool dirIsX;
	int x,y;
	int targX,targY;
	deque<ofVec2f> pos_log;
	ofVec2f pos;
	ofVec2f pos_smooth;
	ofxHexGrid* mGrid;
};

class hakoVisPLink_Prism : public ramBaseScene{
public:
	hakoVisPLink_Prism();
	~hakoVisPLink_Prism(){};

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
	string getName() const { return "dpVisPLink_Prism"; }

protected:


	int		isExistByLabel(int label);
	bool	isExistByChaser(int label);

	bool mDebugDraw;

	vector<hexChaser> chasers;

	ofxHexGrid grid;
};





#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Prism__) */
