#include "testApp.h"


/*!
 Scenes
 */
#include "BasicActor.h"
#include "BigBox.h"
#include "Bullet.h"
#include "Future.h"
#include "Line.h"
#include "DuplicateMe.h"
#include "Stamp.h"
#include "UpsideDown.h"
#include "Expansion.h"
#include "Graph3D.h"

BasicActor basicActor;
BigBox bigbox;
Bullet bullet;
Future future;
Line drawLines;
DuplicateMe duplicateMe;
Stamp stamp;
UpsideDown upsideDown;
Expansion expansion;
Graph3D graph3D;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
	/// ram setup
	// ------------------
	ramInit();
	oscReceiver.setup(10000);
	
	
	/// scenes setup
	// ------------------
	vector<ramBaseScene*> scenes;
	scenes.push_back( graph3D.getPtr() );
	scenes.push_back( drawLines.getPtr() );
	scenes.push_back( basicActor.getPtr() );
	scenes.push_back( upsideDown.getPtr() );
	scenes.push_back( expansion.getPtr() );
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( future.getPtr() );
	scenes.push_back( bullet.getPtr() );
	scenes.push_back( duplicateMe.getPtr() );
	scenes.push_back( stamp.getPtr() );
	
	sceneManager.setup(scenes);
	
	for (int i = 0; i < 5; i++)
	{
		ofEasyCam *cam = ramCameraManager::instance().createCamera<ofEasyCam>();
		cam->disableMouseInput();
	}
	
	for (int i = 0; i < 5; i++)
	{
		ramCameraManager::instance().setActiveCamera(i + 1);
		ramCameraManager::instance().rollbackDefaultCameraSetting(i);
	}
	
	ramCameraManager::instance().setActiveCamera(0);

}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	
	/// Scenes update
	// ------------------
	sceneManager.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// Scenes draw
	// ------------------
//	sceneManager.draw();
	
	setDrawFloorAuto(true);
	
	int main_screen_width = 1920;
	int main_screen_height = 1200;
	
	main_screen_width = min(main_screen_width, ofGetWidth());
	main_screen_height = min(main_screen_height, ofGetHeight());
	
	ramCameraManager::instance().setActiveCamera(0);
	
	ofPushView();
	ofViewport(0, 0, 1920, 1200);
	
	ramBeginCamera();
	drawFloor();
	ramEndCamera();
	
	sceneManager.draw();
	ofPopView();
	
	int screen_w = 1280, screen_h = 720;
	
	int inv_screen_height = ofGetHeight() - screen_h;
	
	for (int i = 0; i < 5; i++)
	{
		ofPushView();
		
		ofCamera *screen_camera = ramCameraManager::instance().getCamera(i + 1);
		
		ofViewport(ofRectangle(main_screen_width + i * screen_w, inv_screen_height, screen_w, screen_w));
		ramCameraManager::instance().setActiveCamera(i + 1);
		
		screen_camera->begin();
		drawFloor();
		screen_camera->end();
		
		sceneManager.draw();
		
		ofPopView();
	}
	
	ramCameraManager::instance().setActiveCamera(0);
	
	setDrawFloorAuto(false);

}

#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
//	/// Scenes drawActor
//	// ------------------
//	sceneManager.drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
//	/// Scenes drawActor
//	// ------------------
//	sceneManager.drawRigid(rigid);
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
