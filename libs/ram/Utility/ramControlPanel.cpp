#include "ramControlPanel.h"

#include "ramSceneBase.h"

ramControlPanel *ramControlPanel::_instance = NULL;

ramControlPanel& ramControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramControlPanel();
	}
	return *_instance;
}

ramControlPanel::ramControlPanel()
{
	mR = 50;
	mG = 50;
	mB = 50;
	mUseBgSlider = true;
	mFloorPattern = ramFloor::FLOOR_NONE;
	mFloorSize = 600.0;
	mGridSize = 50.0;
	mLabelCamPos = new ofxUILabel("x:0 y:0 z:0", OFX_UI_FONT_MEDIUM);
}

void ramControlPanel::setup()
{
	
	/// Event hooks
	// -------------------------------------
	ofAddListener(ofEvents().update, this, &ramControlPanel::update);
	
	
	
	/// First panel
	// -------------------------------------
	/// panel
	mPanelGeneral = new ofxUICanvas(0, 0, kLength+kXInit*2.0, ofGetHeight());
	mPanelGeneral->addWidgetDown(new ofxUILabel("RamDanceToolkit", OFX_UI_FONT_LARGE));
	
	
	/// full screan
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addWidgetDown(new ofxUIToggle(32, 32, false, "FullScrean"));
	
	
	/// background color
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addWidgetDown(new ofxUIToggle(32, 32, true, "Use Background Slider"));
	mPanelGeneral->addSlider("BG:R", 0, 255, &mR, 95, kDim);
	mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	mPanelGeneral->addSlider("BG:G", 0, 255, &mG, 95, kDim);
	mPanelGeneral->addSlider("BG:B", 0, 255, &mB, 95, kDim);
	mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
	
	/// floor pattern
	vector<string> floors = ramFloor::getFloorNames();
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addRadio("Floor Patterns", floors, OFX_UI_ORIENTATION_VERTICAL, kDim, kDim);
	mPanelGeneral->addSlider("Floor Size", 100, 1000, &mFloorSize, kLength/2-kXInit, kDim);
	mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	mPanelGeneral->addSlider("Grid Size", 20, 200, &mGridSize, kLength/2-kXInit, kDim);
	mPanelGeneral->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
	
	/// camera positions
	ofxXmlSettings camSet( kCamSettingFile );
	vector<string> positions = ramCameraSettings::getCamNames(camSet);
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addRadio("Camera Preset", positions, OFX_UI_ORIENTATION_VERTICAL, kDim, kDim);
	
	
	/// Camera Position
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addWidgetDown(new ofxUILabel("Camera Position", OFX_UI_FONT_MEDIUM));
	mPanelGeneral->addWidgetDown(mLabelCamPos);

	
	/// add panel to canvas
	ofAddListener(mPanelGeneral->newGUIEvent, this, &ramControlPanel::guiEvent);
	mTabbedCanvas.add(mPanelGeneral);
	mTabbedCanvas.loadSettings("GUI/guiSettings.xml");
}

void ramControlPanel::update(ofEventArgs &e)
{
	if (mUseBgSlider) ofBackground( ofColor(mR, mG, mB) );
	
	const ofVec3f &camPos = ramCameraManager::instance().getActiveCamera().getPosition();
	stringstream pos;
	pos <<
	" X:" << (int)camPos.x <<
	" Y:" << (int)camPos.y <<
	" Z:" << (int)camPos.z << endl;
	
	mLabelCamPos->setLabel( pos.str() );
}

void ramControlPanel::setupSceneToggles(vector<ramSceneBase*>& scenes)
{
	const int size = scenes.size();
	
	if (size <= 0)
	{
		mPanelGeneral->addSpacer(kLength, 2);
		mPanelGeneral->addLabel("No scenes are assigned.");
		return;
	}
	
	const int numCol = 5;
	const int numRow = 2 / numCol + 1;
	mSceneToggles = new ofxUIToggleMatrix(kDim*3, kDim*2, numRow, numCol, "Scenes");
	mPanelGeneral->addSpacer(kLength, 2);
	mPanelGeneral->addWidgetDown(mSceneToggles);
}

void ramControlPanel::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	
	if ( name == "Use Background Slider" )
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		mUseBgSlider = toggle->getValue();
	}
	
	if ( name == "FullScrean" )
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		ofSetFullscreen(toggle->getValue());
	}
	
	
	/// floor patterns
	if		( name == "NONE" )			 mFloorPattern = ramFloor::FLOOR_NONE;
	else if ( name == "PLANE" )			 mFloorPattern = ramFloor::FLOOR_PLANE;
	else if ( name == "CHECKER_PATTERN" )mFloorPattern = ramFloor::FLOOR_CHECKER_PATTERN;
	else if ( name == "GRID_LINES" )	 mFloorPattern = ramFloor::FLOOR_GRID_LINES;
	
	
	/// camera settings
	if		( name == "CAM_FRONT" )		reloadCameraSetting( 0 );
	else if ( name == "CAM_RIGHT" )		reloadCameraSetting( 1 );
	else if ( name == "CAM_BACK" )		reloadCameraSetting( 2 );
	else if ( name == "CAM_LEFT" )		reloadCameraSetting( 3 );
	else if ( name == "CAM_TOP" )		reloadCameraSetting( 4 );
	else if ( name == "CAM_BOTTOM" )	reloadCameraSetting( 5 );
	else if ( name == "CAM_EDGE_FR" )	reloadCameraSetting( 6 );
	else if ( name == "CAM_EDGE_BR" )	reloadCameraSetting( 7 );
	else if ( name == "CAM_EDGE_BL" )	reloadCameraSetting( 8 );
	else if ( name == "CAM_EDGE_FL" )	reloadCameraSetting( 9 );
}

void ramControlPanel::reloadCameraSetting(const int index)
{
	ofxXmlSettings xml( ramToResourcePath(kCamSettingFile) );
	xml.pushTag("cam", index);
	ramCameraSettings setting( xml );
	ramCameraManager::instance().getActiveCamera().setPosition( setting.pos );
	ramCameraManager::instance().getActiveCamera().lookAt( setting.lookAt );
}



void ramControlPanel::addPanel(ramControllable* control)
{
	ofxUICanvas *panel = new ofxUICanvas(0, 0, ramGetGUI().kLength+ramGetGUI().kXInit*2.0, ofGetHeight());
	control->setupControlPanel(panel);
	getTabbedCanvas().add(panel);
}
