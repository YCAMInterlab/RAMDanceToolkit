//
//  HakoniwaPLink_Prism.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/12.
//
//

#ifndef __RAMDanceToolkit__HakoniwaPLink_Prism__
#define __RAMDanceToolkit__HakoniwaPLink_Prism__

#include "HakoniwaParallelLink_Base.h"

class HakoniwaPLink_Prism : public HakoniwaParallelLink_Base{
public:
	inline string getName() const {return "dpPLink_Prism";}
	virtual string getAddress() {return "192.168.20.56";}

	virtual void initialize();
	virtual void update_over();
	virtual void draw_over();
};


#endif /* defined(__RAMDanceToolkit__HakoniwaPLink_Prism__) */
