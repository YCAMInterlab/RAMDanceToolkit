//
//  HakoniwaPLink_Laser.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/10.
//
//

#ifndef __RAMDanceToolkit__HakoniwaPLink_Laser__
#define __RAMDanceToolkit__HakoniwaPLink_Laser__

#include "HakoniwaParallelLink_Base.h"

class HakoniwaPLink_Laser : public HakoniwaParallelLink_Base{
public:
	inline string getName() const {return "dpPLink_Laser";}
	virtual string getAddress() {return "192.168.20.68";}

	virtual void initialize();
	virtual void update_over();
	virtual void draw_over();
};

#endif /* defined(__RAMDanceToolkit__HakoniwaPLink_Laser__) */
