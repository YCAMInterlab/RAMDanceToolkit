#include "testApp.h"
#include "ofxAutoControlPanel.h"

static const string myActorName = "default";
ofxAutoControlPanel gui;


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
	ofxControlPanel::setTextColor(0xffffff);
	ofxControlPanel::setBackgroundColor(0x000000);
	gui.setup();
	gui.addPanel("Draw Line");
	gui.addPanel("ok");
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
//	if (gui.hasValueChanged( variadic(sliderEmphasis)(sliderFreshness) ))
//	{
//		ghost.setEmphasis( gui.getValueF(sliderEmphasis) );
//		ghost.setFreshness( gui.getValueF(sliderFreshness) );
//	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramBasicActor(actor);
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

