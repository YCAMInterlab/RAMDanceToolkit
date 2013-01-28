#include "ramGlobal.h"

static ramGlobalContext *ctx = NULL;

ramGlobalContext& ramGlobal()
{
	if (ctx == NULL)
	{
		ctx = new ramGlobalContext;
	}
	return *ctx;
}

ramControlPanel& ramGlobalContext::getGUI()
{
	return *gui;
}

void ramGlobalContext::init()
{
	gui = new ramControlPanel;
	gui->setup();
	
//	gui->loadFont("Fonts/din-webfont.ttf", 10);
	gui->loadCameraSettings("settings.camera.xml");
}
