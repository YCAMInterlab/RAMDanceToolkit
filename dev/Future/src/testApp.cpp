#include "testApp.h"
#include "ofxSimpleParticleEngine.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";


ofxSimpleParticleEngine pe;
Noise *noise;

ramGhost ghost;

bool bGhost, bActor, bParticle;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10001);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	//
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	//
	pe.setup(100000);
	pe.addForceFactor(new Gravity);
	pe.addForceFactor(new Floor);
	
	bGhost = false;
	bActor = false;
	bParticle = false;
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	ghost.update( getActor(myActorName) );
	
	for (int i=0; i<ghost.getActor().getNumNode(); i++)
	{
		ramNode &node = ghost.getActor().getNode(i);
		for(int j=0; j<10; j++) pe.emit( node.getPosition() );
	}
	pe.update();
	
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
    ofSetColor(80);
	
	ramCameraBegin();
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		ofPushStyle();
		ofNoFill();
		
		if (bGhost)
		{
			for (int i=0; i<ghost.getActor().getNumNode(); i++)
			{
				ramNode &node = ghost.getActor().getNode(i);
				const int size = (i==ramActor::JOINT_HEAD) ? 6 : 3;
				
				node.transformBegin();
				ofBox(size);
				node.transformEnd();
				
				if (node.hasParent())
					ofLine(node, *node.getParent());
			}
		}
		
		if (bParticle)
		{
			ofSetColor(255);
			pe.draw();
		}
		
		ofPopStyle();
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	ramCameraEnd();
}




//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	
	if (!bActor) return;
	
	glEnable(GL_DEPTH_TEST);
	ofPushStyle();
	ofNoFill();
	ofSetColor(190);
	
	for (int i=0; i<actor.getNumNode(); i++)
    {
		const ramNode &node = actor.getNode(i);
		const int size = (i==ramActor::JOINT_HEAD) ? 6 : 3;
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		node.transformBegin();
		ofBox(size);
		node.transformEnd();
		glPopAttrib();
		glPopMatrix();
        
        if (node.hasParent())
            ofLine(node, *node.getParent());
    }
	
    ofPopStyle();
	glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}



#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
		case 'f': ofToggleFullscreen(); break;
		case 'a': bActor ^= true; break;
		case 's': bGhost ^= true; break;
		case 'd': bParticle ^= true; break;
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

