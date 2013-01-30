#include "ramGlobal.h"
#include "ramBaseApp.h"
#include <numeric>

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

void ramInit()
{
	ofLogWarning("test");
	ofSetLogLevel(OF_LOG_SILENT);
	
	ramGetGUI().setup();
//	ramGetGUI().loadFont(ramToResourcePath("Fonts/din-webfont.ttf"), 10);
//	ramGetGUI().loadCameraSettings("settings.camera.xml");
}
