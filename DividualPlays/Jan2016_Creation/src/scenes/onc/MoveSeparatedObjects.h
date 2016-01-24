//
//  MoveSeparatedObjects.h
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#ifndef __RAMDanceToolkit__MoveSeparatedObjects__
#define __RAMDanceToolkit__MoveSeparatedObjects__

#include "ramMain.h"

class MoveSeparatedObjects : public ramBaseScene {
public:    
	MoveSeparatedObjects();
    virtual ~MoveSeparatedObjects();
    
	void setupControlPanel();
	
    void update();
	void draw();
	
	void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
	inline
    string getName() const { return "MoveSeparatedObjects"; }
    
};

#endif /* defined(__RAMDanceToolkit__MoveSeparatedObjects__) */
