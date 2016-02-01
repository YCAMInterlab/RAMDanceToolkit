//
//  floorline_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/02/01.
//
//

#ifndef floorline_ext_h
#define floorline_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "FloorLine.h"

class floorline_ext : public ramBaseScene{
public:
	
	inline string getName() const {return "floorline_ext";}
	
	floorline_ext(){};
	~floorline_ext(){};
	
	void setupControlPanel();
	void setup();
	void update();
	void draw();
	void drawActorSelected(const ramActor& actor);
	void drawSpheres();
	void drawExtendActor(ramActor &centered);
	void onDisabled();
	void onPanelChanged(ofxUIEventArgs& e);
	
private:
	float mLShoulderScale = 6.0;
	float mRShoulderScale = 6.0;
	
	float mLHipScale = 6.0;
	float mRHipScale = 6.0;
	
	ramCenteredActor mCentered;
	
	static const int NUM_SPHERE = 6;
	
	vector<SoundSphere>mSpheres;
	bool mIsSphere = true;
	
	ofxOscSender mSenderToSound[2];
	
	void setupOscSenders();
	ramMotionExtractor mex;
};

#endif /* floorline_ext_h */
