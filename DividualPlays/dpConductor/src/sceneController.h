//
//  sceneController.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef sceneController_h
#define sceneController_h

#include "ofMain.h"
#include "baseOscSender.h"
#include "dpConductorConstants.h"

class sceneController : public baseOscSender{
public:
	
	string	getSceneName (int index);
	void	setScene(string name, bool viewA, bool viewB);
	void	clearScene();
	
};

#endif /* sceneController_h */
