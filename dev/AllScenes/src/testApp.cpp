#include "testApp.h"
#include "ofxAutoControlPanel.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";


// scenes
#include "BigBox.h"
#include "Balance.h"


ofxAutoControlPanel gui;
vector<ramSceneBase*> scenes;
BigBox bigbox;
Balance balance;



#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(230);
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	oscReceiver.setup(10001);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	// gui setup
	ofxControlPanel::setTextColor(simpleColor(255,0,0,100));
	ofxControlPanel::setBackgroundColor(simpleColor(0,0,0,20));
	gui.setup(ofGetWidth()-100, ofGetHeight());
	
	// scenes
	scenes.push_back(bigbox.getPtr());
	scenes.push_back(balance.getPtr());
	
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
	{
		scenes.at(i)->update();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->draw();
	}
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
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->drawActor(actor);
	}
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->drawRigid(rigid);
	}
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

