//
//  dpSwitchingManager.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2015/01/09.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpSwitchingManager__
#define __dpHakoniwa_cameraUnit__dpSwitchingManager__

#include "ofMain.h"
#include "ofxOsc.h"

#define DP_DISP_PT_STGLEFT 0
#define DP_DISP_PT_STGRIGHT 1
#define DP_DISP_PT_UPSTAGE 2
#define DP_DISP_PT_DOWNSTAGE 3

#define DP_DISP_TYPE_HAKOVIS 0
#define DP_DISP_TYPE_CAMERATHRU 1
#define DP_DISP_TYPE_AUDIENCE 2
#define DP_DISP_TYPE_FLOOR 3

enum hakoniwaType{
	HAKO_STRUGGLE,
	HAKO_FROZENICE,
	HAKO_PLINK_LASER,
	HAKO_PLINK_MAGNET,
	HAKO_PLINK_PRISM,
	HAKO_COLOROFWATER,
	HAKO_SERVOPENDULUM
};

struct dpDisplaySelector{
	int displayType;
	int displayPoint_LR;
	int displayPoint_UD;
};

class dpSwitchingManager{
public:

	void setup();
	void update();
	void draw();

	void enableHakoniwa(hakoniwaType type, dpDisplaySelector disp);
	void disableHakoniwa(hakoniwaType type);
	void disableAllHakoniwa();

	void receiveOscMessage(ofxOscMessage &m);

	//to RDTK
	void setScene(string sceneName, bool enable);
	void setAllScene(bool enable);

	//to Switcher
	void setCamera(int src,int dst);
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpSwitchingManager__) */
