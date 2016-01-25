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

private:
	int fixer;
	ofVec3f offset;
};

#endif /* dp16_ramActorTranslator_h */
