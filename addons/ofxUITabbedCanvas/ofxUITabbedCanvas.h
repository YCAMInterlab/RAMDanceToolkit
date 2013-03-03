#pragma once

#include "ofxUI.h"

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
	ofxUILabel* title;
	bool visible;
	bool enabled, enableable;
public:
	ofxUITab(string tabName = "", bool enableable = true)
	:visible(false)
	,enabled(false)
	,enableable(enableable)
	{
		title = addLabel(tabName, OFX_UI_FONT_LARGE);
		addSpacer();
	}
	
	void setTabName(const string& tabName) {title->setLabel(tabName);}
	string getTabName() const {return title->getLabel();}
	bool& getVisible() {return visible;}
	bool& getEnabled() {return enabled;}
	bool getEnableable() {return enableable;}
};

class ofxUITabbedCanvas : public ofxUICanvasPlus {
protected:
	int currentTab;
	float tabWidth, enableWidth;
	bool visible;
	bool saveStatus, loadStatus;
	vector<ofxUITab*> tabs;
	vector<ofxUILabelToggle*> tabToggles;
	vector<ofxUIToggle*> enableToggles;
	ofxUIImageButton *saveButton, *loadButton;
public:
	ofxUITabbedCanvas(float tabWidth = 100, float enableWidth = 10)
	:currentTab(0)
	,saveStatus(false)
	,loadStatus(false)
	,tabWidth(tabWidth)
	,enableWidth(enableWidth)
	,visible(true) {
        loadButton = new ofxUIImageButton(0, 0, 32, 32, &loadStatus, "../../../../resources/Images/open.png", "Load");
        saveButton = new ofxUIImageButton(0, 0, 32, 32, &saveStatus, "../../../../resources/Images/save.png", "Save");
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
			//ofxUIImageToggle* enableToggle = new ofxUIImageToggle(0, 0, tabToggle->getRect()->height, tabToggle->getRect()->height, &tab->getEnabled(), "../../../../resources/Images/show.png", "Enable " + tab->getTabName());
			ofxUIToggle* enableToggle = new ofxUIToggle("Enable " + tab->getTabName(), &tab->getEnabled(), enableWidth, tabToggle->getRect()->height);
			
			ofxUIRectangle* rect = enableToggle->getRect();
			rect->setWidth(enableWidth);
			rect->setHeight(enableWidth);
			ofxUIRectangle* paddedRect = enableToggle->getPaddingRect();
			paddedRect->setWidth(enableWidth);
			paddedRect->setHeight(enableWidth);
			paddedRect->setParent(rect);
			ofxUILabel* label = enableToggle->getLabelWidget();
			label->setParent(label);
			label->setRectParent(rect);
			label->setEmbedded(true);
			label->setVisible(false);
			
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
	int getTabIndex(string name) {
		for(int i = 0; i < tabToggles.size(); i++) {
			ofxUILabelToggle *tabToggle = tabToggles[i];		
			if(tabToggle->getName() == name) {
				return i;
			}
		}
		return -1;
	}
	void select(string name) {
		int tabIndex = getTabIndex(name);
		if(tabIndex != -1 && tabToggles[tabIndex]->getValue()) {
			currentTab = tabIndex;
			if(enableToggles[tabIndex] != NULL) {
				enableToggles[tabIndex]->setValue(true);
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