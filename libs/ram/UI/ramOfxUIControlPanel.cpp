#include "ramControlPanel.h"

#include "ramBaseScene.h"
#include "ramControllable.h"

#include "ramActorsScene.h"

#ifdef RAM_GUI_SYSTEM_OFXUI

ramOfxUIControlPanel *ramOfxUIControlPanel::_instance = NULL;

ramOfxUIControlPanel& ramOfxUIControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramOfxUIControlPanel();
	}
	return *_instance;
}

ramOfxUIControlPanel::ramOfxUIControlPanel()
:kDim(16)
,kXInit(OFX_UI_GLOBAL_WIDGET_SPACING)
,kLength(320 - kXInit)
,camera_preset_t(0)
,camera_preset(0)
,backgroundColor(ofColor(0))
{
}

ramOfxUIControlPanel::~ramOfxUIControlPanel()
{
	delete (ramActorsScene*) actorsScene;
}

void ramOfxUIControlPanel::setup()
{
	/// Event hooks
	// -------------------------------------
	ofAddListener(ofEvents().update, this, &ramOfxUIControlPanel::update);
	
	addPanel(presetTab);
	addPanel(preferencesTab);
	addPanel(playbackTab);
	
	actorsScene = new ramActorsScene();
	addPanel(actorsScene, false);
	actorsScene->setEnabled(true);
	
	ofAddListener(mSceneTabs.newGUIEvent, this, &ramOfxUIControlPanel::guiEvent);
	
	mSceneTabs.addSpacer();
}

void ramOfxUIControlPanel::update(ofEventArgs &e)
{
	actorsScene->update();
	if(!ofGetMousePressed())
	{
		bool hover = mSceneTabs.isHit(ofGetMouseX(), ofGetMouseY());
		ramCameraManager::instance().setEnableInteractiveCamera(!hover);
	}
}

//

void ramOfxUIControlPanel::addPanel(ramBaseScene* control, bool enableable)
{
	ofxUITab *panel = new ofxUITab(control->getName(), enableable);
	current_panel = panel;	
	control->setupControlPanel();
	getSceneTabs().add(panel);
	scenes.push_back(enableable ? control : NULL);
	panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::addPanel(ofxUITab& tab)
{
	scenes.push_back(NULL);
	getSceneTabs().add(&tab);
}

void ramOfxUIControlPanel::addSection(const string& name)
{
	current_panel->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	current_panel->addSpacer(kLength, 2);
	current_panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::addSeparator()
{
	current_panel->addSpacer(kLength, 2);
	current_panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::addLabel(const string& content)
{
	current_panel->addWidgetDown(new ofxUILabel(content, OFX_UI_FONT_MEDIUM));
	current_panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::addToggle(const string& name, bool *value)
{
	current_panel->addToggle(name, value, 30, 30);
	current_panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::addMultiToggle(const string& name, const vector<string>& content, int *value)
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

ofxUIRadio* ramOfxUIControlPanel::addRadioGroup(const string& name, const vector<string>& content, int *value)
{
	ofxUIRadio *o = current_panel->addRadio(name, content, OFX_UI_ORIENTATION_VERTICAL, kDim, kDim);
	
	// FIXME: memory leak
	RadioGroupListener *e = new RadioGroupListener(o, value);
	ofAddListener(current_panel->newGUIEvent, e, &RadioGroupListener::handle);
	current_panel->autoSizeToFitWidgets();
	return o;
}

void ramOfxUIControlPanel::addDropdown(const string& name, const vector<string>& content, int *value)
{
	assert(false);
}

void ramOfxUIControlPanel::addSlider(const string& name, float min_value, float max_value, float *value)
{
	current_panel->addSlider(name, min_value, max_value, value, kLength, kDim);
	current_panel->autoSizeToFitWidgets();
}

struct ColorSelectorListener
{
	ofxUIToggle* toggle;
	ofFloatColor *value;
	ofFloatColor color;
	
	ColorSelectorListener(ofxUIToggle* toggle, ofFloatColor *value) : toggle(toggle), value(value), color(*value) {}
	
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

void ramOfxUIControlPanel::addColorSelector(const string& name, ofFloatColor *value)
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
	current_panel->autoSizeToFitWidgets();
}

void ramOfxUIControlPanel::remove(const string& name)
{
	assert(false);
}

void ramOfxUIControlPanel::guiEvent(ofxUIEventArgs &e)
{
	for(int i = 0; i < scenes.size(); i++) 
	{
		if(scenes[i] != NULL)
		{
			// bomisutaro: this is a weak connection, it would be better for ramScene to extend ofxUITab
			scenes[i]->setEnabled(mSceneTabs.at(i)->getEnabled());
		}
	}
}

#endif