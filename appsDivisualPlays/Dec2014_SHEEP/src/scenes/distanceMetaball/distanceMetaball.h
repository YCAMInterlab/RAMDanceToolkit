//
//  distanceMetaball.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#ifndef __RAMDanceToolkit__distanceMetaball__
#define __RAMDanceToolkit__distanceMetaball__

#include "ramMain.h"

class distanceMetaball : public ramBaseScene{
public:

	distanceMetaball();
	virtual ~distanceMetaball(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	inline
	string getName() const { return "distanceMetaball"; }

	virtual void onEnabled();
	virtual void onDisabled();

private:

	ofxOscSender* mOscSender;
	ramActor mActor;

};

#endif /* defined(__RAMDanceToolkit__distanceMetaball__) */
