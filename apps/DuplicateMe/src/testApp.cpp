#include "testApp.h"


int numDuplicate = 50;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::BLACK );
	
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	
	/*!
	 GUI setup
	 */
	gui.setup();
	gui.loadFont("Fonts/din-webfont.ttf", 11);
	gui.loadCameraSettings("settings.camera.xml");
	
	
	/*!
	 GUI: Actor scale / move
	 */
	gui.addSlider("Actor Scale", 1.0, 0.1, 3);
	gui.addSlider("Actor Position:x", 0, -600, 600);
	gui.addSlider("Actor Position:y", 0, -600, 600);
	
	gui.addToggle("Toggle actor");
	gui.addSlider("numDuplicate", 50, 1, 100);
	
	
	/*!
	 (user code......)
	 */
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	numDuplicate = gui.getValueF("numDuplicate");
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(gui.getValueI("Floor pattern"),
				  gui.getValueF("Floor size"),
				  gui.getValueF("Grid size"),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_LIGHT-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	float radius = 300;
	float radian = 2 * M_PI / numDuplicate;
	
	for (int i=0; i<numDuplicate; i++)
	{
		float x = cos( radian * i ) * radius;
		float z = sin( radian * i ) * radius;
		
		float scale = gui.getValueF("Actor Scale");
		float posX = gui.getValueF("Actor Position:x");
		float posZ = gui.getValueF("Actor Position:y");
		
		ofColor c1 = ramColor::RED_DEEP;
		ofColor c2 = ramColor::BLUE_LIGHT;
		
		c1.g += i*5;
		c2.b += i*5;
		
		ofPushMatrix();
		ofTranslate(x, 0, z);
		ofRotateY(360/numDuplicate * i);
		if( i==0 )
		{
			ofTranslate(posX, 0, posZ);
			ofScale(scale, scale, scale);
		}
		
		
		
		bool showActor = gui.getValueB("Toggle actor");
		
		if(showActor)
		{
			ramBasicActor(actor, c1, c2, shadowMat.getPtr());
		}
		else
		{
			ofSetColor(c1);
			ramNode &node = actor.getNode(ramActor::JOINT_LEFT_HAND);
			node.transformBegin();
			ofBox(5);
			node.transformEnd();
			
			ofSetColor(c2);
			ramNode &node2 = actor.getNode(ramActor::JOINT_RIGHT_HAND);
			node2.transformBegin();
			ofBox(5);
			node2.transformEnd();
		}
		
		
		ofPopMatrix();
	}
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

