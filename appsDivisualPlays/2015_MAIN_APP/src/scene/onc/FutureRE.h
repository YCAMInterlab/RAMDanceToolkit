//
//  FutureRE.h
//  BasicTraining_Aug2014
//
//  Created by Onishi Yoshito on 8/11/14.
//
//

#ifndef __BasicTraining_Aug2014__FutureRE__
#define __BasicTraining_Aug2014__FutureRE__

#include "ramMain.h"

class FutureRE : public ramBaseScene {
public:
	FutureRE();
    virtual ~FutureRE();
    
	void setupControlPanel();
	
    void update();
	void draw();
	
	void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
	inline
    string getName() const { return "FutureRE"; }
    
    struct Diff {
        ramActor previousActor;
        ramActor previousResult;
    };
    
    map<string, Diff> diffs;
    
    float lowpass;
};

#endif /* defined(__BasicTraining_Aug2014__FutureRE__) */
