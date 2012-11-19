#include "testApp.h"


const string myActorName = "Ando_2012-08-29_13-48-10";
const string myRigidName = "R_Wand500_2012-08-29_13-48-10";


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	mySession.setup(myActorName, ramSession::RAM_ACTOR);
	myRigidSession.setup(myRigidName, ramSession::RAM_RIGID_BODY);
	
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
	
	mySession.update();
	myRigidSession.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableSmoothing();
	
	ramNodeFinder nf(ramActor::JOINT_ADBOMEN);
	
	ramCameraBegin();
    
    {
        vector<ramNode> nodes = nf.get();
        for (int i=0; i<nodes.size(); i++)
        {
            ramNode &node = nodes.at(i);
            
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            ofPushStyle();
            
            ofNoFill();
            ofSetLineWidth(3);
            ofSetHexColor(0x00DDFF);
            node.transformBegin();
            ofBox(100);
            node.transformEnd();
            
            ofPopStyle();
            glPopMatrix();
            glPopAttrib();
        }
    }
    
	
	if (mySession.isPlaying())
	{
		ramActor& actor = mySession.getNextFrameActor();
		for	(int i=0; i<actor.getNumNode(); i++)
		{
			ramNode& node = actor.getNode(i);
			ofBox(node, 10);
		}
	}
	
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
            node.transformBegin();
            ofBox(10);
            node.transformEnd();

            ofSetColor(0, 255, 0);
            ofBox(node, 20);
            
            ofPopStyle();
            glPopMatrix();
            glPopAttrib();
        }
    
        
        if (node.hasParent())
        {
            ramNode *parent_node = node.getParent();
                
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            ofPushStyle();
            
            ofSetColor(255);
            ofLine(node, *parent_node);
            
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
    for (int i=0; i<rigid.getNumNode(); i++)
    {
        ramNode &node = rigid.getNode(i);

        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            ofPushStyle();

            ofSetColor(255, 0, 0);
            ofNoFill();
            node.transformBegin();
            ofBox(10);
            node.transformEnd();

            ofSetColor(0, 255, 0);
            ofBox(node, 20);

            ofPopStyle();
            glPopMatrix();
            glPopAttrib();
        }
    }
}




//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key)
	{
		case 'r':
			mySession.startRecording();
			break;

		case 's':
			mySession.stopRecording();
			break;
			
		case 'p':
			mySession.play();
			break;
			
		case 'q':
			mySession.stop();
			break;

		default:
			break;
	}
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