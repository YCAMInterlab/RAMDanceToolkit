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
    
    int video_no,port_no;

    inline string getName() const {
        return "HakoniwaTheta";
    }

private:
    
    ramMotionExtractor	motionExtractor;

    void radiusChanged(int radius);
    void VideoChanged(int no);
    bool mDrawPreview;
    bool mDrawDump;
    bool mDrawhidden;
    bool m1, m2, m3, m4, m5, m6, m7, m8, m9;

    ofVideoPlayer vidPlay;
    ofSpherePrimitive sphere;

};

#endif /* defined(__RAMDanceToolkit__HakoniwaThetaScene__) */
