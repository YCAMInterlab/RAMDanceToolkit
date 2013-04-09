#pragma once

#include "ofxUITabbedCanvas.h"
#include "ramNodeFinder.h"

class ramPresetTab : public ofxUITab
{
    
protected:
    
    // preset scenes
	ofxUIToggleMatrix* matrix;
    
    
    // preset camera positions
    ofxUIRadio* preset_cam_radio;
	int preset_cam_index;
    
    
    // nodecam
    bool use_node_cam;
    bool cam_position;
    bool cam_look_at;
    
    ramNodeFinder nf_pos;
    ramNodeFinder nf_look_at;
    
    ramNode node_pos;
    ramNode node_look_at;
    
public:
    
	ramPresetTab();
	void update(ofEventArgs& e);
	void draw(ofEventArgs& e);
	void guiEvent(ofxUIEventArgs &e);
    
};
