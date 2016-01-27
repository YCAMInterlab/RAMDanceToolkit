//
//  cameraUnitManager.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#ifndef cameraUnitManager_h
#define cameraUnitManager_h

#include "ofMain.h"
#include "dpConductorConstants.h"
#include "baseOscSender.h"

class cameraUnitManager : public baseOscSender{
public:
	
	void setup(string host);
	void setCameraSlot(string name, bool enable, bool viewA, bool viewB);
	
};

#endif /* cameraUnitManager_h */
