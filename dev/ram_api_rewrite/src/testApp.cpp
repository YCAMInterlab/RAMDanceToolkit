#include "testApp.h"


const string myActorName = "Ando_2012-08-29_13-48-10";

#include "ofxBt.h"

ramBoxPrimitive *cube;
ramSpherePrimitive *sphere;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
	sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
}

//--------------------------------------------------------------
void testApp::update()
{
	while (oscReceiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		updateWithOscMessage(m);
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableSmoothing();
	
	ramNodeFinder nf(ramActor::JOINT_ADBOMEN);
	
	ramCameraBegin();
    
	ofNoFill();
	
	cube->draw();
	sphere->draw();
    
//	ramPhysics::instance().debugDraw();
	
    ofDisableSmoothing();
	
	ramCameraEnd();
}


// ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
    
}

// ram methods
//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
    for (int i=0; i<actor.getNumNode(); i++)
    {
        ramNode &node = actor.getNode(i);
        
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            ofPushStyle();

            ofSetColor(255, 0, 0);
            ofNoFill();
			
            ofSetColor(0, 255, 0);
            ramBox(node, 10);
            
            ofPopStyle();
            glPopMatrix();
            glPopAttrib();
        }
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
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
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