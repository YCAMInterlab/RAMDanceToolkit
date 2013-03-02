#include "ramPresetTab.h"

#include "ramCameraManager.h"

ramPresetTab::ramPresetTab()
:ofxUITab("Presets", false)
,cameraPreset(1)
{
	// should probably be a list of named presets instead of a grid
	matrix = addToggleMatrix("Presets", 4, 4);
	matrix->setAllowMultiple(false);
	addSpacer();
	
	vector<string> cameraPresetNames;
	cameraPresetNames.push_back("Low");
	cameraPresetNames.push_back("High");
	cameraPresetNames.push_back("Overhead");
	cameraPresetRadio = addRadio("Camera position", cameraPresetNames);
	cameraPresetRadio->getToggles()[cameraPreset]->setValue(true);
	
	autoSizeToFitWidgets();
	
	ofAddListener(ofEvents().update, this, &ramPresetTab::setupCamera);
	ofAddListener(newGUIEvent, this, &ramPresetTab::guiEvent);
}

void ramPresetTab::setupCamera(ofEventArgs& e) {
	if(ofGetFrameNum() < 10) {
		cameraPresetRadio->getToggles()[cameraPreset]->setValue(true);
		cameraPresetRadio->getToggles()[cameraPreset]->triggerSelf();
	} else {
		ofRemoveListener(ofEvents().update, this, &ramPresetTab::setupCamera);
	}
}

void ramPresetTab::guiEvent(ofxUIEventArgs &e) {
	int choice = getChoice(e, cameraPresetRadio);
	if(choice != -1) {
		int indices[] = {0, 1, 5};
		int choice = getChoice(cameraPresetRadio);
		ramCameraManager::instance().rollbackDefaultCameraSetting(indices[choice]);
	}
}