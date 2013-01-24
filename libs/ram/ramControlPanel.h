#pragma once
#include "ofxAutoControlPanel.h"
#include "ofxControlPanel.h"


class ramControlPanel : public ofxAutoControlPanel
{
public:
	
	void setup()
	{
		ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 100));
		ofxControlPanel::setBackgroundColor(simpleColor(0, 0, 0, 90));
		ofxAutoControlPanel::setup(400, ofGetHeight());
		
		/*!
			Panel: Config
		 */
		addPanel("Config");
		
		
		/* Background Color */
		addSlider("Background", 0, 0, 255);
		
		
		/* Default Floor */
		addMultiToggle("Floor pattern", 0, ramFloor::getFloorNames());
		addSlider("Floor size", 600.0, 100.0, 1000.0);
		addSlider("Grid size", 50.0, 10.0, 100.0);
	}
	
	/*!
		ofxControlPanel
	 */
	bool hasValueChanged(string xmlName)
	{
		bool bChanged = ofxControlPanel::hasValueChanged(xmlName);
		if (bChanged) clearAllChanged();
		return bChanged;
	}
	
	bool hasValueChangedInPanel(string whichPanel)
	{
		bool bChanged = ofxControlPanel::hasValueChangedInPanel(whichPanel);
		if (bChanged) clearAllChanged();
		return bChanged;
	}
	
	/*!
		ofxAutoControlPanel
	 */
	// usage: panel.hasValueChanged(variadic(1)(2)(3)(4)(5));
	bool hasValueChanged(const vector<string>& values)
	{
		bool bChanged = ofxAutoControlPanel::hasValueChanged(values);
		if (bChanged) clearAllChanged();
		return bChanged;
	}
	
	
	// --
	// usage: panel.addTextDropDown("DropDownName", 0, variadic(1)(2)(3)(4)(5));
	guiTypeTextDropDown * addTextDropDown(string name, int defaultBox, const vector<string>& names)
	{
		return ofxControlPanel::addTextDropDown(name, name, defaultBox, names);
	}
};