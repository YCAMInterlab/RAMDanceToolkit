//
//  HakoniwaParallelLink_Base.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/20.
//
//

#ifndef __RAMDanceToolkit__HakoniwaParallelLink_Base__
#define __RAMDanceToolkit__HakoniwaParallelLink_Base__

#include "ramMain.h"
#include "ParallelLinkManager.h"

class HakoniwaParallelLink_Base : public ramBaseScene {
public:

	HakoniwaParallelLink_Base();
	virtual ~HakoniwaParallelLink_Base(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	inline
	string getName() const { return "HakoniwaParallelLink_Base"; }

private:
	ofxOscSender* mOscSender;
	ramActor mActor;
	static const int kNumValves = 3;


	//============================================
	ParallelLinkManager mLinkManager;

	bool CalibratePose;
	bool ManualPose;
	bool mDigitalOut;
	ofVec3f mManualPosition;
	ofVec3f machinePosition;

};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
