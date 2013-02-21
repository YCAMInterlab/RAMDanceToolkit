#include "ramGwenControlPanel.h"

#ifdef RAM_GUI_SYSTEM_GWEN

ramGwenControlPanel *ramGwenControlPanel::_instance = NULL;

ofColor uiThemecb(64, 192), uiThemeco(192, 192), uiThemecoh(128, 192), uiThemecf(240, 255), uiThemecfh(128, 255), uiThemecp(96, 192), uiThemecpo(255, 192);

ramGwenControlPanel& ramGwenControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramGwenControlPanel();
	}
	return *_instance;
}

ramGwenControlPanel::ramGwenControlPanel() : kDim(16), kXInit(OFX_UI_GLOBAL_WIDGET_SPACING), kLength(320-kXInit)
{
	mFloorPattern = ramFloor::FLOOR_NONE;
	mFloorSize = 600.0;
	mGridSize = 50.0;
	enableShadow = true;
	
	fullScreen = false;
	pause = false;
	
	camera_preset = camera_preset_t = 0;
	
	backgroundColor.set(0, 0, 0, 1);
	
	scenes = NULL;
}

void ramGwenControlPanel::setup()
{
	/// Event hooks
	// -------------------------------------
	ofAddListener(ofEvents().update, this, &ramGwenControlPanel::update);
	
	/// First panel
	// -------------------------------------
	/// panel
	
	
	addPanel("RamDanceToolkit");
	
	addToggle("FullScrean", &fullScreen);
	addToggle("Pause (or press Space Key)", &pause);
	addToggle("Use Shadow", &enableShadow);

	addSeparator();
	
	addColorSelector("Background", &backgroundColor);
	
	addSeparator();
	
	/// floor pattern
	vector<string> floors = ramFloor::getFloorNames();
	addRadioGroup("Floor Patterns", floors, &mFloorPattern);
	
	current_panel->addSlider("Floor Size", 100, 1000, &mFloorSize, kLength/2-kXInit, kDim);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	current_panel->addSlider("Grid Size", 20, 200, &mGridSize, kLength/2-kXInit, kDim);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
	addSeparator();

	/// camera Names
	addRadioGroup("Camera Preset", ramCameraManager::instance().getDefaultCameraNames(), &camera_preset_t);
	
	/// add panel to canvas
	mTabbedCanvas.loadSettings("GUI/guiSettings.xml");
	
	/// Events
	ofAddListener(ofEvents().keyPressed, this, &ramGwenControlPanel::keyPressed);
	ofAddListener(current_panel->newGUIEvent, this, &ramGwenControlPanel::guiEvent);
}

void ramGwenControlPanel::update(ofEventArgs &e)
{
	if (fullScreen != (ofGetWindowMode() == OF_FULLSCREEN))
	{
		ofSetFullscreen(fullScreen);
	}
	
	ramActorManager::instance().setFreezed(pause);
	
	if (camera_preset_t != camera_preset)
	{
		camera_preset = camera_preset_t;
		reloadCameraSetting(camera_preset);
	}
	
	ramEnableShadow(enableShadow);
	
	ofBackground(backgroundColor);
}

//

void ramGwenControlPanel::addPanel(ramControllable* control)
{
	ofxUICanvas *panel = new ofxUICanvas(0, 0, ramGetGUI().kLength+ramGetGUI().kXInit*2.0, ofGetScreenHeight());
	current_panel = panel;
	
	panel->setUIColors(uiThemecb, uiThemeco, uiThemecoh, uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
	
	panel->addWidgetDown(new ofxUILabel(control->getName(), OFX_UI_FONT_LARGE));
	panel->addSpacer(kLength, 2);
	
	control->setupControlPanel(panel);
	getTabbedCanvas().add(panel);
}

void ramGwenControlPanel::addPanel(const string& name)
{
	ofxUICanvas *panel = new ofxUICanvas(0, 0, ramGetGUI().kLength+ramGetGUI().kXInit*2.0, ofGetScreenHeight());
	current_panel = panel;
	
	addSection(name);
	getTabbedCanvas().add(panel);
}

void ramGwenControlPanel::addSection(const string& name)
{
	current_panel->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	current_panel->addSpacer(kLength, 2);
}

void ramGwenControlPanel::addSeparator()
{
	current_panel->addSpacer(kLength, 2);
}

void ramGwenControlPanel::addLabel(const string& content)
{
	current_panel->addWidgetDown(new ofxUILabel(content, OFX_UI_FONT_MEDIUM));
}

void ramGwenControlPanel::addToggle(const string& name, bool *value)
{
	current_panel->addToggle(name, value, 30, 30);
}

void ramGwenControlPanel::addMultiToggle(const string& name, const vector<string>& content, int *value)
{
	assert(false);
}

struct RadioGroupListener
{
	ofxUIRadio *o;
	int *value;
	
	RadioGroupListener(ofxUIRadio *o, int *value) : o(o), value(value)
	{
		o->getToggles().at(*value)->setValue(true);
	}
	
	void handle(ofxUIEventArgs &e)
	{
		if (e.widget->getParent() != o) return;
		vector<ofxUIToggle *> t = o->getToggles();
		for (int i = 0; i < t.size(); i++)
		{
			if (t[i]->getValue())
			{
				*value = i;
				break;
			}
		}
	}
};

void ramGwenControlPanel::addRadioGroup(const string& name, const vector<string>& content, int *value)
{
	ofxUIRadio *o = current_panel->addRadio(name, content, OFX_UI_ORIENTATION_VERTICAL, kDim, kDim);
	
	// FIXME: memory leak
	RadioGroupListener *e = new RadioGroupListener(o, value);
	ofAddListener(current_panel->newGUIEvent, e, &RadioGroupListener::handle);
}

void ramGwenControlPanel::addDropdown(const string& name, const vector<string>& content, int *value)
{
	assert(false);
}

void ramGwenControlPanel::addSlider(const string& name, float min_value, float max_value, float *value)
{
	current_panel->addSlider(name, min_value, max_value, value, kLength, kDim);
}

struct ColorSelectorListener
{
	ofxUIToggle* toggle;
	ofFloatColor *value;
	ofFloatColor color;
	
	ColorSelectorListener(ofxUIToggle* toggle, ofFloatColor *value)
	: toggle(toggle), value(value), color(*value) {}
	
	void handle(ofxUIEventArgs &e)
	{
		if (toggle->getValue())
		{
			color = *value;
			value->a = 1;
		}
		else
		{
			*value = color;
			value->a = 0;
		}
	}
};

void ramGwenControlPanel::addColorSelector(const string& name, ofFloatColor *value)
{
	current_panel->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	
	ofxUIToggle* toggle = current_panel->addToggle("", true, 26, 26);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	
	// FIXME: memory leak
	ColorSelectorListener *e = new ColorSelectorListener(toggle, value);
	ofAddListener(current_panel->newGUIEvent, e, &ColorSelectorListener::handle);
	
	current_panel->addSlider("R", 0, 1, &value->r, 90, kDim);
	current_panel->addSlider("G", 0, 1, &value->g, 90, kDim);
	current_panel->addSlider("B", 0, 1, &value->b, 90, kDim);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}

void ramGwenControlPanel::remove(const string& name)
{
	assert(false);
}

//

void ramGwenControlPanel::reloadCameraSetting(const int index)
{
	ramCameraManager::instance().rollbackDefaultCameraSetting(index);
}

void ramGwenControlPanel::setupSceneToggles(vector<ramBaseScene*>& scenes_)
{
	scenes = &scenes_;
	
	const int size = scenes->size();
	
	if (size <= 0)
	{
		current_panel->addSpacer(kLength, 2);
		current_panel->addLabel("No scenes are assigned.");
		return;
	}
	
	const int numCol = 5;
	const int numRow = ceil((float)size / numCol);
	
	mSceneToggles = new ofxUIToggleMatrix(kDim*3, kDim*2, numRow, numCol, "Scenes");
	current_panel->addSpacer(kLength, 2);
	current_panel->addWidgetDown(mSceneToggles);
}

void ramGwenControlPanel::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	
	/// scene togglematrix
	if (scenes != NULL)
	{
		vector<ofxUIToggle *> toggles = mSceneToggles->getToggles();
		const int numToggles = toggles.size();
		
		for (int i=0; i<numToggles; i++)
		{
			if (i >= scenes->size()) break;
			
			ramBaseScene *scene = scenes->at(i);
			scene->setEnabled( toggles.at(i)->getValue() );
		}
	}
}

void ramGwenControlPanel::keyPressed(ofKeyEventArgs &e)
{
	if (e.key == ' ')
	{
		pause = !pause;
	}
	
	if (e.key == '\t')
	{
		mTabbedCanvas.toggleVisible();
	}
}

#endif