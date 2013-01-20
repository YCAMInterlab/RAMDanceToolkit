#include "testApp.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";


#include "BigBox.h"

#include "ofxAutoControlPanel.h"
ofxAutoControlPanel gui;
vector<ramSceneBase*> scenes;
LengthenJoints joints;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	
	// gui setup
	ofxControlPanel::setTextColor(simpleColor(255,0,0,100));
	ofxControlPanel::setBackgroundColor(simpleColor(0,0,0,20));
	gui.setup(ofGetWidth()-100, ofGetHeight());
	
	// scenes
	scenes.push_back(joints.getPtr());
	
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->setup();
		scenes.at(i)->refreshControlPanel(gui);
	}
	
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	ramCameraManager::instance().getActiveCamera().setPosition(0, 300, 300);
	ramCameraManager::instance().getActiveCamera().lookAt(ofVec3f(0,0,0));
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	joints.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackgroundGradient( ofColor( 240 ), ofColor( 120 ) );
	joints.draw();
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
//	ramBasicFloor(600., 50., ofColor(255), ofColor(200));
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	joints.drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
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

