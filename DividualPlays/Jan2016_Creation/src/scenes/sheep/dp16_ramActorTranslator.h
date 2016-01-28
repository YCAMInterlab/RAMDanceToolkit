//
//  dp16_ramActorTranslator.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#ifndef dp16_ramActorTranslator_h
#define dp16_ramActorTranslator_h

#include "ramMain.h"
#include "dpConstants.h"

class dp16_ramActorTranslator : public ramBaseScene{
public:
	
	inline string getName() const {return "dp16_ramActorTranslator";}
	
	dp16_ramActorTranslator(){};
	~dp16_ramActorTranslator(){};
	
	void setupControlPanel();
	void update();
	void draw();

	void exit();
	void drawTargetActor(int id, ramNodeArray & actor);
	
private:
	
	ofVec3f offset;
	int		fixer;
	bool	floorStay;
	bool	autoLayout;
	bool	drawFloor;
	float	margine;

	//ActorDrawwe=========================
	enum{NUM_DRAWACTOR = 4};
	
	float base_saturation;
	float base_brightness;
	
	bool	visibles[NUM_DRAWACTOR];
	string	nameList[NUM_DRAWACTOR];
	ofFloatColor palette[NUM_DRAWACTOR];
	
	bool viewGridFloor;
	float floor_scale;
	float floor_grid_span;

};

#endif /* dp16_ramActorTranslator_h */
