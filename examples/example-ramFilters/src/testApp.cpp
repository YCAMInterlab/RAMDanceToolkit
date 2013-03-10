#include "testApp.h"

ramExpansion expansion;
ramLowPassFilter lowpass;
ramGhost ghost;
ramPendulum pendulum;
ramUpsideDown upsideDown;
ramTimeShifter timeShifter;

bool drawName;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	/// ram setup
	// ------------------
	ramInitialize(10000);
	
	ramGetGUI().addPanel( &expansion );
	ramGetGUI().addPanel( &lowpass );
	ramGetGUI().addPanel( &ghost );
	ramGetGUI().addPanel( &pendulum );
	ramGetGUI().addPanel( &upsideDown );
	ramGetGUI().addPanel( &timeShifter );
	
	drawName = false;
}

//--------------------------------------------------------------
void testApp::update()
{
	
}	

//--------------------------------------------------------------
void testApp::draw()
{
	ramBeginCamera();
	
	if (getNumNodeArray() > 0)
	{
		// original data
		const ramNodeArray &NA = getNodeArray(0);
		
		
		// filtering
		// each of filter can be enable/disable from GUI
		const ramNodeArray &f1 = lowpass.update(NA);
		const ramNodeArray &f2 = expansion.update(f1);
		const ramNodeArray &f3 = upsideDown.update(f2);
		const ramNodeArray &f4 = timeShifter.update(f3);
		const ramNodeArray &f5 = ghost.update(f4);
		const ramNodeArray &f6 = pendulum.update(f5);
		
		
		// draw filterd actor with blue
		ofSetColor(ramColor::BLUE_LIGHT);
		for (int n=0; n<NA.getNumNode(); n++)
		{
			const ramNode& node = f6.getNode(n);
			node.beginTransform();
			ofNoFill();
			ofBox(5);
			ofDrawAxis(5);
			node.endTransform();
			
			if (drawName)
				node.drawNodeName();
		}
		
		
		// draw lines between original and filterd with gray
		ofSetColor(ramColor::LIGHT_GRAY_ALPHA);
		ramDrawNodeCorresponds(NA, f3);
	}
	
	ramEndCamera();
	
	ofDrawBitmapString("press [n] to draw node name", ofGetWidth()/2, 20);
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	// draw basic actor with yellow
	ofSetColor(ramColor::YELLOW_DEEP);
	ofFill();
	ramDrawBasicActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{
	
}


#pragma mark - ram Events
//--------------------------------------------------------------
void testApp::onActorSetup(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onActorExit(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidSetup(const ramRigidBody &rigid)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidExit(const ramRigidBody &rigid)
{
	
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == 'n')
	{
		
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
