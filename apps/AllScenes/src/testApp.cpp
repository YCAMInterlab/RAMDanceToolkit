#include "testApp.h"


/*!
 Scenes
 */
#include "BasicActor.h"
BasicActor basicActor;

#include "Line.h"
Line drawLines;

#include "BigBox.h"
BigBox bigbox;

#include "Future.h"
Future future;

#include "Donuts.h"
Donuts donuts;

#include "Stamp.h"
Stamp stamp;

#include "Expansion.h"
Expansion expansion;

#include "Particles.h"
Particles particles;

#include "Abacus.h"
Abacus abacus;

#include "SoundCube.h"
SoundCube soundcube;

#include "UpsideDown.h"
UpsideDown upsideDown;

#include "Kepler.h"
Kepler kepler;

#include "HastyChase.h"
HastyChase hastyChase;

extern bool drawModel;

int active_camera_id = 0;

int main_display_width = 1920;
int main_display_height = 1200;

int screen_width = 1280;
int screen_height = 720;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	/// ram setup
	// ------------------
	ramInitialize();
	oscReceiver.setup(10000);
	
	/// scenes setup
	// ------------------
	vector<ramBaseScene*> scenes;
	scenes.push_back( basicActor.getPtr() );
	scenes.push_back( drawLines.getPtr() );
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( future.getPtr() );
	scenes.push_back( donuts.getPtr() );
	scenes.push_back( stamp.getPtr() );
	scenes.push_back( expansion.getPtr() );
	scenes.push_back( particles.getPtr() );
	scenes.push_back( abacus.getPtr() );
	scenes.push_back( soundcube.getPtr() );
    scenes.push_back( upsideDown.getPtr() );
    scenes.push_back( kepler.getPtr() );
	scenes.push_back( hastyChase.getPtr() );
	sceneManager.setup(scenes);
	
    ofEasyCam *cam = (ofEasyCam*)ramCameraManager::instance().getCamera(0);
    cam->setTranslationKey('z');
    
	// for 5 screens
	for (int i = 0; i < 5; i++)
	{
		ofEasyCam *cam = ramCameraManager::instance().createCamera<ofEasyCam>();
        cam->setTranslationKey('z');
		cam->disableMouseInput();
        cam->setFov(40);
	}
	
	for (int i = 0; i < 5; i++)
	{
		ramCameraManager::instance().setActiveCamera(i + 1);
		ramCameraManager::instance().rollbackDefaultCameraSetting(i);
	}
    
    active_camera_id = 0;
	ramCameraManager::instance().setActiveCamera(active_camera_id);

}

//--------------------------------------------------------------
void testApp::update()
{
	ofViewport(0, 0, main_display_width, main_display_height);
	ramCameraManager::instance().setActiveCamera(active_camera_id);
	ramBeginCamera();
	ramEndCamera();
	
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
	setDrawFloorAuto(true);
    
    int screen_y_offset = main_display_height - screen_height;
    
	for (int i = 0; i < 5; i++)
	{
		ofPushView();
		
		ofCamera *screen_camera = ramCameraManager::instance().getCamera(i + 1);
		
		if (i == 4)
		{
			drawModel = false;
			screen_camera->enableOrtho();
		}
		
		ofViewport(ofRectangle(main_display_width + i * screen_width, screen_y_offset, screen_width, screen_height));
		ramCameraManager::instance().setActiveCamera(i + 1);
		
		screen_camera->begin();
		drawFloor();
		screen_camera->end();
		
		sceneManager.draw();
		
		drawModel = true;
		
		ofPopView();
	}
	
	ramCameraManager::instance().setActiveCamera(active_camera_id);
	
	setDrawFloorAuto(false);
    
    string str;
    str += "active camera id: " + ofToString(active_camera_id) + "\n";
    str += "fps: " + ofToString(ofGetFrameRate(), 0);
    ofDrawBitmapString(str, 400, 20);
    
    ofPushView();
	ofViewport(0, 0, main_display_width, main_display_height);
	ramCameraManager::instance().setActiveCamera(active_camera_id);
	ramBeginCamera();
	drawFloor();
	ramEndCamera();
	
	sceneManager.draw();
	ofPopView();

}

#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    int new_active_camera_id = -1;
    
    if (key == '0')
        new_active_camera_id = 0;
    if (key == '1')
        new_active_camera_id = 1;
    if (key == '2')
        new_active_camera_id = 2;
    if (key == '3')
        new_active_camera_id = 3;
    if (key == '4')
        new_active_camera_id = 4;
    if (key == '5')
        new_active_camera_id = 5;
    
    if (new_active_camera_id != -1)
    {
        ramCameraManager::instance().setEnableInteractiveCamera(false);
        
        active_camera_id = new_active_camera_id;
        ramCameraManager::instance().setActiveCamera(active_camera_id);
        
        ramCameraManager::instance().setEnableInteractiveCamera(true);
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
