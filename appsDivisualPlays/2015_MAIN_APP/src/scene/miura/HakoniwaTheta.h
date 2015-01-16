#ifndef __RAMDanceToolkit__HakoniwaTheta__
#define __RAMDanceToolkit__HakoniwaTheta__

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "dpConstants.h"

class HakoniwaTheta : public ramBaseScene{
    
public:
    void setupControlPanel();
    void setup();
    void update();
    void draw();
    void onPanelChanged(ofxUIEventArgs& e);
    
    inline string getName() const {
        return "HakoVisTheta";
    }

private:
    
    ramMotionExtractor	motionExtractor;

    void radiusChanged(int radius);
    void VideoChanged(int no);
    void drawDump();
    bool mDrawDump;
    bool mDrawhidden;

    int mVideoNo,portNo;
    
    ofVideoPlayer vidPlay;
    ofSpherePrimitive sphere;

};

#endif /* defined(__RAMDanceToolkit__HakoniwaThetaScene__) */
