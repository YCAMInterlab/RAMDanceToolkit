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
		ofxAutoControlPanel::setup();
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
};