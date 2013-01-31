#include "ramGlobal.h"
#include "ramBaseApp.h"
#include "ramControlPanel.h"
#include <numeric>

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

void ramInit()
{
	// !!!:
	ofLogWarning("ramInit() in ramGlobal.cpp: ofSetLogLevel(OF_LOG_SILENT) will be deleted when Fonts probrem in ofxUI is fixed.");
	ofSetLogLevel(OF_LOG_SILENT);
	
	ramGetGUI().setup();
//	ramGetGUI().loadFont(ramToResourcePath("Fonts/din-webfont.ttf"), 10);
}
