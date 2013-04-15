#pragma once

#include "ramBaseScene.h"

class MovingCam : public ramBaseScene
{
    
    ramCameraSettings setting_from, setting_to;
    
public:
	
	void setupControlPanel()
	{
		ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
        
        panel->addToggle("Enable Moving", false, 30, 30);
        panel->addButton("Reload XML", false);
        panel->addButton("Reset", false);
        
        ofAddListener(panel->newGUIEvent, this, &MovingCam::onValuChanged);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
        
	}
	
	void draw()
	{
        
	}
    
    void reloadXML()
    {
        
    }
    
    void reset()
    {
        
    }
    
    void toggleMoving(bool b)
    {
        
    }
    
    void onValuChanged(ofxUIEventArgs &e)
    {
        const string name = e.widget->getName();
        
        if (name == "Reload XML")
            reloadXML();
        
        else if (name == "Reset")
            reset();
        
        else if(name == "Enable Moving")
            toggleMoving(static_cast<ofxUIToggle*>(e.widget)->getValue());
    }
	
	string getName() const { return "MovingCam"; }
};
