//
//  HakoniwaPlink_Oil.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/12.
//
//

#ifndef __RAMDanceToolkit__HakoniwaPlink_Oil__
#define __RAMDanceToolkit__HakoniwaPlink_Oil__

#include "HakoniwaParallelLink_Base.h"

class HakoniwaPLink_Oil : public HakoniwaParallelLink_Base{
public:
	inline string getName() const {return "dpHPLink_Oil";}
	virtual string getAddress() {return "192.168.20.69";}

	virtual void initialize();
	virtual void update_over();
	virtual void draw_over();
};


#endif /* defined(__RAMDanceToolkit__HakoniwaPlink_Oil__) */
