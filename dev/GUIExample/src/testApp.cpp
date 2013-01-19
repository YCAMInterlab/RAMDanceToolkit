#include "testApp.h"
#include "ofxSimpleParticleEngine.h"


const string myActorName = "default";


#include "ofxAutoControlPanel.h"
ofxAutoControlPanel gui;
const string sliderEmphasis = "emphasis";
const string sliderFreshness = "freshness";


ofxSimpleParticleEngine pe;
Noise *noise;


ramGhost ghost;
bool bGhost, bActor, bParticle;

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
	
	bGhost = false;
	bActor = false;
	bParticle = false;
	
	//
	pe.setup(100000);
	pe.addForceFactor(new Gravity);
	pe.addForceFactor(new Floor);
	
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
}


//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	if (gui.hasValueChanged( variadic(sliderEmphasis)(sliderFreshness) ))
	{
		ghost.setEmphasis( gui.getValueF(sliderEmphasis) );
		ghost.setFreshness( gui.getValueF(sliderFreshness) );
	}
	
	for (int i=0; i<ghost.getActor().getNumNode(); i++)
	{
		ramNode &node = ghost.getActor().getNode(i);
		for(int j=0; j<10; j++) pe.emit( node.getPosition() );
	}
	ghost.update(getActor(myActorName));
	pe.update();
}


//--------------------------------------------------------------
void testApp::draw()
{
	ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
    ofSetColor(255, 0, 0);
	
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
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glEnable(GL_DEPTH_TEST);
//	glPushMatrix();
//    ofEnableSmoothing();
//	ofNoFill();
//	ramCameraBegin();
//    {
//		ramActor &actor = ghost.getActor();
//		for (int i=0; i<actor.getNumNode(); i++)
//		{
//			ramNode &node = actor.getNode(i);
//			ofBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
//			
//			if (node.hasParent())
//				ofLine(node, *node.getParent());
//		}
//	}
//	ramCameraEnd();
//	ofDisableSmoothing();
//	glPopMatrix();
//	glDisable(GL_DEPTH_TEST);
//	glPopAttrib();
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
	if (!bActor) return;
	
	glEnable(GL_DEPTH_TEST);
	ofPushStyle();
	ofNoFill();
	ofSetColor(0);
	
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



// ram methods
//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
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