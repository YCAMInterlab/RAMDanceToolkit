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
#include "ramMotionExtractor.h"
#include "ParallelLinkManager.h"

class HakoniwaParallelLink_Base : public ramBaseScene {
public:

	HakoniwaParallelLink_Base();
	virtual ~HakoniwaParallelLink_Base(){
		if (mFlagNeedDestract){
			mLinkManager.setPlot_inClamp(ofVec3f(0.0,196.0,0.0));
			mLinkManager.sendSignal();
		}
	};

	virtual void initialize(){};
	virtual void update_over(){};
	virtual void draw_over(){};

	void setupControlPanel();
	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	inline
	string getName() const { return "HakoniwaParallelLink_Base"; }
	virtual string getAddress() {return "192.168.20.57";};

	virtual void onEnabled();
	virtual void onDisabled();

	void savePreset();
	void loadPreset();

protected:
	ofxOscSender* mOscSender;
	ramActor mActor;
	static const int kNumValves = 3;


	//============================================
	ParallelLinkManager mLinkManager;

	bool CalibratePose;
	bool ManualPose;
	bool mDigitalOut;
	bool mTrackMachine;
	bool mDrawExtractor;
	bool mDrawMachine;
	
	float mPwm_Param;

	bool mDigitalIO[6];
	bool mDigitalIO_Prev[6];
	bool mFlagNeedDestract;

	string mSendOSCAddress;
	
	ofVec3f mManualPosition;
	ofVec3f machinePosition;

	ofxUICanvas* xyzGui;
	ofxUICanvas* systemGui;
	ofxUICanvas* settingGui;
	ofxUICanvas* utilityGui;

	ramMotionExtractor motionEx;

	float mSetting_Accel;
	float mSetting_Deccel;
	float mSetting_MaxSpeed;
};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
