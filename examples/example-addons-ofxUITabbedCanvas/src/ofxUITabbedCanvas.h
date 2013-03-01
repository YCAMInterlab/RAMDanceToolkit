#pragma once

#include "ofxUI.h"

class ofxUITab : public ofxUICanvas {
protected:
	string tabName;
	bool visible;
	bool enabled, enableable;
public:
	ofxUITab(string tabName, bool enableable = true)
	:tabName(tabName)
	,visible(false)
	,enabled(false)
	,enableable(enableable)
	{
		addLabel(tabName, OFX_UI_FONT_LARGE);
		addSpacer();
	}
	void setTabName(const string& tabName) {this->tabName = tabName;}
	string getTabName() const {return tabName;}
	bool& getVisible() {return visible;}
	bool& getEnabled() {return enabled;}
	bool getEnableable() {return enableable;}
};

class ofxUITabbedCanvas : public ofxUICanvas {
protected:
	int currentTab;
	float tabWidth, enableWidth;
	bool visible;
	vector<ofxUITab*> tabs;
	vector<ofxUILabelToggle*> tabToggles;
	vector<ofxUIToggle*> enableToggles;
public:
	ofxUITabbedCanvas()
	:currentTab(0)
	,tabWidth(100)
	,enableWidth(10)
	,visible(true) {
	}
	void add(ofxUITab* tab) {
		tab->disableAppEventCallbacks();
		tab->disableMouseEventCallbacks();
		tab->disableKeyEventCallbacks();
		tab->disableWindowEventCallbacks();
		if(tabs.empty()) {
			tab->getVisible() = true;
		}
		tabs.push_back(tab);
		ofxUILabelToggle* tabToggle = new ofxUILabelToggle(tab->getTabName(), tab->getVisible(), tabWidth, 0, 0, 0, OFX_UI_FONT_SMALL, true);
        addWidgetDown(tabToggle);
		tabToggles.push_back(tabToggle);
		if(tab->getEnableable()) {
			ofxUIToggle* enableToggle = new ofxUIToggle("", tab->getEnabled(), enableWidth, tabToggle->getRect()->height);
			addWidgetRight(enableToggle);
			enableToggles.push_back(enableToggle);
		}
		autoSizeToFitWidgets();
	}
	void select(string name) {
		for(int i = 0; i < tabToggles.size(); i++) {
			ofxUILabelToggle *tabToggle = tabToggles[i];		
			if(tabToggle->getName() == name) {
				tabToggle->setValue(true);
				currentTab = i;
			} else {
				tabToggle->setValue(false); 
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
	void update() {
		if (!visible || tabs.empty()) return;
		getCurrent()->update();
	}
	void draw() {
		glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
		
        ofPushStyle();
        ofPushMatrix();
        ofNoFill();
        ofTranslate(getRect()->width, 0);
        tabs[currentTab]->draw();
        ofPopMatrix();
        ofPopStyle();
		
		ofxUICanvas::draw();
	}
	void keyPressed(int key) {
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
	void loadSettings(const string &fileName);
	void saveSettings(const string &fileName);
};