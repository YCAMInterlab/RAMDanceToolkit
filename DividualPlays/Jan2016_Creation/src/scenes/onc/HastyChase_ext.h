//
//  HastyChase_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#ifndef HastyChase_ext_h
#define HastyChase_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class HastyChase_ext : public ramBaseScene
{
public:
    void setupControlPanel();
    void onPanelChanged(ofxUIEventArgs& e);
    void setup();
    void update();
    void drawActor(const ramActor& actor);
    void loadPreset(size_t preset_id=0);
    string getName() const { return "HastyChase_ext"; }
    
private:
    ramMotionExtractor mex;
    map<string, ramTimeShifter> time_shifters;
    float buffer_time;
    float rate;
    bool draw_line;
    bool show_box;
    bool fill_chaser;
    ofFloatColor joint_color;
    
    ofxUIToggle
    *drawLineToggle,
    *showBoxToggle,
    *fillChaserToggle;
};


#endif /* HastyChase_ext_h */
