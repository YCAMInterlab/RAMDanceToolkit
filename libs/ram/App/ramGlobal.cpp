#include "ramGlobal.h"

#include <numeric>

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

void ramInit()
{
	ramGetGUI().setup();
//	ramGetGUI().loadFont(ramToResourcePath("Fonts/din-webfont.ttf"), 10);
//	ramGetGUI().loadCameraSettings("settings.camera.xml");
}
