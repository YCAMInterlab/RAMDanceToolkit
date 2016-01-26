//
//  dp16_actorDrawer.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef dp16_actorDrawer_h
#define dp16_actorDrawer_h

#include "ramMain.h"
#include "dpConstants.h"

class dp16_actorDrawer : public ramBaseScene{
public:
	
	dp16_actorDrawer(){};
	~dp16_actorDrawer(){};
	
	inline string getName() const {return "dp16_actorDrawer";}
	
	void setupControlPanel();
	void update();
	void draw();
	
	void drawTargetActor(int id, ramNodeArray & actor);
	
	enum{
		NUM_DRAWACTOR = 4
	};
	
	float base_saturation;
	float base_brightness;
	
	bool	visibles[NUM_DRAWACTOR];
	string	nameList[NUM_DRAWACTOR];
	ofFloatColor palette[NUM_DRAWACTOR];
	
	ofLight light;
	
	bool viewGridFloor;
	float floor_scale;
	float floor_grid_span;
};

#endif /* dp16_actorDrawer_h */
