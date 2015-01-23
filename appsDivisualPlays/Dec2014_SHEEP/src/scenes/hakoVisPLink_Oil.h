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

	inline
	string getName() const { return "dpVisPLink_Oil"; }


	//Pixelate;
	vector<bool>	pixels;
	vector<ofVec2f> offsets;
	vector<float>	scales;
	vector<float>	scaleTarg;
	int pix_w;
	int pix_h;

protected:

	bool mDebugDraw;
};

#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Oil__) */
