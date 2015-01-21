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
        return "dpVisTheta";
    }

private:
    
    ramMotionExtractor	motionExtractor;

    virtual void onEnabled();
    virtual void onDisabled();
    void radiusChanged(int radius);
    void VideoChanged(int no);
    void drawDump();
    bool mDrawDump;
    bool mDrawhidden;
    float mDegreeOffset;
    
    int mVideoNo,portNo;
    bool vidsetup[5];
    
    ofVideoPlayer	vidPlay[5];
    ofSpherePrimitive sphere;

};

#endif /* defined(__RAMDanceToolkit__HakoniwaThetaScene__) */
