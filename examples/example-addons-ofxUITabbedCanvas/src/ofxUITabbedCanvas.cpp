#include "ofxUITabbedCanvas.h"

//--------------------------------------------------------------
void ofxUITabbedCanvas::loadSettings(const string &fileName)
{
    string baseName = fileName;
    ofStringReplace(baseName, "XML", "xml");
    
    for (int i=0; i<tabs.size(); i++) {
        string file = baseName;
        ofStringReplace(file, ".xml", ofToString(i)+".xml");
        tabs[i]->loadSettings(file);
    }
}

//--------------------------------------------------------------
void ofxUITabbedCanvas::saveSettings(const string &fileName)
{
    string baseName = fileName;
    ofStringReplace(baseName, "XML", "xml");
    
    for (int i=0; i<tabs.size(); i++) {
        string file = baseName;
        ofStringReplace(file, ".xml", ofToString(i)+".xml");
        tabs[i]->saveSettings(file);
    }
}
