#include "ramSharedData.h"
#include "ramBaseApp.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

#include <numeric>

void ramInitialize(int oscPort)
{
	static bool inited = false;
	if (inited) return;
	inited = true;
	
	// !!!:
	ofLogWarning("ramInitialize(10000) in ramSharedData.cpp: ofSetLogLevel(OF_LOG_SILENT) will be deleted when Fonts probrem in ofxUI is fixed.");
	ofLogWarning("see addons/ofxUI.patch");
	// to apply `$ patch -p0 < ../ofxUI.patch` at ofxUI directory
	
	ramSharedData::instance().setup();
	ramActorManager::instance().setup();
	ramActorManager::instance().setupOscReceiver(oscPort);
	
	ramPhysics::instance();
	
	ramGetGUI().setup();
}

ramSharedData* ramSharedData::_instance = NULL;

ramSharedData& ramSharedData::instance()
{
	if (_instance == NULL)
		_instance = new ramSharedData;
	return *_instance;
}

void ramSharedData::setup()
{
	shadow.setup();
}