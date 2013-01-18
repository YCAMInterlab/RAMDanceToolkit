#include "testApp.h"


const string myActorName = "Ando_2012-09-01_18-49-10";


#include "ofxAutoControlPanel.h"
ofxAutoControlPanel gui;
ramGhost ghost;

vector<string> slidersKeys;
const string sliderEmphasis = "emphasis";
const string sliderFreshness = "freshness";


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10001);

	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	
	// gui
	ofxControlPanel::setTextColor(0xffffff);
	ofxControlPanel::setBackgroundColor(0x000000);
	gui.setup();
	gui.addPanel("ramGhost");
	gui.addSlider(sliderEmphasis, 20, 0, 100);
	gui.addSlider(sliderFreshness, 20, 0, 1000);
	slidersKeys.push_back(sliderEmphasis);
	slidersKeys.push_back(sliderFreshness);
}


//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	if (gui.hasValueChanged(slidersKeys))
	{
		ghost.setEmphasis( gui.getValueF(sliderEmphasis) );
		ghost.setFreshness( gui.getValueF(sliderFreshness) );
	}
	ghost.update(getActor(myActorName));
}


//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableSmoothing();
	ofNoFill();
	ramCameraBegin();
    {
		ramActor &actor = ghost.getActor();
		for (int i=0; i<actor.getNumNode(); i++)
		{
			ramNode &node = actor.getNode(i);
			ofBox(node, 10);
		}
	}
	ramCameraEnd();
	ofDisableSmoothing();
}


// ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
    ramBasicFloor(600, 50);
}


// ram methods
//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
    for (int i=0; i<actor.getNumNode(); i++)
    {
        ramNode &node = actor.getNode(i);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		ofPushStyle();
		{
			ofSetColor(255, 0, 0);
			ofNoFill();
			ofSetColor(0, 255, 0);
			ramBox(node, 20);
		}
		ofPopStyle();
		glPopMatrix();
		glPopAttrib();
    }
}



// ram methods
//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


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