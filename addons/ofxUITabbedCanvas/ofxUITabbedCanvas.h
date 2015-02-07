#pragma once

#include "ofxUI.h"
#include "ofxUIToggleMini.h"
#include "ramGlobal.h"

class ofxUICanvasPlus : public ofxUICanvas {
public:
	ofxUICanvasPlus();
	void saveSettingsToXml(ofxXmlSettings& xml);
	void loadSettingsFromXml(ofxXmlSettings& xml);
protected:
	static int getChoice(ofxUIRadio* radio);
	static int getChoice(ofxUIEventArgs& e, ofxUIRadio* radio);
};

class ofxUITab : public ofxUICanvasPlus {
protected:
	string tabName;
	bool visible;
	bool enabled, enableable;
public:
	ofxUITab(const string& tabName = "", bool enableable = true)
	:visible(false)
	,enabled(false)
	,enableable(enableable)
	,tabName(tabName) {
        setFont(ramToResourcePath(RAM_FONT_FILE));
        
		addLabel(tabName, OFX_UI_FONT_LARGE);
		addSpacer();
	}
	const string& getTabName() const {return tabName;}
	bool& getVisible() {return visible;}
	bool& getEnabled() {return enabled;}
	bool getEnableable() {return enableable;}
};

class ofxUITabbedCanvas : public ofxUICanvasPlus {
protected:
	int currentTab;
	float tabWidth;
	bool visible;
	bool saveStatus, loadStatus;
	vector<ofxUITab*> tabs;
	vector<ofxUILabelToggle*> tabToggles;
	vector<ofxUIToggle*> enableToggles;
	ofxUIImageButton *saveButton, *loadButton;
public:
	ofxUITabbedCanvas(float tabWidth = 200)
	:currentTab(0)
	,saveStatus(false)
	,loadStatus(false)
	,tabWidth(tabWidth)
	,visible(true) {
        setFont(ramToResourcePath(RAM_FONT_FILE));
        
        loadButton = new ofxUIImageButton(0, 0, 32, 32, &loadStatus, ramToResourcePath("Images/open.png"), "Load");
        saveButton = new ofxUIImageButton(0, 0, 32, 32, &saveStatus, ramToResourcePath("Images/save.png"), "Save");
        addWidgetRight(loadButton);
        addWidgetRight(saveButton);
		ofAddListener(newGUIEvent, this, &ofxUITabbedCanvas::guiEvent);
	}
    
	void add(ofxUITab* tab) {
		tab->disableAppDrawCallback();
		tab->disableMouseEventCallbacks();
		tab->disableKeyEventCallbacks();
		tab->disableWindowEventCallbacks();
		if(tabs.empty()) {
			tab->getVisible() = true;
		}
		tabs.push_back(tab);
		ofxUILabelToggle* tabToggle = new ofxUILabelToggle(tab->getTabName(), &tab->getVisible(), tabWidth, 0, 0, 0, OFX_UI_FONT_SMALL, true);
        addWidgetDown(tabToggle);
		tabToggles.push_back(tabToggle);
		if(tab->getEnableable()) {
			ofxUIToggle* enableToggle = new ofxUIToggleMini("Enable " + tab->getTabName(), &tab->getEnabled(), tabToggle->getRect()->height, tabToggle->getRect()->height);
			
			addWidgetRight(enableToggle);
			enableToggles.push_back(enableToggle);
		} else {
			enableToggles.push_back(NULL);
		}
		autoSizeToFitWidgets();
	}
    
	bool isHit(float x, float y) {
		return ofxUICanvas::isHit(x, y) || getCurrent()->isHit(x - getRect()->width, y);
	}
    
	ofxUITab* at(int i) {
		return tabs[i];
	}
    
	int getTabIndex(const string& name) const {
		for(int i = 0; i < tabToggles.size(); i++) {
			ofxUILabelToggle *tabToggle = tabToggles[i];		
			if(tabToggle->getName() == name) {
				return i;
			}
		}
		return -1;
	}
    
	void select(string name,bool force = false) {
		int tabIndex = getTabIndex(name);
		if(tabIndex != -1 && (tabToggles[tabIndex]->getValue() || force)) {
			currentTab = tabIndex;
			if(enableToggles[tabIndex] != NULL) {
				enableToggles[tabIndex]->setValue(true);
			}
			for(int i = 0; i < tabToggles.size(); i++) {
				tabToggles[i]->setValue(i == tabIndex);
			}
		}
	}
	void offSelect(string name,bool force = false) {
		int tabIndex = getTabIndex(name);
		if(tabIndex != -1 && (tabToggles[tabIndex]->getValue() || force)) {
			currentTab = tabIndex;
			if(enableToggles[tabIndex] != NULL) {
				enableToggles[tabIndex]->setValue(false);
			}
			for(int i = 0; i < tabToggles.size(); i++) {
				tabToggles[i]->setValue(i == tabIndex);
			}
		}
	}

	void triggerEvent(ofxUIWidget *child) {
		select(child->getName());
		ofxUICanvas::triggerEvent(child);
	}
	ofxUITab* getCurrent() {
		return tabs[currentTab];
	}
    const ofxUITab* const getCurrent() const {
        return tabs[currentTab];
    }
	ofxUIToggle* getEnableToggle(size_t idx)
	{
		return enableToggles.at(idx);
	}
    
    const ofxUIToggle* const getEnableToggle(size_t idx) const
    {
        return enableToggles.at(idx);
    }
    
	void guiEvent(ofxUIEventArgs &e) {
		if (e.widget == saveButton && saveStatus) {
			ofFileDialogResult result = ofSystemSaveDialog("settings.xml", "Save settings.");
			if(result.bSuccess) {
				saveSettings(result.getPath());
			}
		}
		if (e.widget == loadButton && loadStatus) {
			ofFileDialogResult result = ofSystemLoadDialog("Load settings.", false);
			if(result.bSuccess) {
				loadSettings(result.getPath());
			}
		}
	}
	void draw() {
		if(visible) {
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			
			ofPushStyle();
			ofPushMatrix();
			ofNoFill();
			ofTranslate(getRect()->width, 0);
			getCurrent()->draw();
			ofPopMatrix();
			ofPopStyle();
			
			ofxUICanvas::draw();
		}
	}
	void keyPressed(int key) {
		if(key == '\t') visible = !visible;
		if (!visible) return;
		ofxUICanvas::keyPressed(key);
		if (tabs.empty()) return;
		getCurrent()->keyPressed(key);
	}
	void keyReleased(int key) {
		if (!visible) return;
		ofxUICanvas::keyReleased(key);
		if (tabs.empty()) return;
		getCurrent()->keyReleased(key);
	}
	void mouseMoved(int x, int y) {
		if (!visible) return;
		ofxUICanvas::mouseMoved(x, y);
		if (tabs.empty()) return;
		getCurrent()->mouseMoved(x - getRect()->width, y);
	}
	void mouseDragged(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mouseDragged(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mouseDragged(x - getRect()->width, y, tabToggle);
	}
	void mousePressed(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mousePressed(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mousePressed(x - getRect()->width, y, tabToggle);
	}	
	void mouseReleased(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mouseReleased(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mouseReleased(x - getRect()->width, y, tabToggle);
	}	
	void loadSettings(const string &filename);
	void saveSettings(const string &filename);
};