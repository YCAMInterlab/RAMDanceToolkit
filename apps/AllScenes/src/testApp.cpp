#include "testApp.h"

// gui
#include "ofxAutoControlPanel.h"

ofxAutoControlPanel gui;
vector<ramSceneBase*> scenes;



#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(230);
	
	/*!
		ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	
	/*!
		gui setup
	 */
	ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 100));
	ofxControlPanel::setBackgroundColor(simpleColor(0, 0, 0, 90));
	gui.setup();
	
	
	gui.addPanel("Config");
	vector<string> floors;
	floors.push_back("CHECKER_PATTERN");
	floors.push_back("GRID_LINES");
	floors.push_back("NONE");
	gui.addSlider("Background", 0, 0, 255);
	gui.addMultiToggle("Floor pattern", 0, floors);
	gui.addSlider("Floor size", 600.0, 100.0, 1000.0);
	gui.addSlider("Grid size", 50.0, 10.0, 100.0);
	
	/*!
	 scenes setup
	 */
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->setup();
		scenes.at(i)->refreshControlPanel(gui);
	}
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->update();
	
	if (gui.hasValueChanged( variadic("Background") ))
	{
		ofBackground(gui.getValueF("Background"));
		gui.clearAllChanged();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->draw();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(gui.getValueI("Floor pattern"),
				  gui.getValueF("Floor size"),
				  gui.getValueF("Grid size"));
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->drawRigid(rigid);
}





#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

