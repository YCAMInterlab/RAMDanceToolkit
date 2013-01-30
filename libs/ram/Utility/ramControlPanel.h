#pragma once
#include "ofxUITabbedCanvas.h"


class ramControlPanel
{
	
private:
	
	float mR, mG, mB;
	bool mUseBgSlider;

	/// first panel
    ofxUITabbedCanvas mTabbedCanvas;
	ofxUICanvas *mPanelGeneral;
	
	static ramControlPanel *_instance;
	
	ramControlPanel()
	{
		mR = 50;
		mG = 50;
		mB = 50;
		mUseBgSlider = true;
	}

public:
	
	static ramControlPanel& instance();
	
	virtual ~ramControlPanel() {}
	
	void setup()
	{
		
		/// Event hook
		// -------------------------------------
		ofAddListener(ofEvents().update, this, &ramControlPanel::update);
		
		
		
		/// First panel
		// -------------------------------------
		float dim = 16;
		float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
		float length = 320-xInit;
		
		
		/// background
		mPanelGeneral = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());
		mPanelGeneral->addWidgetDown(new ofxUILabel("RamDanceToolkit", OFX_UI_FONT_LARGE));
		mPanelGeneral->addSpacer(length, 2);
		
		
		/// background color
		mPanelGeneral->addWidgetDown(new ofxUIToggle(32, 32, true, "Use Background Slider"));
		mPanelGeneral->addSlider("BG:R", 0, 255, &mR, 95, dim);
		mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		mPanelGeneral->addSlider("BG:G", 0, 255, &mG, 95, dim);
		mPanelGeneral->addSlider("BG:B", 0, 255, &mB, 95, dim);
		mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		
		/// floor pattern
		vector<string> floors;
		floors.push_back("None");
		floors.push_back("Grid");
		floors.push_back("Plane");
		floors.push_back("Checker");
		mPanelGeneral->addSpacer(length, 2);
		mPanelGeneral->addRadio("Floor Patterns", floors, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
		
		
		/// camera positions
		vector<string> names;
		names.push_back("Front");
		names.push_back("Right");
		names.push_back("Back");
		names.push_back("Left");
		mPanelGeneral->addSpacer(length, 2);
		mPanelGeneral->addRadio("Camera Preset", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
		
		
		/// Camera Position
		mPanelGeneral->addSpacer(length, 2);
		mPanelGeneral->addWidgetDown(new ofxUILabel("Camera Position", OFX_UI_FONT_MEDIUM));
		mPanelGeneral->addWidgetDown(new ofxUILabel("x:0.00 y:0.00 z:0.00", OFX_UI_FONT_MEDIUM));
		
		
		/// full screan
		mPanelGeneral->addSpacer(length, 2);
		mPanelGeneral->addWidgetDown(new ofxUIToggle(32, 32, false, "FullScrean"));
		
		
		/// Scenes
		mPanelGeneral->addSpacer(length, 2);
		mPanelGeneral->addWidgetDown(new ofxUIToggleMatrix(dim*3, dim*2, 5, 5, "Scenes"));
		
		
		/// add panel to canvas
		ofAddListener(mPanelGeneral->newGUIEvent, this, &ramControlPanel::guiEvent);
		mTabbedCanvas.add(mPanelGeneral);
		mTabbedCanvas.loadSettings("GUI/guiSettings.xml");
	}
	
	void update(ofEventArgs &e)
	{
		if (mUseBgSlider) ofBackground( ofColor(mR, mG, mB) );
	}
	
	void guiEvent(ofxUIEventArgs &e)
	{
		string name = e.widget->getName();
		int kind = e.widget->getKind();
		
		if (name == "Use Background Slider")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			mUseBgSlider = toggle->getValue();
		}
		
		if (name == "FullScrean")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			ofSetFullscreen(toggle->getValue());
		}
	}

};

inline ramControlPanel& ramGetGUI() { return ramControlPanel::instance(); }
