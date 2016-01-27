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

class dp16_ramActorTranslator : public ramBaseScene{
public:
	
	inline string getName() const {return "dp16_ramActorTranslator";}
	
	dp16_ramActorTranslator(){};
	~dp16_ramActorTranslator(){};
	
	void setupControlPanel();
	void update();
	void draw();

	void exit();
	
private:
	
	int fixer;
	ofVec3f offset;
	bool floorStay;
	bool autoLayout;
	bool drawFloor;
	float margine;
};

#endif /* dp16_ramActorTranslator_h */
