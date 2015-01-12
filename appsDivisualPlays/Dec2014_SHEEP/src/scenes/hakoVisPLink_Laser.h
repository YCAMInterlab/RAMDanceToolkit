//
//  hakoVisPLink_Laser.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/11.
//
//

#ifndef __RAMDanceToolkit__hakoVisPLink_Laser__
#define __RAMDanceToolkit__hakoVisPLink_Laser__

#include "ramMain.h"

class hakoVisPLink_Laser :public ramBaseScene{
public:

	hakoVisPLink_Laser();
	virtual ~hakoVisPLink_Laser(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	inline
	string getName() const { return "hakoVisPLink_Laser"; }
	virtual string getAddress() {return "192.168.20.57";};

//	virtual void onEnabled();
//	virtual void onDisabled();

protected:

	ramOscReceiveTag receiver;
	int pix_w,pix_h;
	vector<bool> pixels;
};

#endif /* defined(__RAMDanceToolkit__hakoVisPLink_Laser__) */
