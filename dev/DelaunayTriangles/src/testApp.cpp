#include "testApp.h"

/*!
 Scenes
 */
#include "CameraMover.h"
#include "Triangles.h"
Triangles tri;
CameraMover mover;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
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
	gui.addSlider("Actor Scale", 1.0, 0.1, 20);
	gui.addSlider2D("Actor Position", "Actor Position", 0, 0, -300, 300, -300, 300, true);
	
	
	/*!
	 scenes setup
	 */
	scenes.push_back( tri.getPtr() );
	scenes.push_back( mover.getPtr() );
	gui.addScenePanels(scenes);
	
	
	/*!
	 (user code......)
	 */
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->setMatrix(shadowMat);
	
	scenes.front()->enable();
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	
	/* Scenes update */
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		scene->update();
		
		/* Enable / Disable scenes */
		string key = scene->getSceneEnableKey();
		
		if ( gui.hasValueChanged(key) )
		{
			scene->setEnabled( gui.getValueB(key) );
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	/* Scenes draw */
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->draw();
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
//	if ( gui.getValueB("Draw Actor") )
	{
		ramBasicActor(actor, shadowMat.getPtr());
	}
	
	
	for (int i=0; i<scenes.size(); i++) scenes.at(i)->drawActor(actor);
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

