#include "ofxUITabbedCanvas.h"

ofColor uiThemecb(64, 192), uiThemeco(192, 192), uiThemecoh(128, 192), uiThemecf(240, 255), uiThemecfh(128, 255), uiThemecp(96, 192), uiThemecpo(255, 192);

ofxUIXmlCanvas::ofxUIXmlCanvas() {
	setUIColors(uiThemecb, uiThemeco, uiThemecoh, uiThemecf, uiThemecfh, uiThemecp, uiThemecpo);
}

string getXmlSafeName(string name) {
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
	for (int i=0; i<tabs.size(); i++) {
		string curTabName = getXmlSafeName(tabs[i]->getTabName());
		xml.pushTag(curTabName);
		tabs[i]->loadSettingsFromXml(xml);
		xml.popTag();
	}
	xml.saveFile(filename);
}

void ofxUITabbedCanvas::saveSettings(const string &filename) {
	ofxXmlSettings xml;
	for (int i=0; i<tabs.size(); i++) {
		string curTabName = getXmlSafeName(tabs[i]->getTabName());
		xml.addTag(curTabName);
		xml.pushTag(curTabName);
		tabs[i]->saveSettingsToXml(xml);
		xml.popTag();
	}
	xml.saveFile(filename);
}

void ofxUIXmlCanvas::saveSettingsToXml(ofxXmlSettings& xml) {
	for(int i = 0; i < widgetsWithState.size(); i++) {
		int index = xml.addTag("Widget");
		if(xml.pushTag("Widget", index)) {
			xml.setValue("Kind", widgetsWithState[i]->getKind(), 0);
			xml.setValue("Name", widgetsWithState[i]->getName(), 0);
			writeSpecificWidgetData(widgetsWithState[i], &xml); 
		}
		xml.popTag();                            
	}
}

void ofxUIXmlCanvas::loadSettingsFromXml(ofxXmlSettings& xml)
{
	int widgetTags = xml.getNumTags("Widget"); 
	for(int i = 0; i < widgetTags; i++) {
		xml.pushTag("Widget", i);
		string name = xml.getValue("Name", "NULL", 0);
		ofxUIWidget *widget = getWidget(name); 
		if(widget != NULL) {
			loadSpecificWidgetData(widget, &xml); 
			triggerEvent(widget); 
		}
		xml.popTag(); 
	}
}