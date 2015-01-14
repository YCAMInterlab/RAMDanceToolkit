#include "ofxUITabbedCanvas.h"

ofColor
uiThemecb(128, 192),
uiThemeco(192, 192),
uiThemecoh(192, 192),
uiThemecf(255, 255),
uiThemecfh(160, 255),
uiThemecp(128, 192),
uiThemecpo(255, 192);

ofxUICanvasPlus::ofxUICanvasPlus() {
	setUIColors(uiThemecb, uiThemeco, uiThemecoh, uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
}

int ofxUICanvasPlus::getChoice(ofxUIRadio* radio) {
	vector<ofxUIToggle*> toggles = radio->getToggles();
	for(int i = 0; i < toggles.size(); i++) {
		if(toggles[i]->getValue()) {
			return i;
		}
	}
	return -1;
}

int ofxUICanvasPlus::getChoice(ofxUIEventArgs& e, ofxUIRadio* radio) {
	vector<ofxUIToggle*> toggles = radio->getToggles();
	for(int i = 0; i < toggles.size(); i++) {
		if(e.widget == toggles[i]) {
			return i;
		}
	}
	return -1;
}

string getXmlSafeName(const string& name) {
	string safe;
	for(int i = 0; i < name.size(); i++) {
		char cur = name[i];
		if((cur >= 'a' && cur <= 'z')
		   || (cur >= 'A' && cur <= 'Z')
		   || (cur >= '0' && cur <= '9')
		   || (cur == '-')) {
			safe += cur;
		}
	}
	return safe;
}

void ofxUITabbedCanvas::loadSettings(const string &filename) {
	ofxXmlSettings xml;
	xml.loadFile(filename);
	loadSettingsFromXml(xml);
	for (int i=0; i<tabs.size(); i++) {
		string curTabName = getXmlSafeName(tabs[i]->getTabName());
		if(curTabName != "Actors") {
			xml.pushTag(curTabName);
			tabs[i]->loadSettingsFromXml(xml);
			xml.popTag();
		}
	}
	xml.saveFile(filename);
}

void ofxUITabbedCanvas::saveSettings(const string &filename) {
	ofxXmlSettings xml;
	saveSettingsToXml(xml);
	for (int i=0; i<tabs.size(); i++) {
		string curTabName = getXmlSafeName(tabs[i]->getTabName());
		if(curTabName != "Actors") {
			xml.addTag(curTabName);
			xml.pushTag(curTabName);
			tabs[i]->saveSettingsToXml(xml);
			xml.popTag();
		}
	}
	xml.saveFile(filename);
}

void ofxUICanvasPlus::saveSettingsToXml(ofxXmlSettings& xml) {
	for(int i = 0; i < widgetsWithState.size(); i++) {
		int index = xml.addTag("Widget");
		if(xml.pushTag("Widget", index)) {
			xml.setValue("Kind", widgetsWithState[i]->getKind(), 0);
			xml.setValue("Name", widgetsWithState[i]->getName(), 0);
			//writeSpecificWidgetData(widgetsWithState[i], &xml);
            widgetsWithState[i]->saveState(&xml);
		}
		xml.popTag();                            
	}
}

void ofxUICanvasPlus::loadSettingsFromXml(ofxXmlSettings& xml) {
	int widgetTags = xml.getNumTags("Widget"); 
	for(int i = 0; i < widgetTags; i++) {
		xml.pushTag("Widget", i);
		string name = xml.getValue("Name", "NULL", 0);
		ofxUIWidget *widget = getWidget(name); 
		if(widget != NULL) {
			//loadSpecificWidgetData(widget, &xml);
            widget->loadState(&xml);
			triggerEvent(widget); 
		}
		xml.popTag(); 
	}
}